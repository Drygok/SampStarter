#include <Windows.h>
#include <string>
#include <iostream>

enum RETCODES {
    OKAY,

    MODULE_KERNEL32_ERROR,
    MODULE_KERNEL32_NOFUNC,

    GAME_START_ERROR,

    INJECTION_ALLOC_ERROR,
    INJECTION_WRITE_ERROR,
    INJECTION_CREATETHREAD_ERROR
};

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    HMODULE kernelModule = GetModuleHandleW(L"kernel32.dll");
    if (!kernelModule) {
        return MODULE_KERNEL32_ERROR;
    }

    FARPROC loadLibraryAddress = GetProcAddress(kernelModule, "LoadLibraryW");
    if (!loadLibraryAddress) {
        return MODULE_KERNEL32_NOFUNC;
    }

    wchar_t currentDirectory[MAX_PATH + 1];
    GetCurrentDirectoryW(MAX_PATH, currentDirectory);
    // MAX_PATH = 260
    // Максимальная длина пути в Windows

    std::wstring copy = L"Автор - Снегирев Максим (vk.com/drygok // drygok.ru // github.com/Drygok/)";

    std::wstring launchFile = L"\\gta_sa.exe";
    std::wstring injectionFiles[] = { L"\\samp.dll" };

    PROCESS_INFORMATION processInfo = { 0 };
    STARTUPINFOW startupInfo = { 0 };

    if (!CreateProcessW((currentDirectory + launchFile).c_str(), GetCommandLineW(), nullptr, nullptr, false, DETACHED_PROCESS | CREATE_SUSPENDED, nullptr, nullptr, &startupInfo, &processInfo)) {
        TerminateProcess(processInfo.hProcess, 0);
        return GAME_START_ERROR;
    }


    for (std::wstring injectionFile : injectionFiles) {
        LPVOID fileStartPointer = VirtualAllocEx(processInfo.hProcess, nullptr, (injectionFile.length() + 1) * sizeof(wchar_t), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (!fileStartPointer) {
            TerminateProcess(processInfo.hProcess, 0);
            return INJECTION_ALLOC_ERROR;
        }

        if (!WriteProcessMemory(processInfo.hProcess, fileStartPointer, injectionFile.c_str(), (injectionFile.length() + 1) * sizeof(wchar_t), nullptr)) {
            TerminateProcess(processInfo.hProcess, 0);
            return INJECTION_WRITE_ERROR;
        }

        HANDLE remoteThread = CreateRemoteThread(processInfo.hProcess, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(loadLibraryAddress), fileStartPointer, CREATE_SUSPENDED, nullptr);
        if (!remoteThread) {
            TerminateProcess(processInfo.hProcess, 0);
            return INJECTION_CREATETHREAD_ERROR;
        }

        ResumeThread(remoteThread);
        //WaitForSingleObject(remoteThread, INFINITE);
    }

    ResumeThread(processInfo.hThread);
    CloseHandle(processInfo.hProcess);
    return 0;
}
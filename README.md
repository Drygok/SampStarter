# SampStarter

This application is designed to simplify the login to the servers of **SA:MP**, **CR:MP**, **AMAZING Russia** and other games on this platform
This application should help developers of launchers and other client applications

***

Это приложение призвано упростить вход на сервера **SA:MP**, **CR:MP**, **АМАЗИНГ Россия** и других игр на этой платформе
Это приложение должно помочь разработчикам лаунчеров и других клиентских приложений

***

To start the game, place the executable file of the application in the game folder and launch it, passing the parameters for launching the multiplayer
  
**Launch parameters for CR:MP, SA:MP and Amazing Russia:**
> -c rcon_password [may be empty, but must be specified to login to the server]
> -h server ip
> -p server port
> -n player name
> -z server password
> -d debug
  
**Example for join to localhost:7777 with Maxim_Alexandrov nickname when game installed to С:\\Games\\GTA\\ from windows console:**
> "C:\\Games\\GTA\\SampStarter.exe" -c -h 127.0.0.1 -p 7777 -n Maxim_Alexandrov

***

Initially, the application starts *gta_sa.exe* and loads *samp.dll* from the game folder, but you can easily change this in the code
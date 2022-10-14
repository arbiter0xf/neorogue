setlocal
    rem Set directories to path for finding SDL dll files.
    set PATH=%~dp0\build\subprojects\SDL2-2.24.1;%~dp0\build\subprojects\SDL2_image-2.6.2
    %~dp0\build\rogue_forever.exe
endlocal

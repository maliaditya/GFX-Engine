cls

powershell -Command "if (-not(Get-Process | Where-Object { $_.MainWindowTitle -eq 'Developer Command Prompt for VS 2019' })) { exit 0 } else { exit 1 }"

:: If not open, run vcvars64.bat to set up environment
if %errorlevel% equ 0 (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
)

del /Q logs\*.log

del *.obj

set VENDOR_LIBS=..\engine\vendor
set ENGINE_DIR=..\engine
set SRC_DIR=.\src
set WINDOWDIR= ..\engine\platform\windows

if exist main.exe (
    del *.exe
    del *.res
)

cl.exe /c /EHsc /I %ENGINE_DIR% /I %WINDOWDIR% /I %VENDOR_LIBS% /I %VENDOR_LIBS%\glew\include %SRC_DIR%\main.cpp %WINDOWDIR%\window.cpp

rc.exe %WINDOWDIR%\Window.rc

link.exe *.obj %WINDOWDIR%\Window.res "/LIBPATH:%WINDOWDIR%\glew\lib\Release\x64" user32.lib gdi32.lib /subsystem:windows

if exist main.exe (
    del *.obj
    del %WINDOWDIR%\*.res
    main.exe
)

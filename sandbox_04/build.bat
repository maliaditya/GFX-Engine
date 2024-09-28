@echo off
cls

:: Check if Developer Command Prompt for VS 2019 is open
powershell -Command "if (-not(Get-Process | Where-Object { $_.MainWindowTitle -eq 'Developer Command Prompt for VS 2019' })) { exit 0 } else { exit 1 }"

:: If not open, run vcvars64.bat to set up environment
if %errorlevel% equ 0 (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
)

:: Clean previous logs and outputs
del /Q logs\*.log
if exist build (
    rd /s /q build
)
mkdir build
mkdir build\obj

:: Set up directories
set VENDOR_LIBS=..\engine\vendor
set ENGINE_DIR=..\engine
set SRC_DIR=.\src
set WINDOWDIR= ..\engine\platform\windows
set OPENGLDIR= ..\engine\platform\opengl
set OBJ_DIR=build\obj
set EXE_DIR=build

:: Compile source files
cl.exe /c /EHsc /I %ENGINE_DIR% /I %OPENGLDIR% /I %WINDOWDIR% /I %VENDOR_LIBS% /I %VENDOR_LIBS%\glew\include %SRC_DIR%\main.cpp   %OPENGLDIR%\shader.cpp   %SRC_DIR%\triangle_mesh.cpp   %WINDOWDIR%\window.cpp   /Fo%OBJ_DIR%\

:: Create resource file
rc.exe %WINDOWDIR%\Window.rc

:: Link object files and create executable
link.exe %OBJ_DIR%\*.obj %WINDOWDIR%\Window.res "/LIBPATH:%VENDOR_LIBS%\glew\lib\Release\x64" user32.lib gdi32.lib /subsystem:windows  /OUT:%EXE_DIR%\main.exe

:: Clean up
if exist main.exe (
    del %OBJ_DIR%\*.obj
    del %WINDOWDIR%\*.res
)

echo Build completed successfully!

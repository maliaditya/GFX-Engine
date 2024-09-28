#include <windows.h>
#include "Window.h"
#include "Experience/Experience.h"

#include <GL/glew.h> // This Must Be Before Including <GL/gl.h>
#include <GL/gl.h>

// OpenGL  libraries
#pragma comment(lib,"GLEW32.lib")
#pragma comment(lib,"OpenGL32.lib")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {
    
    // Variable Declarations
    HWND ghwnd = NULL;
    HDC ghdc = NULL;
    HGLRC ghrc = NULL;
    
    // Create the window
    Engine::Window myWindow(hInstance, iCmdShow, "");

    ghwnd = myWindow.hwnd;


    // Create the Experience object
    Experience exp;

    // Register Experience callbacks to the Window's EventEmitter
    myWindow.eventEmitter.on("update", [&exp]() { exp.update(); });
    myWindow.eventEmitter.on("render", [&exp]() { exp.render(); });


    // Start the game loop
    myWindow.gameLoop();

    return 0;
}
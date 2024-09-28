#pragma once
#include <windows.h>
#include "Window.h"

// ************** Already defined in Window.h *******************
//#include <GL/glew.h> // This Must Be Before Including <GL/gl.h>
//#include <GL/gl.h>
//#pragma comment(lib,"GLEW32.lib")
//#pragma comment(lib,"OpenGL32.lib")

#include "../../engine/utils/Logger.h"

HDC ghdc;
//HGLRC ghrc;
//HWND ghwnd;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {
    // Create the window
    Engine::Window myWindow(hInstance, iCmdShow, "Aditya Shivaji Mali");

    //ghwnd = myWindow.hwnd;
    //ghrc = myWindow.hrc;
    ghdc = myWindow.hdc;

    Logger log("Main.log");

    int initialize();
    void resize(int, int);
    void update();
    void render();

    // Create the Experience object
    //Experience exp;

    // Register Experience callbacks to the Window's EventEmitter
    myWindow.eventEmitter.on("update", [&]() { update(); });
    myWindow.eventEmitter.on("render", [&]() { render(); });
    myWindow.eventEmitter.on("resize", [&]() { resize(myWindow.width, myWindow.height); });
    
    initialize();

    // Show the window
    myWindow.show(); // Make sure to show the window before entering the game loop

    // Start the game loop
    myWindow.gameLoop();

    return 0;
}

int initialize() 
{
    Logger log("Initialize.log");
  
    // Depth related Changes
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Optional
    // glShadeModel(GL_SMOOTH)
    // glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // Clear the screen using blue color
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    log.write("initialize");

    return(0);
}


void resize(int width, int height)
{
    // Code

    if (height == 0)
        height = 1;

    glViewport(0, 0, width, height);
}

void render()
{
    // Code

    Logger log("render.log");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SwapBuffers(ghdc);
}

void update()
{
    // Code

    Logger log("update.log");

}

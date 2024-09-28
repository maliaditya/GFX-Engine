#pragma once
#include <windows.h>
#include "Window.h"

#include <GL/glew.h> // This Must Be Before Including <GL/gl.h>
#include <GL/gl.h>
#include "../../engine/utils/Logger.h"
// OpenGL  libraries
#pragma comment(lib,"GLEW32.lib")
#pragma comment(lib,"OpenGL32.lib")

HDC ghdc;
HGLRC ghrc;
HWND ghwnd;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {
    // Create the window
    Engine::Window myWindow(hInstance, iCmdShow, "Aditya Shivaji Mali");
    
    // Set Default context to false if you need to creaate your custom context
    myWindow.setDefaultContext = FALSE;

    ghwnd = myWindow.hwnd;
       
        
    Logger log("Main.log");

    int initialize(HWND);
    void resize(int, int);
    void update();
    void render();

    int iRetVal = 0;

    // Create the Experience object
    //Experience exp;

    // Register Experience callbacks to the Window's EventEmitter
    myWindow.eventEmitter.on("update", [&]() { update(); });
    myWindow.eventEmitter.on("render", [&]() { render(); });
    myWindow.eventEmitter.on("resize", [&]() { resize(myWindow.width, myWindow.height); });
    
    iRetVal = initialize(ghwnd);

    // Show the window
    myWindow.show(); // Make sure to show the window before entering the game loop

    // Start the game loop
    myWindow.gameLoop();


    switch (iRetVal)
    {
    case -1:
        log.write("Choose Pixel Format Failed\n");
        break;
    case 0:
            log.write("success \n");
            break;

    }

    return 0;
}

int initialize(HWND ghwnd) 
{
    Logger log("Initialize.log");
    
    // Function declarations
    void printGLInfo(void);

    // Variable declarations
    PIXELFORMATDESCRIPTOR pfd;
    int iPixelFormatIndex = 0;

    //code
    //  Initialization of pixel format descriptor
    ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cRedBits = 8;
    pfd.cGreenBits = 8;
    pfd.cBlueBits = 8;
    pfd.cAlphaBits = 8;
    pfd.cDepthBits = 32;

    ghdc = GetDC(ghwnd);

    // Choose pixel format descriptor
    iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
    if (iPixelFormatIndex == 0)
        return(-1);

    // Set the chosen pixel format
    if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
        return(-2);

    // Create OpenGL rendering context
    ghrc = wglCreateContext(ghdc);
    if (ghrc == NULL)
        return(-3);

    // Make rendering context as the current context
    if (wglMakeCurrent(ghdc, ghrc) == FALSE)
        return(-4);

    // Opengl Code

    // GLEW Initialization
    if (glewInit() != GLEW_OK)
    {
        return(-5);
    }

    // Print OpenGL info
    printGLInfo();

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

void printGLInfo(void)
{
    // Code
    
    Logger log("printGLInfo.log");
   
    // Local Variable declarations
    GLint numExtensions;

    log.debug("OpenGL Vendor: %s \n", glGetString(GL_VENDOR));
    log.debug("OpenGL Renderer: %s \n", glGetString(GL_RENDERER));
    log.debug("OpenGL Version: %s \n", glGetString(GL_VERSION));
    log.debug("GLSL Version : %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

    log.debug("Number Of Supported Extentions: %d \n", numExtensions);

    for (int i = 0; i < numExtensions; i++)
    {
        log.debug(" %s \n", glGetStringi(GL_EXTENSIONS, i));

    }
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

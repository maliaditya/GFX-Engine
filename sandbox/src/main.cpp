#include <windows.h>
#include "Window.h"
#include "utils/EventEmitter.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {
    Engine::Window myWindow(hInstance, iCmdShow, "Aditya Shivaji Mali");



    // Main loop
    while (myWindow.IsOpen()) {
        // You could add any additional update logic here if needed
    }

    return 0;
}

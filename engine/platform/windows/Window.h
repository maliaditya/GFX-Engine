#pragma once
#include <windows.h>
#include "../../utils/Logger.h"
#include "../../utils/EventEmitter.h"

#define MYICON  101
#define WINWIDTH  800
#define WINHEIGHT  600

namespace Engine {
		
	class Window
	{
	public:
		Logger log;
		EventEmitter eventEmitter;  // EventEmitter to manage events

		Window (HINSTANCE hInstance, int iCmdShow, const char* title);
		~Window();

		void show();
		void gameLoop();
		bool isOpen() const;
		void toggleFullscreen();

	private:
		// variable declaration
		WNDCLASSEX wndclass;
		HWND hwnd;
		MSG msg;
		TCHAR szAppName[256]; // Change to fixed-size array for title storage
		RECT rc;
		BOOL bFullscreen = FALSE;
		BOOL bActiveWindow = FALSE;
		BOOL bDone = FALSE;

		int initialize(HINSTANCE hInstance);
		void cleanup(); // Cleanup method

		static LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	};
}

#pragma once
#include <windows.h>
#define MYICON  101
#define WINWIDTH  800
#define WINHEIGHT  600

namespace Engine {
		
	class Window
	{
	public:
		Window(HINSTANCE hInstance, int iCmdShow, const char* title);
		~Window();

		void show();
		void update();
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

		int initialize(HINSTANCE hInstance);
		void centerWindow();

		static LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	};
}

#pragma once
#include <windows.h>

namespace Engine {

	class Window
	{
	public:
		Window(HINSTANCE hInstance, int iCmdShow, const char* title);
		~Window();

		void Show();
		void Update();
		bool IsOpen() const;

	private:
		WNDCLASSEX wndclass;
		HWND hwnd;
		MSG msg;
		TCHAR szAppName[256]; // Change to fixed-size array for title storage

		int initialize(HINSTANCE hInstance);
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	};
}

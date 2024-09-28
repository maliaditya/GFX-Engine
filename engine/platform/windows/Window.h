#pragma once
#include <windows.h>

// ImGUI Headers
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_opengl3.h"
#include "Imgui/imgui_impl_win32.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "utils/Logger.h"
#include "utils/EventEmitter.h"
#include <GL/glew.h> // This Must Be Before Including <GL/gl.h>
#include <GL/gl.h>

// OpenGL  libraries
#pragma comment(lib,"GLEW32.lib")
#pragma comment(lib,"OpenGL32.lib")

#define MYICON  101
#define WINWIDTH  800
#define WINHEIGHT  600

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWhwnnd, UINT msg, WPARAM wParam, LPARAM lParam);


namespace Engine {
		
	class Window
	{
	public:
		Logger log;
		EventEmitter eventEmitter;  // EventEmitter to manage events
		HWND hwnd;
		HDC hdc;
		HGLRC hrc;
		BOOL setDefaultContext = TRUE;

		Window (HINSTANCE hInstance, int iCmdShow, const char* title);
		~Window();

		void show();
		void gameLoop();
		bool isOpen() const;
		void toggleFullscreen();
		void uninitialize(); // Cleanup method

		int width = WINWIDTH;
		int height = WINHEIGHT;


	private:
		// variable declaration
		WNDCLASSEX wndclass;
		MSG msg;
		TCHAR szAppName[256]; // Change to fixed-size array for title storage
		RECT rc;
		BOOL bFullscreen = FALSE;
		BOOL bActiveWindow = TRUE;
		BOOL bDone = FALSE;
		


		int initialize(HINSTANCE hInstance);
		int setContext();
		void resize();

		static LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	};
}

#include "Window.h"

namespace Engine {

	Window::Window(HINSTANCE hInstance, int iCmdShow, const char* title) {
		// Copy title to szAppName
		strncpy(szAppName, title, sizeof(szAppName) - 1);
		szAppName[sizeof(szAppName) - 1] = '\0'; // Ensure null-termination

		initialize(hInstance);
		Show();
		Update();
	}

	Window::~Window() {
		// Clean up resources if needed
	}

	int Window::initialize(HINSTANCE hInstance) {
		wndclass.cbSize = sizeof(WNDCLASSEX);
		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.lpfnWndProc = WndProc;
		wndclass.hInstance = hInstance;
		wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.lpszClassName = szAppName;
		wndclass.lpszMenuName = NULL;
		wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		// Register WNDCLASSEX 
		RegisterClassEx(&wndclass);

		// Create the window
		hwnd = CreateWindow(
			szAppName,
			szAppName, // Use the title for both window name and class name
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL,
			NULL,
			hInstance,
			NULL
		);

		return hwnd ? 0 : -1; // Return success or failure
	}

	void Window::Show() {
		ShowWindow(hwnd, SW_SHOW);
		UpdateWindow(hwnd);
	}

	void Window::Update() {
		// Message loop
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	bool Window::IsOpen() const {
		return msg.message != WM_QUIT; // Check if the message is WM_QUIT
	}

	LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
		switch (iMsg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			break;
		}

		return DefWindowProc(hwnd, iMsg, wParam, lParam);
	}
}

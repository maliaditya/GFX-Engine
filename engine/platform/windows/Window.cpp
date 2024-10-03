#include "Window.h"

namespace Engine {



	Window::Window(HINSTANCE hInstance, int iCmdShow, const char* title) : log("window.log") {
		// Copy title to szAppName
		strncpy(szAppName, title, sizeof(szAppName) - 1);
		szAppName[sizeof(szAppName) - 1] = '\0'; // Ensure null-termination

		initialize(hInstance);

        setDefaultContext ? setContext() : (void)0;
		
        show();
	}

    int Window::setContext() {

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

        hdc = GetDC(hwnd);

        // Choose pixel format descriptor
        iPixelFormatIndex = ChoosePixelFormat(hdc, &pfd);
        if (iPixelFormatIndex == 0)
            return(-1);

        // Set the chosen pixel format
        if (SetPixelFormat(hdc, iPixelFormatIndex, &pfd) == FALSE)
            return(-2);

        // Create OpenGL rendering context
        hrc = wglCreateContext(hdc);
        if (hrc == NULL)
            return(-3);

        // Make rendering context as the current context
        if (wglMakeCurrent(hdc, hrc) == FALSE)
            return(-4);

        // GLEW Initialization
        if (glewInit() != GLEW_OK)
        {
            return(-5);
        }
      
        return(0);
    }

    void Window::resize()
    {
        eventEmitter.trigger("resize"); // Trigger the update event
    }

	Window::~Window() {
        uninitialize();
		// Clean up resources if needed
	}
    int Window::initialize(HINSTANCE hInstance) {
        wndclass.cbSize = sizeof(WNDCLASSEX);
        wndclass.style = CS_HREDRAW | CS_VREDRAW;
        wndclass.cbClsExtra = 0;
        wndclass.cbWndExtra = 0;
        wndclass.lpfnWndProc = WndProc;
        wndclass.hInstance = hInstance;
        wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
        wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);

        const char* windowClassName = "EngineWindowClass";
        wndclass.lpszClassName = windowClassName; // Class name
        wndclass.lpszMenuName = NULL;
        wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

        // Register WNDCLASSEX
        if (!RegisterClassEx(&wndclass)) {
            MessageBox(NULL, "Failed to register window class!", "Error", MB_ICONERROR);
            return -1; // Registration failed
        }

        // Get size of Work Area of the window 
        SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);


        // Calculate the center position
        int x = rc.left + (rc.right - rc.left - WINWIDTH) / 2;
        int y = rc.top + (rc.bottom - rc.top - WINHEIGHT) / 2;

        // Create the window
        hwnd = CreateWindow(
            windowClassName, // Class name
            szAppName, // Title
            WS_OVERLAPPEDWINDOW,
            x,
            y,
            WINWIDTH,
            WINHEIGHT,
            NULL,
            NULL,
            hInstance,
            NULL
        );

        // Check if the window was created successfully
        if (!hwnd) {
            MessageBox(NULL, "Window creation failed!", "Error", MB_ICONERROR);
            return -1;
        }

        // Foregrounding and focusing the window
        SetForegroundWindow(hwnd);
        SetFocus(hwnd);

		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

        return 0; // Return success
    }

	void Window::show() {
		ShowWindow(hwnd, SW_SHOW);
		UpdateWindow(hwnd);
	}

	void Window::gameLoop() {
      
		// Message loop
        while (!bDone) {
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) {
                    bDone = true;

                }
                else {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }

            if (bActiveWindow) {
                eventEmitter.trigger("update"); // Trigger the update event
                eventEmitter.trigger("render"); // Trigger the render event
            }
        }
	}

    void Window::toggleFullscreen()
    {
		// Variable declarations
		static DWORD dwStyle;
		static WINDOWPLACEMENT wp;
		MONITORINFO mi;


		// Code
		wp.length = sizeof(WINDOWPLACEMENT);

		if (bFullscreen == FALSE)
		{
			dwStyle = GetWindowLong(hwnd, GWL_STYLE);
			if (dwStyle & WS_OVERLAPPEDWINDOW)
			{
				mi.cbSize = sizeof(MONITORINFO);
				if (GetWindowPlacement(hwnd, &wp) && GetMonitorInfo(MonitorFromWindow(hwnd, MONITORINFOF_PRIMARY), &mi))
				{
					SetWindowLong(hwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
					SetWindowPos(hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
				}
				ShowCursor(FALSE);
				bFullscreen = TRUE;
			}
		}
		else
		{
			SetWindowLong(hwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
			SetWindowPlacement(hwnd, &wp);
			SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

			ShowCursor(TRUE);
			bFullscreen = FALSE;
		}

    }

	bool Window::isOpen() const {
		return msg.message != WM_QUIT; // Check if the message is WM_QUIT
	}


    void Window::uninitialize() {
        
        eventEmitter.trigger("uninitialize");

        if (wglGetCurrentContext() == hrc)
        {
            wglMakeCurrent(NULL, NULL);
        }

        if (hrc)
        {
            wglDeleteContext(hrc);
            hrc = NULL;
        }

        if (hdc)
        {
            ReleaseDC(hwnd, hdc);
            hdc = NULL;
        }

        if (hwnd) {
            DestroyWindow(hwnd); // Destroy the window if it's created
            hwnd = nullptr; // Nullify the handle to avoid dangling pointer
        }

        // Add any other cleanup logic if needed, such as releasing resources
    }



    LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {

        ImGui_ImplWin32_WndProcHandler(hwnd, iMsg, wParam, lParam);
        Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

        if (!window) {
            return DefWindowProc(hwnd, iMsg, wParam, lParam);
        }

        switch (iMsg) {

        case WM_SETFOCUS:
            window->log.write("WM_SETFOCUS");
            window->log.debug("window->WM_SETFOCUS", window->bActiveWindow);
            window->bActiveWindow = TRUE;
            break;
        case WM_KILLFOCUS:
            window->log.write("WM_KILLFOCUS");
            window->log.debug("window->WM_KILLFOCUS", window->bActiveWindow);
            window->bActiveWindow = FALSE;
            break;

        case WM_ERASEBKGND:
            break;

        case WM_CHAR:
            switch (wParam)
            {
            case 'F':
            case 'f':
                if (window) {
                    window->toggleFullscreen();
                }
                break;

            default:
                break;
            }
            break;

       
        case WM_SIZE:
            window->width = LOWORD(lParam);
            window->height = HIWORD(lParam);
            window->resize();
            break;

        case WM_KEYDOWN:
            window->camera.update(hwnd, iMsg, wParam, lParam);
            switch (wParam)
            {
            case 27:
                DestroyWindow(hwnd);
                break;

            default:
                break;
            }
            break;


        case WM_MOUSEMOVE:
            if (wParam & MK_SHIFT) {
                window->camera.update(hwnd, iMsg, wParam, lParam);
            }
            break;
        case WM_MOUSEWHEEL:
            if (wParam & MK_SHIFT) {
                window->camera.update(hwnd, iMsg, wParam, lParam);
                window->width = LOWORD(lParam);
                window->height = HIWORD(lParam);
                window->resize();
            }
            break;
        case WM_DESTROY:
            window->uninitialize();
            PostQuitMessage(0);
            break;

        default:
            break;
        }

        return DefWindowProc(hwnd, iMsg, wParam, lParam);
    }
}

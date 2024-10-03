#pragma once
#include "windows.h"

// Header Files
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_opengl3.h"
#include "Imgui/imgui_impl_win32.h"


#include "mesh.h"

class ImguiManager {
public:
	ImguiManager(HWND hwnd);
	~ImguiManager();

	void beginFrame();
	//void renderUI(Mesh& mesh); // pass mesh object to this function
	void endFrame();
	void renderGUI(Mesh& mesh);
	void destroy();
	void imguiCustomColorTheme();
};
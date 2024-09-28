#pragma once
#include <windows.h>

// Header Files
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_opengl3.h"
#include "Imgui/imgui_impl_win32.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// GLM Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Engine Headers
#include "Window.h"
#include "shader.h"
#include "utils/Logger.h"

// My header files
#include "Experience/World/objects/Triangle.h"

// ************** OpenGL Headers already defined in Window.h *******************

HDC ghdc;
GLuint shaderProgramObject;

GLuint mvpMatrixUniform;
glm::mat4 perspectiveProjectionMatrix;




//HGLRC ghrc;
HWND ghwnd;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {
    // Create the window
    Engine::Window myWindow(hInstance, iCmdShow, "Aditya Shivaji Mali");

    ghwnd = myWindow.hwnd;
    //ghrc = myWindow.hrc;
    ghdc = myWindow.hdc;

    Logger log("Main.log");

    int initialize();
    void resize(int, int);
    void update();
    void render();
    void uninitialize();
    void imguiInit();
    void imguiDestroy();
    // Create the Experience object
    //Experience exp;

    // Register Experience callbacks to the Window's EventEmitter
    myWindow.eventEmitter.on("update", [&]() { update(); });
    myWindow.eventEmitter.on("render", [&]() { render(); });
    myWindow.eventEmitter.on("uninitialize", [&]() { uninitialize(); });
    myWindow.eventEmitter.on("resize", [&]() { resize(myWindow.width, myWindow.height); });
    
    initialize();

    // Show the window
    myWindow.show(); // Make sure to show the window before entering the game loop

    imguiInit();
    // Start the game loop
    myWindow.gameLoop();

    
    imguiDestroy();

    return 0;
}

// Forward declare message handler from imgui_impl_win32.cpp
//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWhwnnd, UINT msg, WPARAM wParam, LPARAM lParam);

TriangleMesh triangle;

int initialize() 
{
	void uninitialize();
	void resize(int, int);

	Logger iniLog("initialize.log");
	Shader shader;
	
	shader.initialize(&triangle, triangle.localShaderPaths);
	shaderProgramObject = shader.getShaderProgram("triangle");
	triangle.init();

    // Depth related Changes
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    perspectiveProjectionMatrix = glm::mat4(1.0);

    // Clear the screen using blue color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	resize(800, 600);

    return(0);
}


void resize(int width, int height)
{
    // Code

    if (height == 0)
        height = 1;

    glViewport(0, 0, width, height);
	perspectiveProjectionMatrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

ImVec2 availableSpaceProperties;

void render()
{
    // Code
    Logger log("render.log");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  

    //imGUI();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowBgAlpha(0.35f);
    ImGui::DockSpaceOverViewport();

    // Other ImGui elements (optional)
    ImGui::Begin("Properties");
    availableSpaceProperties = ImGui::GetContentRegionAvail(); // used for viewport 
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::Separator();

    ImGui::End();


    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::Begin("Scene");
    ImVec2 availableSpace = ImGui::GetContentRegionAvail();

    // Calculate half of available space for centering
    float halfWidth = availableSpace.x / 2.0f;
    float halfHeight = availableSpace.y / 2.0f;
    //glViewport(availableSpaceProperties.x, 0, availableSpace.x, availableSpace.y);

    resize( availableSpace.x, availableSpace.y);

	// Use The Shader Program Object
	glUseProgram(shaderProgramObject);

	// Transformations
	glm::mat4 translationMatrix = glm::mat4(1.0);
	glm::mat4 modelViewMatrix = glm::mat4(1.0);
	glm::mat4 modelViewProjectionMatrix = glm::mat4(1.0);

    modelViewMatrix = translate(modelViewMatrix, glm::vec3(0.0f, 0.0f, -6.0f));
    modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));

	triangle.render();

	// unuser the shader program object
	glUseProgram(0);


    // End Scene Window
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



    SwapBuffers(ghdc);
}

void update()
{
    // Code

    Logger log("update.log");
}

void uninitialize()
{
    Logger log("Uninitialize.log");

}



// IMGUI CODE

void imguiInit()
{
    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();

    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

    // Headers
    colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
    colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };



    // Setup Platform/Renderer bindings
    ImGui_ImplWin32_Init(ghwnd);
    ImGui_ImplOpenGL3_Init("#version 130");



    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


}


void imguiDestroy()
{

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
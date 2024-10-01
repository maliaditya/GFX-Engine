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
#include "Experience/World/objects/plane.h"

// ************** OpenGL Headers already defined in Window.h *******************


HDC ghdc;
GLuint shaderProgramObject;

GLuint modelMatrixUniform;
GLuint viewMatrixUniform;
GLuint projectionMatrixUniform;
glm::mat4 perspectiveProjectionMatrix;




//HGLRC ghrc;
HWND ghwnd;
Camera* camera;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {
    // Create the window
    Engine::Window myWindow(hInstance, iCmdShow, "Aditya Shivaji Mali");

    ghwnd = myWindow.hwnd;
    //ghrc = myWindow.hrc;
    ghdc = myWindow.hdc;
    camera = &myWindow.camera;
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

Plane plane(10,10,1024, 1024);

int initialize() 
{
	void uninitialize();
	void resize(int, int);

	Logger iniLog("initialize.log");
	Shader shader;
	
	shader.initialize(&plane, plane.localShaderPaths);
	shaderProgramObject = shader.getShaderProgram("plane");
    plane.init();

    // Posted Linked Retrieving /Getting uniform location  from the shader program object.
    modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_modelMatrix");
    viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_viewMatrix");
    projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_projectionMatrix");


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

    // Create a slider to adjust the bigWavesElevation
    ImGui::SliderFloat("BigWavesElevation", &plane.bigWavesElevation, 0.0f, 1.0f, "%.3f"); // Adjust the range as needed
    ImGui::SliderFloat("BigWavesFrequencyX", &plane.bigWavesFrequency.x, 0.0f, 10.0f, "%.3f"); // Adjust the range as needed
    ImGui::SliderFloat("BigWavesFrequencyZ", &plane.bigWavesFrequency.y, 0.0f, 10.0f, "%.3f"); // Adjust the range as needed
    ImGui::SliderFloat("BigWavesSpeed", &plane.bigWavesSpeed, 0.0f, 4.0f, "%.3f"); // Adjust the range as needed
    ImGui::SliderFloat("colorOffset", &plane.colorOffset, 0.0f, 4.0f, "%.3f"); // Adjust the range as needed
    ImGui::SliderFloat("colorMultiplier", &plane.colorMultiplier, 0.0f, 4.0f, "%.3f"); // Adjust the range as needed
    ImGui::SliderFloat("smallWavesElevation", &plane.smallWavesElevation, 0.0f, 1.0f, "%.3f"); // Adjust the range as needed
    ImGui::SliderFloat("smallWavesFrequency", &plane.smallWavesFrequency, 0.0f, 30.0f, "%.3f"); // Adjust the range as needed
    ImGui::SliderFloat("smallWavesSpeed", &plane.smallWavesSpeed, 0.0f, 4.0f, "%.3f"); // Adjust the range as needed
    ImGui::SliderFloat("smallWavesIterations", &plane.smallWavesIterations, 0.0f, 5.0f, "%1.0f"); // Adjust the range as needed

    // colors
  // Normalize the colors by dividing by 255.0f
    ImVec4 depthColor = ImVec4(72 / 255.0f, 61 / 255.0f, 139 / 255.0f, 1.0f);  // #186691
    ImVec4 surfaceColor = ImVec4(135 / 255.0f, 206 / 255.0f, 235 / 255.0f, 1.0f);  // #9BD8FF

    // Check if the user changes the colors using the color picker
    if (ImGui::ColorEdit3("depthColor", (float*)&depthColor)) {
        // Update the plane's depth color only if changed
        plane.depthColor.x = depthColor.x;
        plane.depthColor.y = depthColor.y;
        plane.depthColor.z = depthColor.z;
    }

    if (ImGui::ColorEdit3("surfaceColor", (float*)&surfaceColor)) {
        // Update the plane's surface color only if changed
        plane.surfaceColor.x = surfaceColor.x;
        plane.surfaceColor.y = surfaceColor.y;
        plane.surfaceColor.z = surfaceColor.z;
    }
    ImGui::End();


    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::Begin("Scene");
    ImVec2 availableSpace = ImGui::GetContentRegionAvail();

    // Calculate half of available space for centering
    float halfWidth = availableSpace.x / 2.0f;
    float halfHeight = availableSpace.y / 2.0f;
    //glViewport(availableSpaceProperties.x, 0, availableSpace.x, availableSpace.y);

    resize( availableSpace.x, availableSpace.y);
    GLfloat angle = 45;

	// Use The Shader Program Object
	glUseProgram(shaderProgramObject);
    // Transformations
    glm::mat4 modelMatrix = glm::mat4(1.0);
    glm::mat4 viewMatrix = camera->getMatrix();
    glm::mat4 translationMatrix = glm::mat4(1.0);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -6.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, glm::value_ptr(perspectiveProjectionMatrix));

    plane.render();

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
    plane.update();
  

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
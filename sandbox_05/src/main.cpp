#pragma once
#include <windows.h>

// Header Files
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_opengl3.h"
#include "Imgui/imgui_impl_win32.h"


// GLM Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Engine Headers
#include "Window.h"
#include "shader.h"
#include "utils/Logger.h"
#include "ImguiManager.h"

// My header files
#include "Experience/World/objects/Model.h"

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
Mesh* meshPointer = nullptr;
ImguiManager* imguiManager = nullptr;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {
    // Create the window
    Engine::Window myWindow(hInstance, iCmdShow, "Aditya Shivaji Mali");

    ghwnd = myWindow.hwnd;
    ghdc = myWindow.hdc;
    camera = &myWindow.camera;

    int initialize();
    void resize(int, int);
    void update();
    void render();
    void uninitialize();
    void imguiInit();
    void imguiDestroy();
    // Create the Experience object

    // Register Experience callbacks to the Window's EventEmitter
    myWindow.eventEmitter.on("update", [&]() { update(); });
    myWindow.eventEmitter.on("render", [&]() { render(); });
    myWindow.eventEmitter.on("uninitialize", [&]() { uninitialize(); });
    myWindow.eventEmitter.on("resize", [&]() { resize(myWindow.width, myWindow.height); });
    
    //meshPointer = new Plane(10, 10, 500, 500);
   // meshPointer = new Galaxy();
      meshPointer = new Model();

    initialize();

    // Show the window
    myWindow.show(); // Make sure to show the window before entering the game loop

    // Initialize ImGui
    imguiManager = new ImguiManager(ghwnd);
    
    // Start the game loop
    myWindow.gameLoop();

    // Destroy Imgui
    imguiManager->destroy();

    return 0;
}

int initialize() 
{
	void uninitialize();
	void resize(int, int);

    if (meshPointer) {
        Logger log("initialize.log");
         meshPointer->initializeShaders();
        
        // Posted Linked Retrieving /Getting uniform location  from the shader program object.
        modelMatrixUniform = glGetUniformLocation(meshPointer->getShaderProgramObject(), "u_modelMatrix");
        viewMatrixUniform = glGetUniformLocation(meshPointer->getShaderProgramObject(), "u_viewMatrix");
        projectionMatrixUniform = glGetUniformLocation(meshPointer->getShaderProgramObject(), "u_projectionMatrix");

    }

    // Points
    glEnable(GL_PROGRAM_POINT_SIZE);
    
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

GLfloat angle = 0.0f;

void render()
{
    // Code
    void resize(int width, int height); 

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
    imguiManager->beginFrame();
    
    imguiManager->renderGUI(*meshPointer);

    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::Begin("Scene");

    ImVec2 availableSpace = ImGui::GetContentRegionAvail();
    resize( availableSpace.x, availableSpace.y);

    // Transformations
    glm::mat4 viewMatrix = camera->getMatrix();
    glm::mat4 translationMatrix = glm::mat4(1.0);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -4.0f));
	// Use The Shader Program Object
	glUseProgram(meshPointer->getShaderProgramObject());

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(meshPointer->getModelMatrix()));
    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, glm::value_ptr(perspectiveProjectionMatrix));

    meshPointer->render();

	// unuser the shader program object
	glUseProgram(0);

    // End Scene Window
    ImGui::End();

    imguiManager->endFrame();

    SwapBuffers(ghdc);

}

void resize(int width, int height)
{
    // Code

    if (height == 0)
        height = 1;

    glViewport(0, 0, width, height);
    perspectiveProjectionMatrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

}

void update()
{
    // Code
    angle += 0.01;
    meshPointer->update();
}

void uninitialize()
{

}


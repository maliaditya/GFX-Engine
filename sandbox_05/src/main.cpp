#pragma once
#include <windows.h>

// GLM Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Engine Headers
#include "Window.h"
#include "shader.h"
#include "utils/Logger.h"
#include "include/vmath.h"

// My header files
#include "Experience/World/objects/Triangle.h"

// ************** OpenGL Headers already defined in Window.h *******************

HDC ghdc;
GLuint shaderProgramObject;

GLuint mvpMatrixUniform;
glm::mat4 perspectiveProjectionMatrix;


//HGLRC ghrc;
//HWND ghwnd;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {
    // Create the window
    Engine::Window myWindow(hInstance, iCmdShow, "Aditya Shivaji Mali");

    //ghwnd = myWindow.hwnd;
    //ghrc = myWindow.hrc;
    ghdc = myWindow.hdc;

    Logger log("Main.log");

    int initialize();
    void resize(int, int);
    void update();
    void render();
    void uninitialize();

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

    // Start the game loop
    myWindow.gameLoop();

    return 0;
}
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

void render()
{
    // Code

    Logger log("render.log");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	

    SwapBuffers(ghdc);
}

void update()
{
    // Code

    Logger log("update.log");

}

void uninitialize()
{

}

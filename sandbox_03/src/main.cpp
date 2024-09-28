#pragma once
#include <windows.h>
#include "Window.h"
#include "../../engine/include/vmath.h"
// ************** Already defined in Window.h *******************
//#include <GL/glew.h> // This Must Be Before Including <GL/gl.h>
//#include <GL/gl.h>
//#pragma comment(lib,"GLEW32.lib")
//#pragma comment(lib,"OpenGL32.lib")

#include "../../engine/utils/Logger.h"

HDC ghdc;
GLuint shaderProgramObject;

using namespace vmath;

enum
{
	ASM_ATTRIBUTE_POSITION = 0,
	ASM_ATTRIBUTE_COLOR,
	ASM_ATTRIBUTE_NORMAL,
	ASM_ATTRIBUTE_TEXTURE0

};
GLuint VAO;
GLuint VBO_POSITION;
GLuint VBO_COLOR;
GLuint mvpMatrixUniform;
mat4 perspectiveProjectionMatrix;


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

int initialize() 
{
	void uninitialize();
	void resize(int, int);

	Logger iniLog("initialize.log");

	// Vertex Shader 
	const GLchar* vertexShaderSourceCode =
		"#version 460 core" \
		"\n" \
		"in vec4 a_position;" \
		"in vec4 a_color;" \
		"uniform mat4 u_mvpMatrix;" \
		"out vec4 a_color_out;" \
		"void main(void)" \
		"{" \
		"a_color_out = a_color;" \
		"gl_Position = u_mvpMatrix * a_position;" \
		"}";


	// Creat vertexShaderObject
	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);

	// compile vertex shader code
	glCompileShader(vertexShaderObject);

	GLint status;
	GLint infoLogLength;
	char* log = NULL;

	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			log = (char*)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject, infoLogLength, &written, log);
				iniLog.debug("Vertex Shader Compilation log: %s \n", log);
				free(log);
				uninitialize();

			}
		}
	}



	// Fragment Shader
	const GLchar* fragmentShaderSourceCode =
		"#version 460 core" \
		"\n" \
		"in vec4 a_color_out;" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
		"FragColor = a_color_out;" \
		"}";


	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);
	glCompileShader(fragmentShaderObject);

	status = 0;
	infoLogLength = 0;
	log = NULL;

	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			log = (char*)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject, infoLogLength, &written, log);
				iniLog.debug("Fragment Shader Compilation log: %s \n", log);
				free(log);
				uninitialize();

			}
		}
	}

	// shader program object
	shaderProgramObject = glCreateProgram();

	// attach desired shaders 
	glAttachShader(shaderProgramObject, vertexShaderObject);
	glAttachShader(shaderProgramObject, fragmentShaderObject);

	// Pre-linking binding of shader program object with vertex attributes
	glBindAttribLocation(shaderProgramObject, ASM_ATTRIBUTE_POSITION, "a_position");
	glBindAttribLocation(shaderProgramObject, ASM_ATTRIBUTE_COLOR, "a_color");


	// link the
	glLinkProgram(shaderProgramObject);


	// Post Link Retrieving /Getting uniform location  from the shader program object.
	mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_mvpMatrix");

	status = 0;
	infoLogLength = 0;
	log = NULL;


	glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			log = (char*)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObject, infoLogLength, &written, log);
				iniLog.debug("Shader Program Link log: %s \n", log);
				free(log);
				uninitialize();

			}
		}
	}

	// vao and VBO_POSITION realated code
		//Declare vertex data arrays 
	const GLfloat trianglPosition[] = {
										0.0f, 1.0f, 0.0f,
									   -1.0f, -1.f, 0.0f,
										1.0f,-1.0f,0.0f
	};

	const GLfloat trianglColor[] = {
										1.0f,0.0f,0.0f,
										0.0f,1.0f,0.0f,
										0.0f,0.0f,1.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO for position
	//Bind with vertex array object and start recording buffer and buffer data related steps.
	glGenBuffers(1, &VBO_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_POSITION);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trianglPosition), trianglPosition, GL_STATIC_DRAW);
	glVertexAttribPointer(ASM_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(ASM_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// VBO for Color
	glGenBuffers(1, &VBO_COLOR);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_COLOR);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trianglColor), trianglColor, GL_STATIC_DRAW);
	glVertexAttribPointer(ASM_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(ASM_ATTRIBUTE_COLOR);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// Orthographic projection matrix

    // Depth related Changes
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

	perspectiveProjectionMatrix = mat4::identity();

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
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void render()
{
    // Code

    Logger log("render.log");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use The Shader Program Object
	glUseProgram(shaderProgramObject);


	// Transformations
	mat4 translationMatrix = mat4::identity();
	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -4.0f);
	modelViewMatrix = translationMatrix;
	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	//Bind with vertex array object
	glBindVertexArray(VAO);

	// Here the should be the Animation/scenes code
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//Unbind with vertex array object.
	glBindVertexArray(0);



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

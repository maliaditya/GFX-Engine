#include "Cube.h"

const GLfloat Cube::cubePosition[] = {
	// Front face
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	// Back face
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,

	// Left face
	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	// Right face
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,

	 // Top face
	 -0.5f,  0.5f, -0.5f,
	  0.5f,  0.5f, -0.5f,
	  0.5f,  0.5f,  0.5f,
	 -0.5f,  0.5f,  0.5f,

	 // Bottom face
	 -0.5f, -0.5f, -0.5f,
	  0.5f, -0.5f, -0.5f,
	  0.5f, -0.5f,  0.5f,
	 -0.5f, -0.5f,  0.5f
};

const GLfloat Cube::cubeColor[] = {
	// Front face (Red)
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	// Back face (Green)
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	// Left face (Blue)
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	// Right face (Yellow)
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,

	// Top face (Cyan)
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,

	// Bottom face (Magenta)
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f
};

const GLuint Cube::indices[] = 
{
	
	// Front face
	0, 1, 2, 2, 3, 0,
	
	// Back face
	4, 5, 6, 6, 7, 4,

	// Left face
	8, 9, 10, 10, 11, 8,
	
	// Right face
	12, 13, 14, 14, 15, 12,
	
	// Top face
	16, 17, 18, 18, 19, 16,
	
	// Bottom face
	20, 21, 22, 22, 23, 20

};

Cube::Cube() :
	log("Cube.log"),
	angle(0.0f),
	localShaderPaths(
		{
			{GL_VERTEX_SHADER, "src/shader/triangle/triangle.vert"},
			{GL_FRAGMENT_SHADER,"src/shader/triangle/triangle.frag"}

		}
	) 
{
	// Code
	setModelMatrix();
}

Cube::~Cube()
{
	uninitializer(); // Ensure resources are cleaned up
}

void Cube::renderGUI()
{

	// ImGui Frame preparation
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::DockSpaceOverViewport();

	// Other ImGui elements
	ImGui::Begin("Properties");

	ImGui::End();

}


void Cube::update()
{
	angle += 0.01f;
	updateModelMatrix();
}


void Cube::setModelMatrix()
{
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -6.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	log.write("1: modelmatrix");
}


GLuint Cube::initializeShaders()
{
	// Code

	Shader shader;

	// Initialize shader with the paths from localShaderPaths

	shader.initialize(this, localShaderPaths);
	shaderProgramObject = shader.getShaderProgram(shader.shaderName);
	log.debug("initializeShaders :shaderProgramObject", shaderProgramObject);

	return shaderProgramObject;
}

GLuint Cube::getShaderProgramObject() const
{
	return shaderProgramObject;
}

glm::mat4 Cube::getModelMatrix() const
{
	return modelMatrix;
}

void Cube::init() {

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Bind with vertex array object and start recording buffer and buffer data related steps.
	glGenBuffers(1, &VBO_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_POSITION);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubePosition), cubePosition, GL_STATIC_DRAW);
	glVertexAttribPointer(ASM_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(ASM_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// VBO for Color
	glGenBuffers(1, &VBO_COLOR);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_COLOR);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColor), cubeColor, GL_STATIC_DRAW);
	glVertexAttribPointer(ASM_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(ASM_ATTRIBUTE_COLOR);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

}


void Cube::bindAttributeLocationsAndLink(GLuint shaderProgramObject) {
	glBindAttribLocation(shaderProgramObject, ASM_ATTRIBUTE_POSITION, "a_position");
	glBindAttribLocation(shaderProgramObject, ASM_ATTRIBUTE_COLOR, "a_color");
	glLinkProgram(shaderProgramObject);
}

void Cube::updateModelMatrix() {
	modelMatrix = glm::mat4(1.0); // Reset to identity matrix
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -6.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Cube::render()
{
	//code
	//Bind with vertex array object
	glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glDrawElements(GL_TRIANGLES, sizeof(indices)*sizeof(indices[0]), GL_UNSIGNED_INT, 0); // Use this with index buffer
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

void Cube::uninitializer() {

	if (VBO_POSITION) {
		glDeleteBuffers(1, &VBO_POSITION);
		VBO_POSITION = 0;
	}
	if (VBO_COLOR) {
		glDeleteBuffers(1, &VBO_COLOR);
		VBO_COLOR = 0;
	}
	if (VAO) {
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
}


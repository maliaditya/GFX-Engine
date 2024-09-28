#include "Triangle.h"

const GLfloat TriangleMesh::trianglPosition[] = {
	0.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f
};

const GLfloat TriangleMesh::trianglColor[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};

TriangleMesh::TriangleMesh() :
	localShaderPaths(
		{
			{GL_VERTEX_SHADER, "src/shader/triangle/triangle.vert"},
			{GL_FRAGMENT_SHADER,"src/shader/triangle/triangle.frag"}

		}
	) {
	//code
//D:\Aadis GFX LAB\GFX - Engine\sandbox_04/src/shader/triangle/triangleFragment.glsl
}

TriangleMesh::~TriangleMesh()
{
	uninitializer(); // Ensure resources are cleaned up
}




void TriangleMesh::init() {
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

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

}


void TriangleMesh::bindAttributeLocationsAndLink(GLuint shaderProgramObject) {
	glBindAttribLocation(shaderProgramObject, ASM_ATTRIBUTE_POSITION, "a_position");
	glBindAttribLocation(shaderProgramObject, ASM_ATTRIBUTE_COLOR, "a_color");
	glLinkProgram(shaderProgramObject);
}


void TriangleMesh::render() 
{
	//code
	//Bind with vertex array object
	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(0);

}

void TriangleMesh::uninitializer() {

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


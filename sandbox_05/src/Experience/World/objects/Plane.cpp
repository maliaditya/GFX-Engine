#include "Plane.h"
#include <chrono>
/*
const GLfloat Plane::planePositions[] = {
	
	// Front face (a simple 2D square)

	-0.5f, -0.5f,  0.0f,
	 0.5f, -0.5f,  0.0f,
	 0.5f,  0.5f,  0.0f,
	-0.5f,  0.5f,  0.0f,
};

const GLfloat Plane::planeColors[] = {
	
	// Colors for each vertex

	1.0f, 0.0f, 0.0f,  // Red
	0.0f, 1.0f, 0.0f,  // Green
	0.0f, 0.0f, 1.0f,  // Blue
	1.0f, 1.0f, 0.0f,  // Yellow
};

const GLuint Plane::planeIndices[] = {
	
	// Indices for the two triangles forming the plane

	0, 1, 2,
	2, 3, 0,
};
*/


Plane::Plane(unsigned int width, unsigned int height, unsigned int gridX, unsigned int gridZ) :
	
	// Initialization
	width(width),
	height(height),
	gridX(gridX),
	gridZ(gridZ),
	VAO(0),
	VBO_COLOR(0),
	VBO_POSITION(0),
	EBO(0),
	localShaderPaths(
						{
							{GL_VERTEX_SHADER, "src/shader/plane/plane.vert"},
							{GL_FRAGMENT_SHADER, "src/shader/plane/plane.frag"}
						}
					)
{
	// Code

}

void Plane::generatePlane()
{
	// Code
	float halfWidth = width * 0.5;
	float halfHeight = height * 0.5;

	for (unsigned int i = 0; i <= gridZ; ++i)
	{
		for (unsigned int j = 0; j <= gridX; ++j)
		{
			// Positions
			float x = j * width / static_cast<float>(gridX) - halfWidth;
			float z = i * height / static_cast<float>(gridZ) - halfHeight;
			positions.push_back(x);
			positions.push_back(0.0f);
			positions.push_back(z);

			// Colors
			colors.push_back(static_cast<float>(j) / gridX);
			colors.push_back(0.0f);
			colors.push_back(static_cast<float>(i) / gridZ);

			// Normals
			normals.push_back(0.0f);
			normals.push_back(1.0f);
			normals.push_back(0.0f);

			// Tex Coords
			float s = static_cast<float>(j) / gridX;
			float t = static_cast<float>(i) / gridZ;
			texCoords.push_back(s);
			texCoords.push_back(t);
		}

	}
};


void Plane::generateIndices()
{
	for (unsigned int i = 0; i < gridZ; ++i) {
		for (unsigned int j = 0; j < gridX; ++j) {
			unsigned int topLeft = i * (gridX + 1) + j;
			unsigned int topRight = topLeft + 1;
			unsigned int bottomLeft = (i + 1) * (gridX + 1) + j;
			unsigned int bottomRight = bottomLeft + 1;

			// First triangle (top left, bottom left, top right)
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);

			// Second triangle (bottom left, bottom right, top right)
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
			indices.push_back(topRight);
		}
	}

};

void Plane::generateBuffers()
{
	// Code
		// Generate and bind VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Vertex positions
	glGenBuffers(1, &VBO_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_POSITION);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(GLfloat), positions.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(ASM_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(ASM_ATTRIBUTE_POSITION);

	// Vertex colors
	glGenBuffers(1, &VBO_COLOR);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_COLOR);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(ASM_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(ASM_ATTRIBUTE_COLOR);

	// Indices
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	// Unbind VAO
	glBindVertexArray(0);
};

Plane::~Plane()
{
	// Code
}

void Plane::init()
{
	// Code
	generatePlane();
	generateIndices();
	generateBuffers();

}
void Plane::bindAttributeLocationsAndLink(GLuint shaderProgramObject)
{
	// Code

	glBindAttribLocation(shaderProgramObject, ASM_ATTRIBUTE_POSITION, "a_position");
	glBindAttribLocation(shaderProgramObject, ASM_ATTRIBUTE_COLOR, "a_color");
	glLinkProgram(shaderProgramObject);
	shaderProgramObject = shaderProgramObject;

	// Post linking Uniforms
	uTime = glGetUniformLocation(shaderProgramObject, "uTime");
	uBigWavesElevation = glGetUniformLocation(shaderProgramObject, "uBigWavesElevation");
	uBigWavesFrequency = glGetUniformLocation(shaderProgramObject, "uBigWavesFrequency");
	uBigWavesSpeed = glGetUniformLocation(shaderProgramObject, "uBigWavesSpeed");
	uDepthColor = glGetUniformLocation(shaderProgramObject, "uDepthColor");
	uSurfaceColor = glGetUniformLocation(shaderProgramObject, "uSurfaceColor");
	uColorOffset = glGetUniformLocation(shaderProgramObject, "uColorOffset");
	uColorMultiplier = glGetUniformLocation(shaderProgramObject, "uColorMultiplier");
	uSmallWavesElevation = glGetUniformLocation(shaderProgramObject, "uSmallWavesElevation");
	uSmallWavesFrequency = glGetUniformLocation(shaderProgramObject, "uSmallWavesFrequency");
	uSmallWavesSpeed = glGetUniformLocation(shaderProgramObject, "uSmallWavesSpeed");
	uSmallWavesIterations = glGetUniformLocation(shaderProgramObject, "uSmallWavesIterations");
}

void Plane::uniforms()
{
	glUniform1f(uTime, elapsedTime);
	glUniform1f(uBigWavesElevation, bigWavesElevation);
	glUniform1f(uBigWavesSpeed, bigWavesSpeed);
	glUniform1f(uColorOffset, colorOffset);
	glUniform1f(uColorMultiplier, colorMultiplier);
	glUniform1f(uSmallWavesElevation, smallWavesElevation);
	glUniform1f(uSmallWavesFrequency, smallWavesFrequency);
	glUniform1f(uSmallWavesSpeed, smallWavesSpeed);
	glUniform1f(uSmallWavesIterations, smallWavesIterations);
	glUniform2fv(uBigWavesFrequency, 1, glm::value_ptr(bigWavesFrequency));
	glUniform3fv(uDepthColor, 1, glm::value_ptr(depthColor));
	glUniform3fv(uSurfaceColor, 1, glm::value_ptr(surfaceColor));
	
}


void Plane::render()
{
	// Code


	uniforms();
	glBindVertexArray(VAO);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Wireframe mode
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

void Plane::update()
{
	Logger log("UpdatePlane.log");
	// Code
	float deltaTime = timer.getElapsedTime();

	elapsedTime += deltaTime;
	log.debug("elapsedTime", elapsedTime);
}

void Plane::uninitializer()
{
	// Code
	
	if (VBO_COLOR)
	{
		glDeleteBuffers(1, &VBO_COLOR);
		VBO_COLOR = NULL;

	}
	if (VBO_POSITION)
	{
		glDeleteBuffers(1, &VBO_POSITION);
		VBO_POSITION = NULL;

	}
	if (EBO)
	{
		glDeleteBuffers(1, &EBO);
		EBO = NULL;

	}
	if (VAO)
	{
		glDeleteVertexArrays(1,&VAO);
		VAO = NULL;
	}

}







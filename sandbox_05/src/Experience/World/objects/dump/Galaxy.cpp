#include "Galaxy.h"
#include <cstdlib>
#include <ctime>
#include <random>

const GLfloat Galaxy::cubePosition[] = {
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

const GLfloat Galaxy::cubeColor[] = {
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

const GLuint Galaxy::indices[] =
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

Galaxy::Galaxy() :
	log("Galaxy.log"),
	angle(0.0f),
	count(1000000),
	radius(50.0f),
	positions(count * 3),
	colors(count*3),
	scales(count*1),
	randomness(count*3),
	localShaderPaths(
		{ 
			{GL_VERTEX_SHADER, "src/shader/galaxy/galaxy.vert"},
			{GL_FRAGMENT_SHADER,"src/shader/galaxy/galaxy.frag"}
		}
	)
{
	// Code
	generateGalaxy();
	setModelMatrix();
}

Galaxy::~Galaxy()
{
	uninitializer(); // Ensure resources are cleaned up
}

// Function similar to Math.random()
float Galaxy::randomFloat() {
	// Create a random number generator
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	std::mt19937 generator(std::random_device{}());
	return distribution(generator);
}



void Galaxy::renderGUI()
{

	// ImGui Frame preparation
	//ImGui::SetNextWindowBgAlpha(0.35f);
	//ImGui::DockSpaceOverViewport();

	// Other ImGui elements
	//ImGui::Begin("Properties");
	//if (ImGui::SliderFloat("Count", &count, 1000.0f, 1000000.0f, "%.10f")) {
	//	// Call your function directly if the value changed
	//	generateGalaxy();
	//}
	//
	//ImGui::SliderFloat("Size", &size, 20.0f, 100.0f, ".3f");
	
	//ImGui::End();

}


void Galaxy::generateGalaxy()
{
	log.write("Generate Galaxy");
	

	for (int i = 0; i < count; i++)
	{	
		// Generate a random position along the x-axis
		float _radius = randomFloat() * radius; // Random x position in the range [-radius, radius]
		float branchAngle = (i % 3) / 3.0f * PI * 2.0f;
		int i3 = i * 3;
		
		GLfloat randomX = glm::pow(randomFloat(), 3) * (randomFloat() < 0.5 ? 1 : -1) * 0.3 * _radius;
		GLfloat randomY = glm::pow(randomFloat(), 3) * (randomFloat() < 0.5 ? 1 : -1) * 0.3 * _radius;
		GLfloat randomZ = glm::pow(randomFloat(), 3) * (randomFloat() < 0.5 ? 1 : -1) * 0.3 * _radius;

		positions[i3    ] = glm::cos(branchAngle) * _radius; 
		positions[i3 + 1] = 0;						
		positions[i3 + 2] = glm::sin(branchAngle) * _radius;       
		
		randomness[i3    ] = randomX; 
		randomness[i3 + 1] = randomY;						
		randomness[i3 + 2] = randomZ;

		// Interpolation between white and blue based on radius
		Color startColor(255/ 255.0f, 96 / 255.0f, 48 / 255.0f); // White
		Color endColor(27 / 255.0f, 57 / 255.0f, 132 / 255.0f);   // Blue

		// Calculate interpolation factor based on radius
		float t = _radius / radius; // t varies between 0 (center) and 1 (edge)

		// Clamp to avoid overflows
		t = glm::clamp(t, 0.0f, 1.0f);

		// Interpolate between colors
		Color mixedColor = startColor.lerp(endColor, t);

		// Assign colors
		colors[i3] = mixedColor.r;
		colors[i3 + 1] = mixedColor.g;
		colors[i3 + 2] = mixedColor.b;

		scales[i] = randomFloat();

	}

	for (int i = 0; i < count; i++)
	{

		log.debug("scales[i] ", scales[i]);
		
	}
	time = 1.0f;

}


void Galaxy::update()
{
	// Code
	if (time > 0.0f)
	{

	float deltaTime = timer.getElapsedTime();
	time  += deltaTime * 2.0f;
	}
	angle += 0.01f;
	updateModelMatrix();
}


void Galaxy::setModelMatrix()
{
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
	log.write("1: modelmatrix");
}


GLuint Galaxy::initializeShaders()
{
	// Code

	Shader shader;

	// Initialize shader with the paths from localShaderPaths

	shader.initialize(this, localShaderPaths);
	shaderProgramObject = shader.getShaderProgram(shader.shaderName);
	log.debug("initializeShaders :shaderProgramObject", shaderProgramObject);

	return shaderProgramObject;
}

GLuint Galaxy::getShaderProgramObject() const
{
	return shaderProgramObject;
}

glm::mat4 Galaxy::getModelMatrix() const
{
	return modelMatrix;
}

void Galaxy::init() {

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Bind with vertex array object and start recording buffer and buffer data related steps.
	glGenBuffers(1, &VBO_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_POSITION);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(GLfloat), positions.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(ASM_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(ASM_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Bind with vertex array object and start recording buffer and buffer data related steps.
	glGenBuffers(1, &VBO_RANDOM);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_RANDOM);
	glBufferData(GL_ARRAY_BUFFER, randomness.size() * sizeof(GLfloat), randomness.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(ASM_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(ASM_ATTRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// VBO for Color
	glGenBuffers(1, &VBO_COLOR);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_COLOR);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(ASM_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(ASM_ATTRIBUTE_COLOR);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &VBO_SCALES);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_SCALES);
	glBufferData(GL_ARRAY_BUFFER, scales.size() * sizeof(GLfloat), scales.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 0, nullptr); // Stride is 0 since scales are tightly packed
	glEnableVertexAttribArray(5);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

}


void Galaxy::bindAttributeLocationsAndLink(GLuint shaderProgramObject) {
	glBindAttribLocation(shaderProgramObject, ASM_ATTRIBUTE_POSITION, "a_position");
	glBindAttribLocation(shaderProgramObject, ASM_ATTRIBUTE_COLOR, "a_color");
	glBindAttribLocation(shaderProgramObject, ASM_ATTRIBUTE_NORMAL, "a_randomness");
	glBindAttribLocation(shaderProgramObject, 5, "a_scales");
	glLinkProgram(shaderProgramObject);

	// Post linking Uniforms
	uTime = glGetUniformLocation(shaderProgramObject, "uTime");
}

void Galaxy::updateModelMatrix() {
	modelMatrix = glm::mat4(1.0); // Reset to identity matrix
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));

	//modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
	//modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Galaxy::render()
{
	//uniform
	glUniform1f(uTime, time);
	//code
	//Bind with vertex array object

	// Additive blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	glDepthMask(GL_FALSE);

	timer.reset();

	glBindVertexArray(VAO);

	glDrawArrays(GL_POINTS, 0, count);
	
	glBindVertexArray(0);

	// Re-enable depth buffer writing
	glDepthMask(GL_TRUE);

	glDisable(GL_BLEND);
}

void Galaxy::uninitializer() {

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


#include "mesh.h"

#include "utils/Logger.h"
#include "../../../utils/Time.h"


class Plane : public Mesh
{

public:

	Plane(unsigned int width = 1, unsigned int height = 1, unsigned int gridX = 1, unsigned int gridZ = 1 );
   ~Plane();
	
	std::unordered_map<GLenum,const char*> localShaderPaths;

	void init() override;
	void bindAttributeLocationsAndLink(GLuint) override;
	void render() override;
	void renderGUI() override;
	void uninitializer() override;
	void update() override;
	void setModelMatrix() override;
	
	GLuint initializeShaders() override;
	glm::mat4 getModelMatrix() const override;
	GLuint getShaderProgramObject() const override;
	void updateModelMatrix();
	// Uniforms
	glm::mat4 modelMatrix;

	GLuint uBigWavesElevation;
	GLuint uBigWavesFrequency;
	GLuint uTime;
	GLuint uBigWavesSpeed;
	GLuint uDepthColor;
	GLuint uSurfaceColor;
	GLuint uColorOffset;
	GLuint uColorMultiplier;
	GLuint uSmallWavesElevation;
	GLuint uSmallWavesFrequency;
	GLuint uSmallWavesSpeed;
	GLuint uSmallWavesIterations;
	
	GLfloat bigWavesElevation = 0.2;
	GLfloat elapsedTime = 0;
	GLfloat bigWavesSpeed = 1.3;
	GLfloat colorOffset = 0.5;
	GLfloat colorMultiplier = 2.2;
	GLfloat smallWavesElevation = 0.18;
	GLfloat smallWavesFrequency = 1.5;
	GLfloat smallWavesSpeed = 1.15;
	GLfloat smallWavesIterations = 4.0;
	glm::vec2 bigWavesFrequency = glm::vec2(1.5f, 0.5f);
	glm::vec3 depthColor = glm::vec3(0 / 255.0f, 51 / 255.0f, 102 / 255.0f);
	glm::vec3 surfaceColor = glm::vec3(102 / 255.0f, 178 / 255.0f, 255 / 250);

private:
	
	unsigned int width, height, gridX, gridZ;

	GLuint shaderProgramObject = 0;

	GLuint VAO = 0;
	GLuint VBO_POSITION = 0;
	GLuint VBO_COLOR = 0;
	GLuint EBO = 0;

	Timer timer;

	void generatePlane();
	void generateIndices();
	void generateBuffers();
	void setUniforms();

	std::vector<GLfloat> positions;
	std::vector<GLfloat> colors;
	std::vector<GLfloat> normals; // normals
	std::vector<GLfloat> texCoords;
	std::vector<GLuint> indices;

	/*static const GLfloat planePositions[];
	static const GLfloat planeColors[];
	static const GLuint planeIndices[];*/
};
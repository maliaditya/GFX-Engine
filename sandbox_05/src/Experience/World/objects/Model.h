#pragma once
#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>
#include <sstream> // Include for string stream
#include "../../../utils/Time.h"

class Model : public Mesh
{
public:
    Model();
    ~Model() override;

    std::unordered_map<GLenum, const char*> localShaderPaths;
    Logger log;
    void init() override;
    void render() override;
    void renderGUI() override;
    void bindAttributeLocationsAndLink(GLuint) override;
    void uninitializer() override;
    void update() override;
    void setModelMatrix() override;
    GLuint initializeShaders() override;
    GLuint getShaderProgramObject() const override;
    glm::mat4 getModelMatrix() const override;
    glm::mat4 modelMatrix;
    void updateModelMatrix();

    struct MeshData 
    {
        std::string name;
        std::vector<glm::vec3> vertexPositions;
        std::vector<unsigned int> indices;
    };

private:
    GLfloat angle;
    GLuint shaderProgramObject = 0;
    GLuint VAO = 0;
    GLuint VBO_POSITION[3];
    GLuint VBO_COLOR = 0;
    GLuint EBO = 0;

    Timer timer;
    GLfloat time;
    GLuint uTime;


    const aiScene* scene;
    aiMesh* mesh;
    bool pos;
    bool waiting = false;
    int waitCounter = 0;
    const int WAIT_LIMIT = 1500;
    std::vector<GLfloat> positions;
    glm::vec3 colora = glm::vec3(57 / 255.0f, 255 / 255.0f, 20 / 255.0f);
    glm::vec3 colorb = glm::vec3(0 / 255.0f, 188 / 255.0f, 255 /255.0f);
    GLuint colorA;
    GLuint colorB;

    std::vector<MeshData> meshVertexPositions;
    std::vector<unsigned int> indices;
    std::string directory;
    void loadModel(std::string const& path);

    GLuint getVAO() const { return VAO; };
};

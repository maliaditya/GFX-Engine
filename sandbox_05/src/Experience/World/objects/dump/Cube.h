#include "mesh.h"

class Cube : public Mesh
{
public:
    Cube();
    ~Cube() override;

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

private:
    GLfloat angle;
    GLuint shaderProgramObject = 0;
    GLuint VAO = 0;
    GLuint VBO_POSITION = 0;
    GLuint VBO_COLOR = 0;
    GLuint EBO = 0;

    static const GLfloat cubePosition[];
    static const GLfloat cubeColor[];
    static const GLuint indices[];

    GLuint getVAO() const { return VAO; };
};

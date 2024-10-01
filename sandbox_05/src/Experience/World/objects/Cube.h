#include "mesh.h"

class Cube : public Mesh
{
public:
    Cube();
    ~Cube() override;

    std::unordered_map<GLenum, const char*> localShaderPaths;

    void init() override;
    void uninitializer() override;
    void bindAttributeLocationsAndLink(GLuint programID) override;
    void render() override;


private:
    GLuint VAO = 0;
    GLuint VBO_POSITION = 0;
    GLuint VBO_COLOR = 0;
    GLuint EBO = 0;

    static const GLfloat cubePosition[];
    static const GLfloat cubeColor[];
    static const GLuint indices[];

    GLuint getVAO() const { return VAO; };
};

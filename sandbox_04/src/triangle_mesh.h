#include "shader.h"
#include "mesh.h"

class TriangleMesh : public Mesh
{
public:
    TriangleMesh();
    ~TriangleMesh() override;

    GLuint init() override;
    void uninitializer() override; 
    void bindAttributeLocationsAndLink(GLuint programID) override;
    void Objects() override;
    void render() override;
    
    std::unordered_map<GLenum, const char*> localShaderPaths;


    GLuint VAO = 0;
    GLuint VBO_POSITION = 0;
    GLuint VBO_COLOR = 0;

    static const GLfloat trianglPosition[];
    static const GLfloat trianglColor[];
    GLuint getVAO() const { return VAO; };
    Buffer buffer;
};

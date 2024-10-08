#include "mesh.h"

class TriangleMesh : public Mesh
{
public:
    TriangleMesh();
    ~TriangleMesh() override;

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
    Buffer buffer;

    static const GLfloat trianglPosition[];
    static const GLfloat trianglColor[]; 
    static const GLuint indices[];
    
    GLuint getVAO() const { return VAO; };
};

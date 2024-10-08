#include "mesh.h"
#include "../../../utils/Time.h"

// Define Color struct
struct Color {
    float r, g, b;

    Color(float r = 1.0f, float g = 1.0f, float b = 1.0f)
        : r(r), g(g), b(b) {}

    Color lerp(const Color& other, float t) const {
        t = glm::clamp(t, 0.0f, 1.0f); // Clamp t to [0, 1]
        return Color(
            glm::clamp(r + (other.r - r) * t, 0.0f, 1.0f),
            glm::clamp(g + (other.g - g) * t, 0.0f, 1.0f),
            glm::clamp(b + (other.b - b) * t, 0.0f, 1.0f)
        );
    }
};


class Galaxy : public Mesh
{
public:
    Galaxy();
    ~Galaxy() override;

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
    // Uniforms
    GLfloat size;



    GLfloat angle;
    GLuint shaderProgramObject = 0;
    GLuint VAO = 0;
    GLuint VBO_POSITION = 0;
    GLuint VBO_RANDOM = 0;
    GLuint VBO_COLOR = 0;
    GLuint VBO_SCALES = 0;
    GLuint EBO = 0;

    GLfloat count = 0;
    GLfloat radius = 0;

    Timer timer;;
    GLuint uTime = 0;
    GLfloat time = 0;

    static const GLfloat cubePosition[];
    static const GLfloat cubeColor[];
    static const GLuint indices[];
    std::vector<GLfloat> positions;
    std::vector<GLfloat> randomness;
    std::vector<GLfloat> colors;
    std::vector<GLfloat> scales;
    void generateGalaxy();
    float randomFloat();
    GLuint getVAO() const { return VAO; };
};

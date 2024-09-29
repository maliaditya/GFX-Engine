#include "Mesh.h"

class Particles : public Mesh
{
	// Code
public:

	void init() override;
	void render() override;
	void bindAttributeLocationsAndLink(GLuint) override;
	void uninitialize() override;

private:

	GLuint VAO = 0;
	GLuint VBO_POSITION = 0;


};
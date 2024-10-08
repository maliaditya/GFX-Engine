#include "mesh.h"

class Object :public Mesh
{
public:
	Object(GLfloat* vertices, GLfloat* color,GLuint* indices);
	~object() override;

	void init() override;
	void bindAttributeLocationsAndLink(Gluint);
	void render() override;
	void uninitializer();

private:
	GLuint VAO;
	GLuint VBO_POSITION;
	GLuint VBO_COLOR;
	GLuint EBO;
	
	GLfloat* vertices;
	GLfloat* color;
	GLuint* indices;

	GLuint getVAO() const { return VAO };

};
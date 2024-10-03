#pragma once
#include "buffer.h"
#include <GL/glew.h> // This Must Be Before Including <GL/gl.h>
#include <GL/gl.h>

#include <unordered_map>
#include <string>



const float PI = 3.14159265358979323846f;

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//	
//using namespace glm;

enum
{
	ASM_ATTRIBUTE_POSITION = 0,
	ASM_ATTRIBUTE_COLOR,
	ASM_ATTRIBUTE_NORMAL,
	ASM_ATTRIBUTE_TEXTURE0

};

class Mesh {
public:
	virtual ~Mesh() = default;
	virtual void init() = 0;           // Pure virtual method
	virtual void render() = 0;   // Pure virtual method
	virtual void renderGUI() = 0;
	virtual void bindAttributeLocationsAndLink(GLuint) = 0;
	virtual void uninitializer() = 0;   // Pure virtual method


};

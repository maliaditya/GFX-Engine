
#include <GL/glew.h> // This Must Be Before Including <GL/gl.h>
#include <GL/gl.h>
#include <stdlib.h>

class Buffer {
public:


    void createBuffer(GLuint &bufferID, GLenum bufferType, const void* data, GLsizei dataSize, GLenum usage, GLuint attributeIndex = GLuint(-1), GLint componentCount = 0, GLenum componentType = GL_FLOAT, GLboolean normalized = GL_FALSE, GLsizei stride = 0, const void* offset = NULL){
        // Generate a buffer and bind it
        glGenBuffers(1, &bufferID);
        glBindBuffer(bufferType, bufferID);
        
        // Upload the data to the buffer
        glBufferData(bufferType, dataSize, data, usage);
        
        // Specify the layout of the buffer's data
        if (attributeIndex != GLuint(-1)) { // Check if attributeIndex is valid
            glVertexAttribPointer(attributeIndex, componentCount, componentType, normalized, stride, offset);
            glEnableVertexAttribArray(attributeIndex);
        }
        
        // Unbind the buffer
        glBindBuffer(bufferType, 0);
    }



};
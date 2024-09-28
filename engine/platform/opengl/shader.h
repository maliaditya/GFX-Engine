#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string>
#include "../../engine/utils/Logger.h"
#include "mesh.h"


    class Shader {
    private:
        GLuint shaderProgramObject;
    
    	FILE* file;
        Logger logger;
    
    public:
        Shader() : logger("shader.log") {
            shaderProgramObject = glCreateProgram();
        }
    
        static std::unordered_map<std::string, GLuint> shaderProgramObjectMap;
    
        ~Shader();
    
        GLuint initialize(Mesh* m, const std::unordered_map<GLenum, const char*>& localShaderPaths);
        GLuint getObject();
        char* readShader(const char* fileName);
        void unitializeShader();
        void createAndCompileShader(const GLchar* shaderString, GLenum shaderType, const char* fileName);
        void shaderObjectErrorChecking();
        void getAllUniformLocations(const std::string& filename);
        std::string extractShaderName(const std::string& path);
        static GLuint getShaderProgram(const std::string& shaderName);
        static const std::unordered_map<std::string, GLuint>& getAllShaderPrograms();
    };


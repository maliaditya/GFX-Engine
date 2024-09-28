#include "shader.h"

std::unordered_map<std::string, GLuint> Shader::shaderProgramObjectMap;

Shader::~Shader() {
    // Cleanup resources if necessary
}

GLuint Shader::initialize(Mesh* mesh, const std::unordered_map<GLenum, const char*>& localShaderPaths) {

	if (shaderProgramObjectMap.empty()) {
		shaderProgramObjectMap.clear();
	}


	for (const auto& shader : localShaderPaths) {

		std::string shaderName = extractShaderName(shader.second);
		logger.debug("Shader Name: ", shaderName);

		logger.debug("shader.first", shader.first);
		logger.debug("shader.second", shader.second);

		createAndCompileShader(readShader(shader.second), shader.first, shader.second);

		if (shaderProgramObjectMap.find(shaderName) == shaderProgramObjectMap.end()) {
			shaderProgramObjectMap[shaderName] = shaderProgramObject;
		}

    }
	mesh->bindAttributeLocationsAndLink(shaderProgramObject);
	shaderObjectErrorChecking();
	mesh->init();

	

	return shaderProgramObject;
}


std::string  Shader::extractShaderName(const std::string& path) {
	// Find the last occurrence of '/' or '\\' in the path
	size_t lastSlash = path.find_last_of("/\\");

	// Find the last occurrence of '.' in the path after the last '/'
	size_t lastDot = path.find_last_of('.');

	// Extract the name between the last '/' and the last '.'
	if (lastSlash != std::string::npos && lastDot != std::string::npos && lastDot > lastSlash) {
		return path.substr(lastSlash + 1, lastDot - lastSlash - 1);
	}

	// If the path does not have '/' or '.' or if '.' is before '/'
	return path;
}

GLuint Shader::getObject() {
    return shaderProgramObject;
}



GLuint Shader::getShaderProgram(const std::string& shaderName) {
	auto it = shaderProgramObjectMap.find(shaderName);
	if (it != shaderProgramObjectMap.end()) {
		return it->second;
	}
	return 0; // or some invalid value
}

const std::unordered_map<std::string, GLuint>& Shader::getAllShaderPrograms() {
	return shaderProgramObjectMap;
}


char* Shader::readShader(const char* fileName) {
	    FILE *fp;
		long size = 0;
		char* shaderContent;
		
		fp = fopen(fileName, "rb");
		if(fp == NULL) {
			return "";
		}
		fseek(fp, 0L, SEEK_END);
		size = ftell(fp)+1;
		fclose(fp);

		fp = fopen(fileName, "r");
		shaderContent =  (char*)memset(malloc(size), '\0', size);
		fread(shaderContent, 1, size-1, fp);
		fclose(fp);

		return shaderContent;
}

void Shader::unitializeShader() {
	fclose(file);
    // Shader unintialization code
    if (shaderProgramObject)
		{

			glUseProgram(shaderProgramObject);

			GLsizei numAttachedShaders;
			glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &numAttachedShaders);

			GLuint* shaderObjects = NULL;
			shaderObjects = (GLuint*)malloc(numAttachedShaders * sizeof(GLuint));

			// filling this empty buffer 
			glGetAttachedShaders(shaderProgramObject, numAttachedShaders, &numAttachedShaders, shaderObjects);
			for (GLsizei i = 0; i < numAttachedShaders; i++)
			{
				glDetachShader(shaderProgramObject, shaderObjects[i]);
				glDeleteShader(shaderObjects[i]);
				shaderObjects[i] = 0;
			}

			free(shaderObjects);
			shaderObjects = NULL;
			glUseProgram(0);
			glDeleteProgram(shaderProgramObject);
			shaderProgramObject = 0;
		}
}

void Shader::createAndCompileShader(const GLchar* shaderString, GLenum shaderType, const char* fileName) {
            GLint status;
			GLint infoLogLength;
			char* _log = NULL;
			GLuint shaderObject = glCreateShader(shaderType);
			if (shaderObject ==0)
			{
                
				logger.debug("Error creating shader type", shaderType);
				exit(0);
			}
			glShaderSource(shaderObject, 1, (const GLchar**)&shaderString, NULL);
			glCompileShader(shaderObject);
			glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE)
			{
				glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
				if (infoLogLength > 0)
				{
					_log = (char*)malloc(infoLogLength);
					if (_log != NULL)
					{
						GLsizei written;
						glGetShaderInfoLog(shaderObject, infoLogLength, &written, _log);
						logger.debug(  "\nShader", fileName);
						logger.debug(  "Compilation log:\n", _log);
						free(_log);
						unitializeShader();
					}
				}
			}
			glAttachShader(shaderProgramObject, shaderObject);
}

void Shader::shaderObjectErrorChecking() {
    // Error checking for shader program
    	GLint status;
		GLint infoLogLength;
		char* log = NULL;


		glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
			if (infoLogLength > 0)
			{
				log = (char*)malloc(infoLogLength);
				if (log != NULL)
				{
					GLsizei written;
					glGetProgramInfoLog(shaderProgramObject, infoLogLength, &written, log);
					logger.debug("Shader Program Link log: ", log);
					free(log);
					unitializeShader();

				}
			}
		}
}

void Shader::getAllUniformLocations(const std::string& filename) {
    // Code to get uniform locations and write to file
		std::ofstream outputFile("Uniform.log");

		if (!outputFile.is_open()) {

			return;
		}
		GLint numUniforms = 0;
		glGetProgramiv(shaderProgramObject, GL_ACTIVE_UNIFORMS, &numUniforms);

		std::vector<GLchar> nameData(256);
		GLsizei length = 0;
		GLint size = 0;
		GLenum type = 0;

		for (int i = 0; i < numUniforms; ++i) {
			glGetActiveUniform(shaderProgramObject, i, nameData.size(), &length, &size, &type, &nameData[0]);
			std::string name((char*)&nameData[0], length);

			GLint location = glGetUniformLocation(shaderProgramObject, name.c_str());
			outputFile << "Uniform: " << name << ", Location: " << location << std::endl;
		}

		outputFile.close();
	}


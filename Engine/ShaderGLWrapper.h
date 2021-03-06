#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "EngineException.h"

class ShaderManager;

class ShaderGLWrapper
{
private:
	enum ShaderType
	{
		VERTEX_SHADER,
		FRAGMENT_SHADER,

		NUM_SHADERS
	};

	GLuint mProgram;
	GLuint mShaders[NUM_SHADERS];
	std::string mFileName;
	ShaderManager &mShaderManager;

public:
	ENGINE_API ShaderGLWrapper(const std::string& fileName, ShaderManager &shaderManager);

	ENGINE_API void Bind();

	ENGINE_API virtual ~ShaderGLWrapper();

	void SetModelViewProjectionMatrixUniform(glm::mat4 mvp);
	GLuint GetUniformLocation(std::string name);

private:
	void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string &errorMesage);
	std::string loadShader(const std::string &fileName);
	GLuint createShader(const std::string &fullText, GLenum shaderType);
};

class ShaderException : public EngineException
{
public:
	ShaderException(std::string fileName, std::string errorMsg);
};
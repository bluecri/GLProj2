#include "../../stdafx.h"
#include "ShaderObj.h"

SHADER::ShaderObj::ShaderObj(const char * vertexShader, const char * fragmentShader)
	: _keyStr(vertexShader)
{
	m_shaderID = LoadShaders(vertexShader, fragmentShader);
	_keyStr.append(vertexShader);
}

SHADER::ShaderObj::~ShaderObj() { glDeleteProgram(m_shaderID); }

void SHADER::ShaderObj::bind() { glUseProgram(m_shaderID); }

void SHADER::ShaderObj::unbind() { glUseProgram(0); }

GLuint SHADER::ShaderObj::getShaderID() { return m_shaderID; }

void SHADER::ShaderObj::loadInt(GLuint location, int value) { glUniform1i(location, value); }

void SHADER::ShaderObj::loadFloat(GLuint location, float value) { glUniform1f(location, value); }

void SHADER::ShaderObj::loadVector2(GLuint location, const glm::vec2 & vector)
{
	glUniform2f(location, vector.x, vector.y);
}

void SHADER::ShaderObj::loadVector3(GLuint location, const glm::vec3 & vector)
{
	glUniform3f(location, vector.x, vector.y, vector.z);
}

void SHADER::ShaderObj::loadMatrix4(GLuint location, const glm::mat4 & matrix)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, (GLfloat*)&matrix);
}

std::string & SHADER::ShaderObj::getKeyStr()
{
	return _keyStr;
}

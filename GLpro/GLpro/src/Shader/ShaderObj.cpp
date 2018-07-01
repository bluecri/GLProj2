#include "../../stdafx.h"
#include "ShaderObj.h"

SHADER::ShaderObj::ShaderObj(const char * vertexShader, const char * fragmentShader)
	: _keyStr(vertexShader)
{
	m_shaderID = LoadShaders(vertexShader, fragmentShader);
	_keyStr.append(fragmentShader);
}

SHADER::ShaderObj::~ShaderObj() { glDeleteProgram(m_shaderID); }

void SHADER::ShaderObj::bind() { glUseProgram(m_shaderID); }

void SHADER::ShaderObj::unbind() { glUseProgram(0); }

GLuint SHADER::ShaderObj::getShaderID() { return m_shaderID; }

void SHADER::ShaderObj::loadInt(GLuint location, int value) { glUniform1i(location, value); }

void SHADER::ShaderObj::loadVector2i(GLuint location, const int& int1, const int& int2)
{
	glUniform2i(location, int1, int2);
}

void SHADER::ShaderObj::loadFloat(GLuint location, float value) { glUniform1f(location, value); }

void SHADER::ShaderObj::loadVector2(GLuint location, const glm::vec2 & vector)
{
	glUniform2f(location, vector.x, vector.y);
}

void SHADER::ShaderObj::loadVector3(GLuint location, const glm::vec3 & vector)
{
	glUniform3f(location, vector.x, vector.y, vector.z);
}

void SHADER::ShaderObj::loadVector3f(GLuint location, const glm::vec3 & vector)
{
	glUniform3i(location, vector.x, vector.y, vector.z);
}


void SHADER::ShaderObj::loadVector3(GLuint location, const float& x, const float& y, const float& z)
{
	glUniform3f(location, x, y, z);
}

void SHADER::ShaderObj::loadMatrix4(GLuint location, const glm::mat4 & matrix, int count)
{
	glUniformMatrix4fv(location, count, GL_FALSE, (GLfloat*)&matrix);
}

void SHADER::ShaderObj::uniformBlockBind(GLuint location, int uniformIndex)
{
	glUniformBlockBinding(m_shaderID, location, uniformIndex);
}

std::string & SHADER::ShaderObj::getKeyStr()
{
	return _keyStr;
}

#include "ShaderShadow.h"

SHADER::ShaderShadow::ShaderShadow(const char * vertexShader, const char * fragmentShader)
	: ShaderObj(vertexShader, fragmentShader)
{
	if (m_shaderID != -1)
	{
		MMatrixID = glGetUniformLocation(m_shaderID, _NameM);
		VPMatrixID = glGetUniformLocation(m_shaderID, _NameVP);
	}
}

#include "ShaderFXAA.h"

SHADER::ShaderFXAA::ShaderFXAA(const char * vertexShader, const char * fragmentShader)
	: ShaderObj(vertexShader, fragmentShader)
{
	if (m_shaderID != -1)
	{
		m_screenTex = glGetUniformLocation(m_shaderID, "TextrueScreen");

		m_lumaThreshold = glGetUniformLocation(m_shaderID, "LumaThreshold");
		m_mulReduce = glGetUniformLocation(m_shaderID, "MulReduce");
		m_minReduce = glGetUniformLocation(m_shaderID, "MinReduce");
		m_maxSpan = glGetUniformLocation(m_shaderID, "MaxSpan");

		m_texelStep = glGetUniformLocation(m_shaderID, "TexelStep");
	}
}


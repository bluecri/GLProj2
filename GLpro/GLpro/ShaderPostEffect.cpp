#include "ShaderPostEffect.h"

SHADER::ShaderPostEffect::ShaderPostEffect(const char * vertexShader, const char * fragmentShader)
	: ShaderObj(vertexShader, fragmentShader)
{
	if (m_shaderID != -1)
	{
		//m_screenTex = glGetUniformLocation(m_shaderID, "TextrueScreen");
	}
}

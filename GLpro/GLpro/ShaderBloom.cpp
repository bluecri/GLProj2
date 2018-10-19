#include "ShaderBloom.h"

SHADER::ShaderBloom::ShaderBloom(const char * vertexShader, const char * fragmentShader)
	: ShaderObj(vertexShader, fragmentShader)
{
	if (m_shaderID != -1)
	{
		m_texBloom = glGetUniformLocation(m_shaderID, "TextureBloom");
		m_horizontal = glGetUniformLocation(m_shaderID, "BHorizontal");
	}
}

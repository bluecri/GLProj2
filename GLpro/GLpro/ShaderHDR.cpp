#include "ShaderHDR.h"

SHADER::ShaderHDR::ShaderHDR(const char * vertexShader, const char * fragmentShader)
	: ShaderObj(vertexShader, fragmentShader)
{
	if (m_shaderID != -1)
	{
		m_texGFBO = glGetUniformLocation(m_shaderID, "TextrueGFBO");
		m_texBloom = glGetUniformLocation(m_shaderID, "TextureBloom");
		m_useBloom = glGetUniformLocation(m_shaderID, "UseBloom");
		m_useToneMapping = glGetUniformLocation(m_shaderID, "UseToneMapping");

		m_exposure = glGetUniformLocation(m_shaderID, "Exposure");
		m_mipmapLevel = glGetUniformLocation(m_shaderID, "MipmapLevel");
	}
}

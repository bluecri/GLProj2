#include "ShaderMipmap.h"

SHADER::ShaderMipmap::ShaderMipmap(const char * vertexShader, const char * fragmentShader)
	: ShaderObj(vertexShader, fragmentShader)
{
	if (m_shaderID != -1)
	{
		m_texture = glGetUniformLocation(m_shaderID, "MipmapTextrue");
		m_mipmapLevel = glGetUniformLocation(m_shaderID, "MipmapLevel");
	}
}
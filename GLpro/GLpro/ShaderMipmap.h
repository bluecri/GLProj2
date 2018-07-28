#pragma once
#include "./src/Shader/ShaderObj.h"

namespace SHADER
{
	class ShaderMipmap : public ShaderObj
	{
	public:
		ShaderMipmap(const char * vertexShader, const char * fragmentShader);

		GLuint m_texture;
		GLuint m_mipmapLevel;
	};
}

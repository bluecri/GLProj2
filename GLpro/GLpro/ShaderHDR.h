#pragma once
#include "./src/Shader/ShaderObj.h"

namespace SHADER
{
	class ShaderHDR : public ShaderObj
	{
	public:
		ShaderHDR(const char * vertexShader, const char * fragmentShader);

		GLuint m_texGFBO;
		GLuint m_texBloom;

		GLuint m_useBloom;
		GLuint m_useToneMapping;
		

		GLuint m_mipmapLevel;
		GLuint m_exposure;
	};
}

#pragma once
#include "./src/Shader/ShaderObj.h"

namespace SHADER
{
	class ShaderBloom : public ShaderObj
	{
	public:
		ShaderBloom(const char * vertexShader, const char * fragmentShader);

		GLuint m_texBloom;
		GLuint m_horizontal;
	};
}

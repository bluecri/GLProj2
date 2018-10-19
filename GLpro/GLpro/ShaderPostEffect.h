#pragma once
#include "./src/Shader/ShaderObj.h"

namespace SHADER
{
	class ShaderPostEffect : public ShaderObj
	{
	public:
		ShaderPostEffect(const char * vertexShader, const char * fragmentShader);

		//GLuint m_screenTex;
	};
}

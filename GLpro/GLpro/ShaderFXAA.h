#pragma once
#include "./src/Shader/ShaderObj.h"

namespace SHADER
{
	class ShaderFXAA : public ShaderObj
	{
	public:
		ShaderFXAA(const char * vertexShader, const char * fragmentShader);

		GLuint m_screenTex;

		GLuint m_lumaThreshold;
		GLuint m_mulReduce;
		GLuint m_minReduce;
		GLuint m_maxSpan;

		GLuint m_texelStep;
	};
}

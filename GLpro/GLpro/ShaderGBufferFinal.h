#pragma once

#include "./src/Shader/ShaderObj.h"

namespace SHADER
{
	// final pass
	class ShaderGBufferFinal : public ShaderObj
	{
	public:
		ShaderGBufferFinal(const char * vertexShader, const char * fragmentShader);
	};
}

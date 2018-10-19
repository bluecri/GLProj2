#pragma once

#include "./src/Shader/ShaderObj.h"

namespace SHADER
{
	// create GBuffer
	class ShaderGBufferGeo : public ShaderObj
	{
	public:
		ShaderGBufferGeo(const char * vertexShader, const char * fragmentShader);

		GLuint m_cameraVPMatrixID;
		GLuint m_modelMatrixID;
		GLuint m_textureID;
		GLuint m_bloomValueID;
	};
}

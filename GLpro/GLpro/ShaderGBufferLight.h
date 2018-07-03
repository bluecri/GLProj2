#pragma once

#include "./src/Shader/ShaderObj.h"

namespace SHADER
{
	// pass light & shadow
	class ShaderGBufferLight : public ShaderObj
	{
	public:
		ShaderGBufferLight(const char * vertexShader, const char * fragmentShader);

		GLuint m_texWorld;
		GLuint m_texColor;
		GLuint m_texNormal;

		GLuint m_texShadow;

		GLuint m_cameraViewMat;
		GLuint m_cameraVPMat;

		GLuint m_directionalLIghtBlockID;
		GLuint m_spotLIghtBlockID;
		GLuint m_pointLIghtBlockID;
		GLuint m_deferredPointLIghtBlockID;
	};
}

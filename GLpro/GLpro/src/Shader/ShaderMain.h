#pragma once
#include "./ShaderObj.h"
#include "../../configs_ubo.h"

namespace SHADER
{
	class ShaderMain : public ShaderObj 
	{
	public:
		ShaderMain(const char * vertexShader, const char * fragmentShader);

		GLuint m_cameraVPMatrixID;
		GLuint m_cameraViewMatrixID;
		GLuint m_modelMatrixID;
		GLuint m_shadowMapID;
		GLuint m_textureID;

		GLuint m_directionalLIghtBlockID;

		GLuint m_spotLIghtBlockID;
		GLuint m_pointLIghtBlockID;
		GLuint m_deferredPointLIghtBlockID;

	};
}

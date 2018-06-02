#pragma once
#include "./ShaderObj.h"

namespace SHADER
{
	class ShaderParticle : public ShaderObj
	{
	public:
		ShaderParticle(const char * vertexShader, const char * fragmentShader)
			: ShaderObj(vertexShader, fragmentShader) 
		{
			if (m_shaderID != -1) {
				m_textureID = glGetUniformLocation(m_shaderID, _NameTextureSampler);

				m_cameraRight_worldspace_ID = glGetUniformLocation(m_shaderID, _NameCamRight);
				m_cameraUp_worldspace_ID = glGetUniformLocation(m_shaderID, _NameCamUp);
				m_viewProjMatrixID = glGetUniformLocation(m_shaderID, _NameVP);

			}
		}

		GLuint m_textureID;
		GLuint m_cameraRight_worldspace_ID;
		GLuint m_cameraUp_worldspace_ID;
		GLuint m_viewProjMatrixID;

	private:
		const char * _NameTextureSampler = "myTextureSampler";
		const char * _NameCamRight = "CameraRight_worldspace";
		const char * _NameCamUp = "CameraUp_worldspace";
		const char * _NameVP = "VP";
	};
}

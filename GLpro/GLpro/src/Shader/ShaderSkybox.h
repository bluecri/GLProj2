#pragma once
#include "./ShaderObj.h"

namespace SHADER
{
	class ShaderSkybox : public ShaderObj
	{
	public:
		ShaderSkybox(const char * vertexShader, const char * fragmentShader)
			: ShaderObj(vertexShader, fragmentShader)
		{
			if (m_shaderID != -1)
			{
				m_textureID = glGetUniformLocation(m_shaderID, _NameTextureSampler);
				m_MVPMatrixID = glGetUniformLocation(m_shaderID, _NameMVP);
				m_cameraViewMatrixID = glGetUniformLocation(m_shaderID, _NameView);
				m_modelMatrixID = glGetUniformLocation(m_shaderID, _NameM);
			}
		}

		GLuint m_textureID;
		GLuint m_MVPMatrixID;
		GLuint m_cameraViewMatrixID;
		GLuint m_modelMatrixID;

	private:
		const char * _NameTextureSampler = "myTextureSampler";
		const char * _NameMVP = "MVP";
		const char * _NameView = "V";
		const char * _NameM = "M";
	};
}

#include "./ShaderObj.h"
namespace SHADER
{
	class ShaderMain : public ShaderObj 
	{
	public:
		ShaderMain(const char * vertexShader, const char * fragmentShader) : ShaderObj(vertexShader, fragmentShader)
		{
			if (m_shaderID != -1)
			{
				m_MVPMatrixID = glGetUniformLocation(m_shaderID, "MVP");
				m_cameraViewMatrixID = glGetUniformLocation(m_shaderID, "V");
				m_modelMatrixID = glGetUniformLocation(m_shaderID, "M");
				m_depthBiasID = glGetUniformLocation(m_shaderID, "DepthBiasMVP");
				m_shadowMapID = glGetUniformLocation(m_shaderID, "shadowMap");
				m_lightInvDirID = glGetUniformLocation(m_shaderID, "LightInvDirection_worldspace");
				m_textureID = glGetUniformLocation(m_shaderID, "myTextureSampler");
			}
		}
	};
}

#include "ShaderMain.h"

SHADER::ShaderMain::ShaderMain(const char * vertexShader, const char * fragmentShader)
	: ShaderObj(vertexShader, fragmentShader)
{
	if (m_shaderID != -1)
	{
		m_cameraVPMatrixID = glGetUniformLocation(m_shaderID, "ViewVP");
		m_cameraViewMatrixID = glGetUniformLocation(m_shaderID, "ViewV");
		m_modelMatrixID = glGetUniformLocation(m_shaderID, "M");
		m_shadowMapID = glGetUniformLocation(m_shaderID, "shadowMap");
		m_textureID = glGetUniformLocation(m_shaderID, "myTextureSampler");

		m_directionalLIghtBlockID = glGetUniformBlockIndex(m_shaderID, "DirectionalLIghtBlock");
		m_spotLIghtBlockID = glGetUniformBlockIndex(m_shaderID, "SpotLIghtBlock");
		m_pointLIghtBlockID = glGetUniformBlockIndex(m_shaderID, "PointLIghtBlock");
		m_deferredPointLIghtBlockID = glGetUniformBlockIndex(m_shaderID, "DeferredPointLIghtBlock");

		uniformBlockBind(m_directionalLIghtBlockID, UBO_INDEX_DIRECTIONAL_LIGHT);
		uniformBlockBind(m_spotLIghtBlockID, UBO_INDEX_SPOT_LIGHT);
		uniformBlockBind(m_pointLIghtBlockID, UBO_INDEX_POINT_LIGHT);
		uniformBlockBind(m_deferredPointLIghtBlockID, UBO_INDEX_DEFERRED_POINT_LIGHT);
	}
}

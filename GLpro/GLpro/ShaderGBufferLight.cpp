#include "ShaderGBufferLight.h"
#include "../../configs_ubo.h"

SHADER::ShaderGBufferLight::ShaderGBufferLight(const char * vertexShader, const char * fragmentShader)
	: ShaderObj(vertexShader, fragmentShader)
{
	if (m_shaderID != -1)
	{
		m_texWorld = glGetUniformLocation(m_shaderID, "gPositionMap");
		m_texColor = glGetUniformLocation(m_shaderID, "gColorMap");
		m_texNormal = glGetUniformLocation(m_shaderID, "gNormalMap");
		m_texShadow = glGetUniformLocation(m_shaderID, "shadowMap");


		m_cameraViewMat = glGetUniformLocation(m_shaderID, "ViewV");
		m_cameraVPMat = glGetUniformLocation(m_shaderID, "ViewVP");

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

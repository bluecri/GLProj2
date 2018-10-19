#include "ShaderGBufferGeo.h"
#include "../../configs_ubo.h"

SHADER::ShaderGBufferGeo::ShaderGBufferGeo(const char * vertexShader, const char * fragmentShader)
	: ShaderObj(vertexShader, fragmentShader)
{
	if (m_shaderID != -1)
	{
		m_cameraVPMatrixID = glGetUniformLocation(m_shaderID, "ViewVP");
		m_modelMatrixID = glGetUniformLocation(m_shaderID, "M");
		m_textureID = glGetUniformLocation(m_shaderID, "myTextureSampler");
		m_bloomValueID = glGetUniformLocation(m_shaderID, "bloomValue");

	}
}

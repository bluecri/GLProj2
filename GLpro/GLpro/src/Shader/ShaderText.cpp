#include "../../stdafx.h"
#include "ShaderText.h"

SHADER::ShaderText::ShaderText(const char * vertexShader, const char * fragmentShader)
	: ShaderObj(vertexShader, fragmentShader)
{
	if (m_shaderID != -1)
	{
		m_text2DUniformID = glGetUniformLocation(m_shaderID, _NameTextureSampler);
		m_movedVec2ID = glGetUniformLocation(m_shaderID, _NameMovedVec2);
		m_textColorID = glGetUniformLocation(m_shaderID, _NameTextColor);
	}
}

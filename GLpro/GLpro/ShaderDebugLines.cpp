#include "ShaderDebugLines.h"

SHADER::ShaderDebugLines::ShaderDebugLines(const char * vertexShader, const char * geometryShader, const char * fragmentShader)
	: ShaderObj(vertexShader, geometryShader, fragmentShader)
{
	if (m_shaderID != -1)
	{
		PosVecID = glGetUniformLocation(m_shaderID, _NamePosVec);
		AxisLenVecID = glGetUniformLocation(m_shaderID, _NameAxisLenVec);
		RenderTypeIntID = glGetUniformLocation(m_shaderID, _NameRednerTypeInt);
		LineColorVecID = glGetUniformLocation(m_shaderID, _NameLineColorVec);
		OBBMatrixID = glGetUniformLocation(m_shaderID, _NameOBBM);
		VPMatrixID = glGetUniformLocation(m_shaderID, _NameVP);
	}
}

SHADER::ShaderDebugLines::ShaderDebugLines(const char * vertexShader, const char * fragmentShader)
	: ShaderObj(vertexShader, fragmentShader)
{
	if (m_shaderID != -1)
	{
		PosVecID = glGetUniformLocation(m_shaderID, _NamePosVec);
		AxisLenVecID = glGetUniformLocation(m_shaderID, _NameAxisLenVec);
		RenderTypeIntID = glGetUniformLocation(m_shaderID, _NameRednerTypeInt);
		LineColorVecID = glGetUniformLocation(m_shaderID, _NameLineColorVec);
		OBBMatrixID = glGetUniformLocation(m_shaderID, _NameOBBM);
		VPMatrixID = glGetUniformLocation(m_shaderID, _NameVP);
	}
}

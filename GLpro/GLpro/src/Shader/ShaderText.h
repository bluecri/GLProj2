#pragma once
#include "./ShaderObj.h"

namespace SHADER 
{
	class ShaderText : public ShaderObj
	{
	public:
		ShaderText(const char * vertexShader, const char * fragmentShader)
			: ShaderObj(vertexShader, fragmentShader)
		{
			if (m_shaderID != -1)
			{
				m_text2DUniformID = glGetUniformLocation(m_shaderID, _NameTextureSampler);
				m_movedVec2ID = glGetUniformLocation(m_shaderID, _NameMovedVec2);
			}
		}
			
		GLuint m_text2DUniformID;
		GLuint m_movedVec2ID;

	private:
		const char * _NameTextureSampler = "myTextureSampler";
		const char * _NameMovedVec2 = "movedPosition";
	};
}

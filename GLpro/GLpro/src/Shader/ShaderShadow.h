#pragma once
#include "./ShaderObj.h"

namespace SHADER {
	class ShaderShadow : public ShaderObj
	{
	public:
		ShaderShadow(const char * vertexShader, const char * fragmentShader)
			: ShaderObj(vertexShader, fragmentShader)
		{
			if (m_shaderID != -1) 
			{
				MVPMatrixID = glGetUniformLocation(m_shaderID, _NameMVP);
			}
		}
		GLuint MVPMatrixID;

	private:
		const char * _NameMVP = "MVP";
	};
}

#pragma once
#include "./ShaderObj.h"

namespace SHADER 
{
	class ShaderText : public ShaderObj
	{
	public:
		ShaderText(const char * vertexShader, const char * fragmentShader);

		GLuint m_text2DUniformID;
		GLuint m_movedVec2ID;
		GLuint m_textColorID;

	private:
		const char * _NameTextureSampler = "myTextureSampler";
		const char * _NameMovedVec2 = "movedPosition";
		const char * _NameTextColor = "inColor";
	};
}

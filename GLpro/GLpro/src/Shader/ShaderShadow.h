#pragma once
#include "./ShaderObj.h"

namespace SHADER {
	class ShaderShadow : public ShaderObj
	{
	public:
		ShaderShadow(const char * vertexShader, const char * fragmentShader);

		GLuint MMatrixID;
		GLuint VPMatrixID;

	private:
		const char * _NameM = "M";
		const char * _NameVP = "VP";
		
	};
}

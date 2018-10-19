#pragma once

#include "./src/Shader/ShaderObj.h"

namespace SHADER
{
	// final pass
	class ShaderDebugLines : public ShaderObj
	{
	public:
		ShaderDebugLines(const char * vertexShader, const char * geometryShader, const char * fragmentShader);
		ShaderDebugLines(const char * vertexShader, const char * fragmentShader);

		GLuint PosVecID;
		GLuint AxisLenVecID;
		GLuint RenderTypeIntID;
		GLuint LineColorVecID;
		GLuint OBBMatrixID;
		GLuint VPMatrixID;

	private:
		const char * _NamePosVec = "posVec";
		const char * _NameAxisLenVec = "axisLenVec";
		const char * _NameRednerTypeInt = "renderType";
		const char * _NameLineColorVec = "lineColorVec";
		const char * _NameVP = "ViewVP";
		const char * _NameOBBM = "OBBM";
	};
}
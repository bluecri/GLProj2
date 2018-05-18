#pragma once

namespace SHADER {
	class ShaderObj;
}

namespace RENDER
{
	class RReneder
	{
		virtual SHADER::ShaderObj* getShader() = 0;
	};
}
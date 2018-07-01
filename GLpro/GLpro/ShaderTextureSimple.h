#pragma once
#include "./src/Shader/ShaderObj.h"

namespace SHADER {
	class ShaderTextureSimple : public ShaderObj
	{
	public:
		ShaderTextureSimple(const char * vertexShader, const char * fragmentShader)
			: ShaderObj(vertexShader, fragmentShader)
		{
			if (m_shaderID != -1)
			{
				TextureID = glGetUniformLocation(m_shaderID, _NameRenderTexture);
			}
		}
		GLuint TextureID;

	private:
		const char * _NameRenderTexture = "myTextureSampler";
	};
}

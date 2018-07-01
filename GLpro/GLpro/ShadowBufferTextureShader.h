#pragma once

#include "../../stdafx.h"

namespace SHADER
{
	class ShaderShadow;
}

namespace RESOURCE
{
	class ShadowBufferTextureShader
	{
	public:
		ShadowBufferTextureShader();
		virtual ~ShadowBufferTextureShader();
		void init();

		void bindFBO();
		void unbindFBO();
		void bindTexture();
		void bindShader();
		void unbindShader();

		SHADER::ShaderShadow*	_shadowShader;
		int _fboX, _fboY;
	protected:
		void createBuffer();

	private:
		GLuint	_shadowFBO;
		GLuint	_shadowDepthTexture;

	};
}

extern RESOURCE::ShadowBufferTextureShader* GShadowBufferTexture;
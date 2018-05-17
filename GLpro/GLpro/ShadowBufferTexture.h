#pragma once

#include "../../stdafx.h"

namespace RESOURCE
{
	class ShadowBufferTexture
	{
	public:
		ShadowBufferTexture(int fboX = 1024, int fboY = 1024);
		virtual ~ShadowBufferTexture();
		void init();

		void bindFBO();
		void unbindFBO();
		void bindTexture();

	protected:

		void createBuffer();

	private:
		GLuint	_shadowFBO;
		GLuint	_shadowDepthTexture;

		int _fboX, _fboY;
	};
}

extern RESOURCE::ShadowBufferTexture* GShadowBufferTexture;
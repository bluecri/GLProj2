#include "stdafx.h"
#include "ShadowBufferTexture.h"
#include "src/Shader/ShaderManager.h"

RESOURCE::ShadowBufferTexture::ShadowBufferTexture(int fboX, int fboY)
{
	_fboX = fboX;
	_fboY = fboY;

}

RESOURCE::ShadowBufferTexture::~ShadowBufferTexture()
{
	glDeleteBuffers(1, &_shadowFBO);
}

void RESOURCE::ShadowBufferTexture::init()
{
	createBuffer();
	unbindFBO();
}

void RESOURCE::ShadowBufferTexture::bindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _shadowFBO);		// render to buffer -> texture

	glViewport(0, 0, _fboX, _fboY); // Render on the whole framebuffer, complete from the lower left corner to the upper right
	glCullFace(GL_FRONT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RESOURCE::ShadowBufferTexture::unbindFBO()
{
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	// renter to screen
	// TODO : roll back viewport
	// glViewport(0, 0, m_control->m_width, m_control->m_height); // Render on the whole framebuffer, complete from the lower left corner to the upper right
}

void RESOURCE::ShadowBufferTexture::bindTexture()
{
	glBindTexture(GL_TEXTURE_2D, _shadowDepthTexture);
}

void RESOURCE::ShadowBufferTexture::createBuffer()
{
	glGenFramebuffers(1, &_shadowFBO);
	glGenTextures(1, &_shadowDepthTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, _fboX, _fboY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _shadowDepthTexture, 0);
	// No color output in the bound framebuffer, only depth.

	GLenum DrawBuffers[1] = { GL_DEPTH_ATTACHMENT };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf_s("[ERR] : ShadowBufferTexture::createBuffer glCheckFramebufferStatus error\n");
		return;
	}
}

RESOURCE::ShadowBufferTexture* GShadowBufferTexture = nullptr;
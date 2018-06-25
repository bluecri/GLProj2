#include "stdafx.h"
#include "ShadowBufferTextureShader.h"
#include "src/Shader/ShaderManager.h"
#include "src/Shader/ShaderShadow.h"
#include "src/window.h"

#include "configs_light.h"

RESOURCE::ShadowBufferTextureShader::ShadowBufferTextureShader()
{
	_fboX = TEXTURE_SHADOW_WIDTH;
	_fboY = TEXTURE_SHADOW_HEIGHT;
	_shadowShader = GShaderManager->m_addShader<SHADER::ShaderShadow>(ENUM_SHADER_TYPE::SHADER_TYPE_SHADOW, "data/Shader/DepthRTT.vertexshader", "data/Shader/DepthRTT.fragmentshader");
}

RESOURCE::ShadowBufferTextureShader::~ShadowBufferTextureShader()
{
	glDeleteBuffers(1, &_shadowFBO);
}

void RESOURCE::ShadowBufferTextureShader::init()
{
	_shadowShader = GShaderManager->m_addShader<SHADER::ShaderShadow>(SHADER_TYPE_SHADOW, "data/Shader/DepthRTT.vertexshader", "data/Shader/DepthRTT.fragmentshader");

	createBuffer();
	unbindFBO();
}

void RESOURCE::ShadowBufferTextureShader::bindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _shadowFBO);		// render to buffer -> texture

	glViewport(0, 0, _fboX, _fboY); // Render on the whole framebuffer, complete from the lower left corner to the upper right
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);
}

void RESOURCE::ShadowBufferTextureShader::unbindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	// renter to screen
	glViewport(0, 0, GWindow->_windowWidth, GWindow->_windowHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right
	glCullFace(GL_BACK);
}

void RESOURCE::ShadowBufferTextureShader::bindTexture()
{
	glBindTexture(GL_TEXTURE_2D, _shadowDepthTexture);
}

void RESOURCE::ShadowBufferTextureShader::bindShader()
{
	_shadowShader->bind();
}

void RESOURCE::ShadowBufferTextureShader::unbindShader()
{
	_shadowShader->unbind();
}

void RESOURCE::ShadowBufferTextureShader::createBuffer()
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
		printf_s("[ERR] : ShadowBufferTextureShader::createBuffer glCheckFramebufferStatus error\n");
		return;
	}
}

RESOURCE::ShadowBufferTextureShader* GShadowBufferTexture = nullptr;
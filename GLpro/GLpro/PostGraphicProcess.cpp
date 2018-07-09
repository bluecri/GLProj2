#include "PostGraphicProcess.h"


#include "src/Shader/ShaderManager.h"
#include "src/Shader/ShaderShadow.h"
#include "ShaderGBufferGeo.h"
#include "ShaderGBufferLight.h"
#include "ShaderGBufferFinal.h"

#include "src/window.h"
#include "configs_light.h"

#include "src/RenderTarget/Normal/NormalFObj.h"
#include "RigidbodyComponent.h"

#include "src/Resource/Model.h"
#include "src/Resource/ModelManager.h"
#include "ModelOnlyVertex.h"
#include "src/Resource/Texture.h"
#include "src/Shader/ShaderObj.h"

#include "DeferredGFBO.h"
#include "Option.h"

RESOURCE::PostGraphicProcess::PostGraphicProcess(int GBOX, int GBOY)
{
	_GBOX = GBOX;
	_GBOY = GBOY;
}

RESOURCE::PostGraphicProcess::~PostGraphicProcess()
{
	glDeleteBuffers(1, &_FBO);
	glDeleteTextures(1, &_FBOFXAATexture);
}

void RESOURCE::PostGraphicProcess::initPostGraphicProcess()
{
	//_hdrShader = GShaderManager->m_addShader<SHADER::ShaderHDR>(SHADER_TYPE_SHADOW, "data/Shader/DepthRTT.vertexshader", "data/Shader/DepthRTT.fragmentshader");
	//_fxaaShader = GShaderManager->m_addShader<SHADER::ShaderFXAA>(SHADER_TYPE_SHADOW, "data/Shader/DepthRTT.vertexshader", "data/Shader/DepthRTT.fragmentshader");
	// SHADER::ShaderPostEffect*	_postEffectShader;

	createBuffer();
}

void RESOURCE::PostGraphicProcess::beforePostGraphicProcess(GLuint GFBORetTextureID, GLuint bloomTextureID, float exposure)
{
	_exposure = exposure;
	_renderPipePrevIdx = 0;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GFBORetTextureID);

	if (bloomTextureID != -1)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, bloomTextureID);
	}
}

void RESOURCE::PostGraphicProcess::doPostGraphicProcess()
{
	if (GOption->_useBloom)
	{
		// blooming bloom texture
	}

	if (GOption->_useHDR)
	{
		// get exposure
		calcExposure(GDeferredGFBO->calcExposureWithMipmap());
	}

	while (!postGraphicProcessLoop()) {};

	if (GOption->_useFXAA)
	{
		// draw target is FXAA Texture
		bindTargetTextureFXAA();

		if (GOption->_bUsePostEffect)
		{
			// draw target is Post Texture
			bindTargetTexturePostEffect();
		}
		else
		{
			bindDrawTargetScreen();
		}
	}
	else if(GOption->_bUsePostEffect)
	{
		// draw target is Post Texture
		bindTargetTexturePostEffect();



		bindDrawTargetScreen();

	}
	else
	{
		// only use HDR or Bloom :  draw target is screen
		bindDrawTargetScreen();
	}

	



	if (GOption->_useFXAA)
	{
		// draw to postTexture with blooming texture & exposure
	}

}

float RESOURCE::PostGraphicProcess::getExposure()
{
	return _exposure;
}

void RESOURCE::PostGraphicProcess::refreshRenderPipeline()
{
	_renderPipelineVec.clear();

	_renderPipelineVec.push_back(POST_PIPELINE_PREV);

	if (GOption->_useHDR)
		_renderPipelineVec.push_back(POST_PIPELINE_HDR);

	if (GOption->_useFXAA)
		_renderPipelineVec.push_back(POST_PIPELINE_FXAA);

	if (GOption->_bUsePostEffect)
		_renderPipelineVec.push_back(POST_PIPELINE_POST);

	_renderPipelineVec.push_back(POST_PIPELINE_SCREEN);
}

void RESOURCE::PostGraphicProcess::createBuffer()
{
	glGenFramebuffers(1, &_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);		// render to buffer -> texture

	glGenTextures(1, &_FBOFXAATexture);
	glBindTexture(GL_TEXTURE_2D, _FBOFXAATexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _GBOX, _GBOY, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _FBOFXAATexture, 0);
	
	/*
	glGenTextures(1, &_FBOPostEffectTexture);
	glBindTexture(GL_TEXTURE_2D, _FBOPostEffectTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _GBOX, _GBOY, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _FBOPostEffectTexture, 0);
	*/

	glBindFramebuffer(GL_FRAMEBUFFER, 0);	// renter to screen
}

void RESOURCE::PostGraphicProcess::bindTargetTextureFXAA()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf_s("[ERR] : PostGraphicProcess::bindTargetTextureIntColor glCheckFramebufferStatus error\n");
		return;
	}

	glClear(GL_COLOR_BUFFER_BIT);

}

void RESOURCE::PostGraphicProcess::unbindTargetTextureFXAA()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RESOURCE::PostGraphicProcess::bindTargetTexturePostEffect()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf_s("[ERR] : PostGraphicProcess::bindTargetTexturePostEffect glCheckFramebufferStatus error\n");
		return;
	}

	glClear(GL_COLOR_BUFFER_BIT);
}

void RESOURCE::PostGraphicProcess::unbindTargetTexturePostEffect()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RESOURCE::PostGraphicProcess::bindDrawTargetScreen()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void RESOURCE::PostGraphicProcess::bindReadTextureGFBO()
{
}

void RESOURCE::PostGraphicProcess::bindReadTextureFXAA()
{
}

void RESOURCE::PostGraphicProcess::bindReadTexturePostEffect()
{
}

void RESOURCE::PostGraphicProcess::bindHDRShader()
{
	_hdrShader->bind();
	_hdrShader->loadInt(_hdrShader->m_texGFBO, 0);

	if (GOption->_useBloom)
		_hdrShader->loadInt(_hdrShader->m_texBloom, 1);

	_hdrShader->loadInt(_hdrShader->m_useHdr, (int)GOption->_useHDR);
	_hdrShader->loadInt(_hdrShader->m_useBloom, (int)GOption->_useBloom);

}

void RESOURCE::PostGraphicProcess::unbindHDRShader()
{
	_hdrShader->unbind();
}

void RESOURCE::PostGraphicProcess::bindFXAAShader()
{
	_fxaaShader->bind();
	_fxaaShader->loadInt(_fxaaShader->m_screenTex, 0);
	
}

void RESOURCE::PostGraphicProcess::unbindFXAAShader()
{
	_fxaaShader->unbind();
}

void RESOURCE::PostGraphicProcess::hdrDraw(float deltaTime)
{
	// render quad screen ( (-1, -1) ~ (1, 1) )
	ModelOnlyVertex* modelOnlyVertex = GModelManager->getModelOnlyVertexWIthName("defaultVertex_QuadScreeen");
	modelOnlyVertex->bind();
	modelOnlyVertex->render();
	modelOnlyVertex->unbind();
}

void RESOURCE::PostGraphicProcess::fxaaDraw(float deltaTime, std::list<std::shared_ptr<std::pair<RENDER_TARGET::NORMAL::NormalFObj*, RigidbodyComponent*>>>& drawObjList)
{
	// render quad screen ( (-1, -1) ~ (1, 1) )
	ModelOnlyVertex* modelOnlyVertex = GModelManager->getModelOnlyVertexWIthName("defaultVertex_QuadScreeen");
	modelOnlyVertex->bind();
	modelOnlyVertex->render();
	modelOnlyVertex->unbind();
}

void RESOURCE::PostGraphicProcess::screenDraw(float deltaTime)
{

	// render quad screen ( (-1, -1) ~ (1, 1) )
	ModelOnlyVertex* modelOnlyVertex = GModelManager->getModelOnlyVertexWIthName("defaultVertex_QuadScreeen");
	modelOnlyVertex->bind();
	modelOnlyVertex->render();
	modelOnlyVertex->unbind();
}



void RESOURCE::PostGraphicProcess::calcExposure(float newExposure)
{
	_exposure = glm::lerp(_exposure, newExposure, _exposureAdjustSpeed);
}

void RESOURCE::PostGraphicProcess::setExposure(float exp)
{
	_exposure = exp;
}

float RESOURCE::PostGraphicProcess::getExposure()
{
	return _exposure;
}

void RESOURCE::PostGraphicProcess::setExposureAdjustSpeed(float adjustSpeed)
{
	_exposureAdjustSpeed = adjustSpeed;
}

float RESOURCE::PostGraphicProcess::getExposureAdjustSpeed()
{
	return _exposureAdjustSpeed;
}

bool RESOURCE::PostGraphicProcess::postGraphicProcessLoop()
{
	int prevProcess = _renderPipelineVec[_renderPipePrevIdx];
	int curProcess = _renderPipelineVec[_renderPipePrevIdx + 1];

	if (curProcess == POST_PIPELINE_SCREEN)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// bind texture
		switch (prevProcess)
		{
		case POST_PIPELINE_PREV:
			return;
		case POST_PIPELINE_HDR:
			return;
		case POST_PIPELINE_FXAA:
			return;
		case POST_PIPELINE_POST:
			return;
		}

		// bind shader and drawCall
		switch (curProcess)
		{
		case POST_PIPELINE_HDR:
			return;
		case POST_PIPELINE_FXAA:
			return;
		case POST_PIPELINE_POST:
			return;
		case POST_PIPELINE_SCREEN:
			return;
		}

		return true;	// end
	}

	switch (prevProcess)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

	case POST_PIPELINE_PREV:
		return;
	case POST_PIPELINE_HDR:
		return;
	case POST_PIPELINE_FXAA:
		return;
	case POST_PIPELINE_POST:
		return;
	}

	switch (curProcess)
	{
	case POST_PIPELINE_HDR:
		return;
	case POST_PIPELINE_FXAA:
		return;
	case POST_PIPELINE_POST:
		return;
	case POST_PIPELINE_SCREEN:
		return;
	}

	_renderPipePrevIdx++;

	return false;
}

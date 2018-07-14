#include "PostGraphicProcess.h"


#include "src/Shader/ShaderManager.h"
#include "src/Shader/ShaderShadow.h"
#include "ShaderHDR.h"
#include "ShaderBloom.h"
#include "ShaderFXAA.h"

#include "ShaderTextureSimple.h"
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

	_exposureAdjustSpeed = 0.5f;

	_modelOnlyVertex = GModelManager->getModelOnlyVertexWIthName("defaultVertex_QuadScreeen");
}

RESOURCE::PostGraphicProcess::~PostGraphicProcess()
{
	glDeleteBuffers(1, &_FBO);
	glDeleteTextures(1, &_FBOFXAATexture);
}

void RESOURCE::PostGraphicProcess::initPostGraphicProcess()
{
	_shaderTextureSimple = GShaderManager->m_addShader<SHADER::ShaderTextureSimple>(ENUM_SHADER_TYPE::SHADER_TYPE_TEXTURESIMPLE, "data/Shader/TexturePrint.vertexshader", "data/Shader/TexturePrint.fragmentshader");
	_hdrShader = GShaderManager->m_addShader<SHADER::ShaderHDR>(SHADER_TYPE_SHADOW, "data/Shader/HDR.vertexshader", "data/Shader/HDR.fragmentshader");
	_bloomShader = GShaderManager->m_addShader<SHADER::ShaderBloom>(SHADER_TYPE_SHADOW, "data/Shader/Bloom.vertexshader", "data/Shader/Bloom.fragmentshader");
	_fxaaShader = GShaderManager->m_addShader<SHADER::ShaderFXAA>(SHADER_TYPE_SHADOW, "data/Shader/FXAA.vertexshader", "data/Shader/FXAA.fragmentshader");
	// SHADER::ShaderPostEffect*	_postEffectShader;
	createBuffer();

	refreshRenderPipeline();
}

void RESOURCE::PostGraphicProcess::beforePostGraphicProcess(GLuint GFBORetTextureID, GLuint bloomBeforeTextureID)
{
	_renderPipePrevIdx = 0;

	// binded in calcExposure
	_GFBORetTextureID = GFBORetTextureID;
	_bloomBeforeTexture = bloomBeforeTextureID;

	glDisable(GL_DEPTH_TEST);	// particle depth test

}

void RESOURCE::PostGraphicProcess::doPostGraphicProcess()
{
	if (GOption->_useHDR)
	{
		// get exposure
		calcExposure(calcExposureWithMipmap());
	}

	if (GOption->_useBloom && _bloomBeforeTexture != -1)
	{
		// blooming bloom texture
		doBlooming();
	}

	while (!postGraphicProcessLoop()) {};

	unbindTargetTexture();

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
	
	
	glGenTextures(1, &_FBOPostEffectTexture);
	glBindTexture(GL_TEXTURE_2D, _FBOPostEffectTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _GBOX, _GBOY, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _FBOPostEffectTexture, 0);
	

	glGenTextures(1, &_FBOHDRTexture);
	glBindTexture(GL_TEXTURE_2D, _FBOHDRTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _GBOX, _GBOY, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, _FBOHDRTexture, 0);


	glGenTextures(1, &_FBOBloomVertTexture);
	glBindTexture(GL_TEXTURE_2D, _FBOBloomVertTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _GBOX, _GBOY, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, _FBOBloomVertTexture, 0);

	glGenTextures(1, &_FBOBloomHorTexture);
	glBindTexture(GL_TEXTURE_2D, _FBOBloomHorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _GBOX, _GBOY, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, _FBOBloomHorTexture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);	// renter to screen
}

void RESOURCE::PostGraphicProcess::bindTargetTextureHDR()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf_s("[ERR] : PostGraphicProcess::bindTargetTextureIntColor glCheckFramebufferStatus error\n");
		return;
	}

	glClear(GL_COLOR_BUFFER_BIT);
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

void RESOURCE::PostGraphicProcess::unbindTargetTexture()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void RESOURCE::PostGraphicProcess::bindReadTextureGFBO()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _GFBORetTextureID);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _FBOBloomHorTexture);
}

void RESOURCE::PostGraphicProcess::bindReadTextureHDR()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _FBOHDRTexture);
}

void RESOURCE::PostGraphicProcess::bindReadTextureFXAA()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _FBOFXAATexture);
}

void RESOURCE::PostGraphicProcess::bindReadTexturePostEffect()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _FBOPostEffectTexture);
}

void RESOURCE::PostGraphicProcess::bindSimpleShader()
{
	_shaderTextureSimple->bind();
	_shaderTextureSimple->loadInt(_shaderTextureSimple->TextureID, 0);
}

void RESOURCE::PostGraphicProcess::bindHDRShader()
{
	_hdrShader->bind();
	_hdrShader->loadInt(_hdrShader->m_texGFBO, 0);

	if (GOption->_useBloom)
		_hdrShader->loadInt(_hdrShader->m_texBloom, 1);

	_hdrShader->loadInt(_hdrShader->m_useBloom, (int)GOption->_useBloom);
	_hdrShader->loadInt(_hdrShader->m_useToneMapping, (int)GOption->_useToneMapping);

	_hdrShader->loadFloat(_hdrShader->m_exposure, _exposure);
}

void RESOURCE::PostGraphicProcess::unbindHDRShader()
{
	_hdrShader->unbind();
}

void RESOURCE::PostGraphicProcess::bindFXAAShader()
{
	_fxaaShader->bind();
	_fxaaShader->loadInt(_fxaaShader->m_screenTex, 0);

	_fxaaShader->loadFloat(_fxaaShader->m_lumaThreshold, 0.5f);
	_fxaaShader->loadFloat(_fxaaShader->m_mulReduce, 1.0f/8.0f);
	_fxaaShader->loadFloat(_fxaaShader->m_minReduce, 1.0f/128.0f);
	_fxaaShader->loadFloat(_fxaaShader->m_maxSpan, 8.0);
	_fxaaShader->loadVector2(_fxaaShader->m_texelStep, glm::vec2(1.0f/(float)_GBOX, 1.0f / (float)_GBOY));
}

void RESOURCE::PostGraphicProcess::unbindFXAAShader()
{
	_fxaaShader->unbind();
}

void RESOURCE::PostGraphicProcess::hdrDraw()
{
	// render quad screen ( (-1, -1) ~ (1, 1) )
	_modelOnlyVertex->bind();
	_modelOnlyVertex->render();
	_modelOnlyVertex->unbind();
}

void RESOURCE::PostGraphicProcess::fxaaDraw()
{
	// render quad screen ( (-1, -1) ~ (1, 1) )
	_modelOnlyVertex->bind();
	_modelOnlyVertex->render();
	_modelOnlyVertex->unbind();
}

void RESOURCE::PostGraphicProcess::screenDraw()
{
	// render quad screen ( (-1, -1) ~ (1, 1) )
	_modelOnlyVertex->bind();
	_modelOnlyVertex->render();
	_modelOnlyVertex->unbind();
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

float RESOURCE::PostGraphicProcess::calcExposureWithMipmap()
{
	// get result texture mipmap
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _GFBORetTextureID);
	glGenerateMipmap(GL_TEXTURE_2D);

	int mipmapLevel = (int)floor(log2((float)max(_GBOX, _GBOY))) + 0;
	float avgColor[3] = { 0.0f, 0.0f, 0.0f };
	glGetTexImage(GL_TEXTURE_2D, mipmapLevel, GL_RGB, GL_FLOAT, (void*)avgColor);

	// debug
	//printf_s("%f %f %f\n", avgColor[0], avgColor[1], avgColor[2]);

	// reference : https://www.opengl.org/discussion_boards/showthread.php/164483-automatic-exposure-control
	float newExposure = 0.5 / (avgColor[0] + avgColor[1] + avgColor[2]);

	return newExposure;
}

bool RESOURCE::PostGraphicProcess::postGraphicProcessLoop()
{
	int prevProcess = _renderPipelineVec[_renderPipePrevIdx];
	int curProcess = _renderPipelineVec[_renderPipePrevIdx + 1];

	if (curProcess == POST_PIPELINE_SCREEN)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _FBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		// bind texture
		switch (prevProcess)
		{
		case POST_PIPELINE_PREV:
			bindReadTextureGFBO();
			bindSimpleShader();
			_modelOnlyVertex->bind();
			_modelOnlyVertex->render();
			_modelOnlyVertex->unbind();
			break;

		case POST_PIPELINE_HDR:
			glReadBuffer(GL_COLOR_ATTACHMENT2);
			glBlitFramebuffer(0, 0, _GBOX, _GBOY, 0, 0, _GBOX, _GBOY, GL_COLOR_BUFFER_BIT, GL_LINEAR);
			break;

		case POST_PIPELINE_FXAA:
			glReadBuffer(GL_COLOR_ATTACHMENT0);
			glBlitFramebuffer(0, 0, _GBOX, _GBOY, 0, 0, _GBOX, _GBOY, GL_COLOR_BUFFER_BIT, GL_LINEAR);
			break;

		case POST_PIPELINE_POST:
			glReadBuffer(GL_COLOR_ATTACHMENT1);
			glBlitFramebuffer(0, 0, _GBOX, _GBOY, 0, 0, _GBOX, _GBOY, GL_COLOR_BUFFER_BIT, GL_LINEAR);
			break;
		}

		return true;	// end
	}

	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	switch (prevProcess)
	{
	case POST_PIPELINE_PREV:
		bindReadTextureGFBO();	// already binded in beforePostGraphicProcess func
		break;
	case POST_PIPELINE_HDR:
		bindReadTextureHDR();
		break;
	case POST_PIPELINE_FXAA:
		bindReadTextureFXAA();
		break;
	}

	switch (curProcess)
	{
	case POST_PIPELINE_HDR:
		bindTargetTextureHDR();
		bindHDRShader();
		hdrDraw();
		break;
	case POST_PIPELINE_FXAA:
		bindTargetTextureFXAA();
		bindFXAAShader();
		fxaaDraw();
		break;
	case POST_PIPELINE_POST:
		bindTargetTexturePostEffect();
		screenDraw();
		//bindPostShader();
		break;
	}

	_renderPipePrevIdx++;

	return false;
}

void RESOURCE::PostGraphicProcess::doBlooming()
{
	/*
	*	_bloomBeforeTexture -> _FBOBloomVertTexture -> _FBOBloomHorTexture
	*		-> hor -> vert ->... iteration
	*/
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

	_bloomShader->bind();

	bloomingFirst();		// prev to vert

	for (int loopBloom = 0; loopBloom < 9; loopBloom++)
	{
		bloomingHor();		// vert to hor
		bloomingVert();		// hor to vert
	}

	bloomingHor();

	_modelOnlyVertex->unbind();
	_bloomShader->unbind();
}

void RESOURCE::PostGraphicProcess::bloomingFirst()
{
	// before bloom to vert
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	//glDrawBuffer(GL_COLOR_ATTACHMENT3);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _bloomBeforeTexture);

	_bloomShader->loadInt(_bloomShader->m_texBloom, 2);
	_bloomShader->loadBool(_bloomShader->m_horizontal, false);

	_modelOnlyVertex->bind();
	_modelOnlyVertex->render();
}

void RESOURCE::PostGraphicProcess::bloomingVert()
{
	// hor to ver
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _FBOBloomHorTexture);

	_bloomShader->loadInt(_bloomShader->m_texBloom, 2);
	_bloomShader->loadBool(_bloomShader->m_horizontal, false);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf_s("[ERR] : PostGraphicProcess::doBlooming glCheckFramebufferStatus error\n");
		return;
	}

	_modelOnlyVertex->render();
}

void RESOURCE::PostGraphicProcess::bloomingHor()
{
	// vert to hor
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _FBOBloomVertTexture);

	_bloomShader->loadInt(_bloomShader->m_texBloom, 2);
	_bloomShader->loadBool(_bloomShader->m_horizontal, true);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT4 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf_s("[ERR] : PostGraphicProcess::doBlooming glCheckFramebufferStatus error\n");
		return;
	}

	_modelOnlyVertex->render();
}

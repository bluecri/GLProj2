#include "PostGraphicProcess.h"


#include "src/Shader/ShaderManager.h"
#include "src/Shader/ShaderShadow.h"
#include "ShaderHDR.h"
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

	modelOnlyVertex = GModelManager->getModelOnlyVertexWIthName("defaultVertex_QuadScreeen");
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
	_fxaaShader = GShaderManager->m_addShader<SHADER::ShaderFXAA>(SHADER_TYPE_SHADOW, "data/Shader/FXAA.vertexshader", "data/Shader/FXAA.fragmentshader");
	// SHADER::ShaderPostEffect*	_postEffectShader;
	createBuffer();

	refreshRenderPipeline();
}

void RESOURCE::PostGraphicProcess::beforePostGraphicProcess(GLuint GFBORetTextureID, GLuint bloomTextureID)
{
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

	unbindTargetTexture();

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
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _FBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		// bind texture
		switch (prevProcess)
		{
		case POST_PIPELINE_PREV:
			//bindReadTextureGFBO();	// already binded in beforePostGraphicProcess func
			bindSimpleShader();
			modelOnlyVertex->bind();
			modelOnlyVertex->render();
			modelOnlyVertex->unbind();
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

	switch (prevProcess)
	{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

	case POST_PIPELINE_PREV:
		//bindReadTextureGFBO();	// already binded in beforePostGraphicProcess func
		return;
	case POST_PIPELINE_HDR:
		bindReadTextureHDR();
		return;
	case POST_PIPELINE_FXAA:
		bindReadTextureFXAA();
		return;
	}

	switch (curProcess)
	{
	case POST_PIPELINE_HDR:
		bindTargetTextureHDR();
		bindHDRShader();
		return;
	case POST_PIPELINE_FXAA:
		bindTargetTextureFXAA();
		bindFXAAShader();
		return;
	case POST_PIPELINE_POST:
		bindTargetTexturePostEffect();
		//bindPostShader();
		return;
	}

	_renderPipePrevIdx++;

	return false;
}

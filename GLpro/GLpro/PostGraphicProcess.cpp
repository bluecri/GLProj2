#include "PostGraphicProcess.h"


#include "src/Shader/ShaderManager.h"
#include "src/Shader/ShaderShadow.h"
#include "ShaderMipmap.h"
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

	_swapPBOIndex = 0;
	_curExposureUpdateInterval = 0;
	_exposureUpdateInterval = 10;
	_mipmapLevel = (int)floor(log2((float)max(_GBOX, _GBOY))) + 0;
	_bFirstTry = true;
}

RESOURCE::PostGraphicProcess::~PostGraphicProcess()
{
	glDeleteBuffers(1, &_FBO);
	glDeleteTextures(1, &_FBOFXAATexture);
}

void RESOURCE::PostGraphicProcess::initPostGraphicProcess()
{
	_shaderTextureSimple = GShaderManager->m_addShader<SHADER::ShaderTextureSimple>(ENUM_SHADER_TYPE::SHADER_TYPE_TEXTURESIMPLE, "data/Shader/TexturePrint.vertexshader", "data/Shader/TexturePrint.fragmentshader");
	_hdrShader = GShaderManager->m_addShader<SHADER::ShaderHDR>(SHADER_TYPE_POST_HDR, "data/Shader/HDR.vertexshader", "data/Shader/HDR.fragmentshader");
	_bloomShader = GShaderManager->m_addShader<SHADER::ShaderBloom>(SHADER_TYPE_POST_BLOOM, "data/Shader/Bloom.vertexshader", "data/Shader/Bloom.fragmentshader");
	_fxaaShader = GShaderManager->m_addShader<SHADER::ShaderFXAA>(SHADER_TYPE_POST_FXAA, "data/Shader/FXAA.vertexshader", "data/Shader/FXAA.fragmentshader");
	_mipmapShader = GShaderManager->m_addShader<SHADER::ShaderMipmap>(SHADER_TYPE_POST_MIPMAP, "data/Shader/MipmapLast.vertexshader", "data/Shader/MipmapLast.fragmentshader");

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
		//calcExposure(calcExposureWithMipmap());
		GenerateMipmapExposure();
		/*
		if (GOption->_useReadPixel)
		{
			calcExposure(calcExposureWithMipmapReadPixel());
		}
		else
		{
			calcExposure(calcExposureWithMipmap());
		}
		*/
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

	// mipmap FBO & Texture
	glGenFramebuffers(1, &_FBOForMipmap);
	glBindFramebuffer(GL_FRAMEBUFFER, _FBOForMipmap);		// render to buffer -> texture

	glGenTextures(1, &_FBOLastMipmapTexture);
	glBindTexture(GL_TEXTURE_2D, _FBOLastMipmapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _FBOLastMipmapTexture, 0);
	
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	// renter to screen

	// PBO
	glGenBuffers(PBO_SIZE, _pixelBufferID);
	for (int i = 0; i < PBO_SIZE; i++)
	{
		glBindBuffer(GL_PIXEL_PACK_BUFFER, _pixelBufferID[i]);
		glBufferData(GL_PIXEL_PACK_BUFFER, sizeof(GLfloat) * 3, NULL, GL_STREAM_READ);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	}
	

	glGenTextures(1, &_emptyTestTexture);
	glBindTexture(GL_TEXTURE_2D, _emptyTestTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _GBOX, _GBOY, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void RESOURCE::PostGraphicProcess::bindTargetTextureHDR()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	/*	opt
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf_s("[ERR] : PostGraphicProcess::bindTargetTextureIntColor glCheckFramebufferStatus error\n");
		return;
	}
	*/
	glClear(GL_COLOR_BUFFER_BIT);
}


void RESOURCE::PostGraphicProcess::bindTargetTextureFXAA()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	/*	opt
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf_s("[ERR] : PostGraphicProcess::bindTargetTextureIntColor glCheckFramebufferStatus error\n");
		return;
	}
	*/

	glClear(GL_COLOR_BUFFER_BIT);

}

void RESOURCE::PostGraphicProcess::bindTargetTexturePostEffect()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	/*	opt
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf_s("[ERR] : PostGraphicProcess::bindTargetTexturePostEffect glCheckFramebufferStatus error\n");
		return;
	}
	*/

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
	//glGenerateMipmap(GL_TEXTURE_2D);

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

	_hdrShader->loadInt(_hdrShader->m_mipmapLevel, _mipmapLevel);
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

void RESOURCE::PostGraphicProcess::GenerateMipmapExposure()
{
	glActiveTexture(GL_TEXTURE4);
	//glBindTexture(GL_TEXTURE_2D, _emptyTestTexture);
	glBindTexture(GL_TEXTURE_2D, _GFBORetTextureID);
	glGenerateMipmap(GL_TEXTURE_2D);
}

float RESOURCE::PostGraphicProcess::calcExposureWithMipmap()
{
	// generate mipmap
	
	//glBindTexture(GL_TEXTURE_2D, _emptyTestTexture);

	/*
	_curExposureUpdateInterval = (_curExposureUpdateInterval + 1) % _exposureUpdateInterval;
	if (_curExposureUpdateInterval != 1)
	{
		return _prevExposure;
	}
	*/

	int curFrame = _swapPBOIndex % PBO_SIZE;
	_swapPBOIndex = (_swapPBOIndex + 1) % PBO_SIZE;
	int nextFrame = _swapPBOIndex;
	curFrame = 0;

	// generate mipmap
	glActiveTexture(GL_TEXTURE4);
	
	//glBindTexture(GL_TEXTURE_2D, _emptyTestTexture);
	glBindTexture(GL_TEXTURE_2D, _GFBORetTextureID);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindBuffer(GL_PIXEL_PACK_BUFFER, _pixelBufferID[curFrame]);
	glBufferData(GL_PIXEL_PACK_BUFFER, sizeof(GLfloat) * 3, NULL, GL_STREAM_READ);
	glGetTexImage(GL_TEXTURE_2D, _mipmapLevel, GL_RGB, GL_FLOAT, NULL);

	/*
	_fenceSync[curFrame] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	
	
	printf_s("===========..\n");
	GLenum syncResult;
	while (GL_TIMEOUT_EXPIRED == (syncResult = glClientWaitSync(_fenceSync[curFrame], GL_SYNC_FLUSH_COMMANDS_BIT, 0)))
	{
		// Sleep(10);
		printf_s("sleep..\n");
		// do other calculation
	}
	glDeleteSync(_fenceSync[curFrame]);
	*/

	/*
	GLsizei len;
	GLint values;
	glGetSynciv(_fenceSync[curFrame], GL_SYNC_STATUS, sizeof(values), &len, &values);
	if (GL_SIGNALED == values)
	{
		printf_s("always signaled\n");
	}
	*/

	/*
	if (_bFirstTry)
	{
		_prevExposure = 1.0f;
		_bFirstTry = false;
		return _prevExposure;
	}
	GLenum syncResult;
	printf_s("check _fenceSync:  %d..\n", nextFrame);
	while (GL_TIMEOUT_EXPIRED == (syncResult = glClientWaitSync(_fenceSync[nextFrame], GL_SYNC_FLUSH_COMMANDS_BIT, 0)))
	{
		//Sleep(1);
		printf_s("sleep..\n");
		// do other calculation
	}

	//case GL_ALREADY_SIGNALED:
	//case GL_CONDITION_SATISFIED:
	if (syncResult == GL_WAIT_FAILED)
	{
		printf_s("PostGraphicProcess::calcExposureWithMipmap syncResult is GL_WAIT_FAILED\n");
	}

	if (syncResult == GL_ALREADY_SIGNALED)
	{
		//printf_s("PostGraphicProcess::calcExposureWithMipmap syncResult is GL_ALREADY_SIGNALED\n");
	}

	if (syncResult == GL_CONDITION_SATISFIED)
	{
		printf_s("PostGraphicProcess::calcExposureWithMipmap syncResult is GL_CONDITION_SATISFIED\n");
	}
	glDeleteSync(_fenceSync[nextFrame]);
	*/

	// read prev pixel : from PBO to mem
	
	glBindBuffer(GL_PIXEL_PACK_BUFFER, _pixelBufferID[curFrame]);
	GLfloat* ptr = (GLfloat*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY_ARB);
	if (ptr)
	{
		_prevExposure = 0.5 / (ptr[0] + ptr[1] + ptr[2] + 0.01);	// 0.01 : prevent value to INF
		glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
		printf_s("%f %f %f\n", ptr[0], ptr[1], ptr[2]);
		//printf_s("%f\n", _prevExposure);
	}
	else
	{
		printf_s("PostGraphicProcess::calcExposureWithMipmap :: cannot get ptr\n");
		_prevExposure = 1.0f;
	}
	

	// reference : https://www.opengl.org/discussion_boards/showthread.php/164483-automatic-exposure-control
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

	return _prevExposure;
}

float RESOURCE::PostGraphicProcess::calcExposureWithMipmapReadPixel()
{
	/*
	// get result texture mipmap
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, _emptyTestTexture);
	glBindTexture(GL_TEXTURE_2D, _GFBORetTextureID);
	glGenerateMipmap(GL_TEXTURE_2D);

	int mipmapLevel = (int)floor(log2((float)max(_GBOX, _GBOY))) + 0;

	float avgColor[3] = { 0.0f, 0.0f, 0.0f };
	glGetTexImage(GL_TEXTURE_2D, mipmapLevel, GL_RGB, GL_FLOAT, (void*)avgColor);
	float newExposure = 0.5 / (avgColor[0] + avgColor[1] + avgColor[2]);

	printf_s("%f %f %f\n", avgColor[0], avgColor[1], avgColor[2]);
	return newExposure;
	*/

	// =====================================================================

	// get result texture mipmap
	/*
	if (_pboUpdateInterval % 10 != 0)
	{
	_pboUpdateInterval++;
	return _prevExposure;
	}
	_pboUpdateInterval++;
	*/

	int curFrame = _swapPBOIndex % 2;
	_swapPBOIndex = (_swapPBOIndex + 1) % 2;
	int nextFrame = _swapPBOIndex;

	// generate mipmap
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _GFBORetTextureID);
	glGenerateMipmap(GL_TEXTURE_2D);

	// print to frameBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, _FBOForMipmap);		// render to buffer -> texture
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	_mipmapShader->bind();
	_mipmapShader->loadInt(_mipmapShader->m_texture, 0);
	_mipmapShader->loadInt(_mipmapShader->m_mipmapLevel, _mipmapLevel);

	_modelOnlyVertex->bind();
	_modelOnlyVertex->render();
	_modelOnlyVertex->unbind();

	_mipmapShader->unbind();

	// read pixel : from frameBuffer to PBO
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, _pixelBufferID[curFrame]);
	glBufferData(GL_PIXEL_PACK_BUFFER, sizeof(GLfloat) * 3, NULL, GL_STREAM_READ);
	glReadPixels(0, 0, 1, 1, GL_RGB, GL_FLOAT, 0);	// NULL == offset of GL_PIXEL_PACK_BUFFER
													//glGetTexImage(GL_TEXTURE_2D, _mipmapLevel, GL_RGB, GL_FLOAT, NULL);


													// read prev pixel : from PBO to mem
	glBindBuffer(GL_PIXEL_PACK_BUFFER, _pixelBufferID[nextFrame]);
	GLfloat* ptr = (GLfloat*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY_ARB);
	if (ptr)
	{
		_prevExposure = 0.5 / (ptr[0] + ptr[1] + ptr[2] + 0.01);	// 0.01 : prevent value to INF
		glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
		printf_s("%f %f %f\n", ptr[0], ptr[1], ptr[2]);
		printf_s("%f\n", _prevExposure);
	}
	else
	{
		printf_s("PostGraphicProcess::calcExposureWithMipmap :: cannot get ptr\n");
		_prevExposure = 1.0f;
	}

	// reference : https://www.opengl.org/discussion_boards/showthread.php/164483-automatic-exposure-control
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return _prevExposure;

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

	/*
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf_s("[ERR] : PostGraphicProcess::doBlooming glCheckFramebufferStatus error\n");
		return;
	}
	*/

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

	/* opt
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf_s("[ERR] : PostGraphicProcess::doBlooming glCheckFramebufferStatus error\n");
		return;
	}
	*/

	_modelOnlyVertex->render();
}

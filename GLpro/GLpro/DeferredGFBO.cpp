#include "DeferredGFBO.h"

#include "src/Shader/ShaderManager.h"
#include "src/Shader/ShaderShadow.h"
#include "ShaderGBufferGeo.h"
#include "ShaderGBufferLight.h"
#include "ShaderGBufferFinal.h"

#include "src/window.h"
#include "configs_light.h"

#include "src/Camera/Camera.h"
#include "src/Camera/CameraManager.h"
#include "src/RenderTarget/Normal/NormalFObj.h"
#include "RigidbodyComponent.h"

#include "src/Resource/Model.h"
#include "src/Resource/ModelManager.h"
#include "ModelOnlyVertex.h"
#include "src/Resource/Texture.h"
#include "src/Shader/ShaderObj.h"

#include "LightManager.h"
#include "DirectionalLightManager.h"
#include "SpotLightManager.h"
#include "PointLightManager.h"
#include "DeferredPointLightManager.h"

#include "ShaderStructDirectionalLight.h"
#include "ShaderStructSpotLight.h"
#include "ShaderStructPointLight.h"
#include "ShaderStructDeferredPointLight.h"

RESOURCE::DeferredGFBO::DeferredGFBO(int screenSizeX, int screenSizeY)
{
	_shadowTextureFBOX = TEXTURE_SHADOW_WIDTH;
	_shadowTextureFBOY = TEXTURE_SHADOW_HEIGHT;

	_GBOX = screenSizeX;
	_GBOY = screenSizeY;

	_bRenderOnScreenDirect = false;
}

RESOURCE::DeferredGFBO::~DeferredGFBO()
{
	glDeleteTextures(1, &_lightsShadowDepthTexture);
	glDeleteTextures(1, &_GFBOWorldPosTexture);
	glDeleteTextures(1, &_GFBOColorTexture);
	glDeleteTextures(1, &_GFBONormalTexture);
	glDeleteTextures(1, &_GFBODepthTexture);
	glDeleteTextures(1, &_GFBOResultTexture);

	glDeleteBuffers(1, &_lightsShadowFBO);
	glDeleteBuffers(1, &_GFBO);
}

void RESOURCE::DeferredGFBO::init()
{
	_shadowShader	= GShaderManager->m_addShader<SHADER::ShaderShadow>(SHADER_TYPE_SHADOW, "data/Shader/DepthRTT.vertexshader", "data/Shader/DepthRTT.fragmentshader");
	_geoShader		= GShaderManager->m_addShader<SHADER::ShaderGBufferGeo>(SHADER_TYPE_GBUFFER_GEO, "data/Shader/GbufferGeoPass.vertexshader", "data/Shader/GbufferGeoPass.fragmentshader");
	_lightShader	= GShaderManager->m_addShader<SHADER::ShaderGBufferLight>(SHADER_TYPE_GBUFFER_LIGHT, "data/Shader/GbufferLightPass.vertexshader", "data/Shader/GbufferLightPass.fragmentshader");
	_finalShader	= GShaderManager->m_addShader<SHADER::ShaderGBufferFinal>(SHADER_TYPE_GBUFFER_FINAL, "data/Shader/GbufferFinalPass.vertexshader", "data/Shader/GbufferFinalPass.fragmentshader");

	createBuffer();
}

void RESOURCE::DeferredGFBO::deferredDraw(float deltaTime, std::list<std::shared_ptr<std::pair<RENDER_TARGET::NORMAL::NormalFObj*, RigidbodyComponent*>>>& drawObjList)
{
	shadowDraw(deltaTime, drawObjList);
	geoDraw(deltaTime, drawObjList);
	lightDraw(deltaTime, drawObjList);

	if (_bRenderOnScreenDirect)
		return;

	finalDraw(deltaTime, drawObjList);
}

void RESOURCE::DeferredGFBO::bindShadowFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _lightsShadowFBO);		// render to buffer -> texture
	glViewport(0, 0, _shadowTextureFBOX, _shadowTextureFBOY); // Render on the whole framebuffer, complete from the lower left corner to the upper right
	glClear(GL_DEPTH_BUFFER_BIT);
}

SHADER::ShaderShadow * RESOURCE::DeferredGFBO::getShaderDhadow()
{
	return _shadowShader;
}

void RESOURCE::DeferredGFBO::unbindShadowFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, _GBOX, _GBOY);
}

void RESOURCE::DeferredGFBO::bindGFBO_GEO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _GFBO);

	GLenum DrawBuffers[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};	//, GL_DEPTH_ATTACHMENT
	glDrawBuffers(3, DrawBuffers); // "1" is the size of DrawBuffers

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf_s("[ERR] : DeferredGFBO::bindGFBO_GEO glCheckFramebufferStatus error\n");
		return;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

SHADER::ShaderGBufferGeo * RESOURCE::DeferredGFBO::getShaderGFBOGeo()
{
	return _geoShader;
}

void RESOURCE::DeferredGFBO::unbindGFBO_GEO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RESOURCE::DeferredGFBO::bindGFBO_LIGHT()
{
	if (_bRenderOnScreenDirect)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _GFBO);

		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			printf_s("[ERR] : DeferredGFBO::bindGFBO_GEO glCheckFramebufferStatus error\n");
			return;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear

	}

	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear all draw before normal obj
}

SHADER::ShaderGBufferLight * RESOURCE::DeferredGFBO::getShaderGFBOLight()
{
	return _lightShader;
}

void RESOURCE::DeferredGFBO::unbndGFBO_LIGHT()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// render result texture to screen
void RESOURCE::DeferredGFBO::renderGFBOToScreen()
{
	if (_bRenderOnScreenDirect)
		return;

	glBlitFramebuffer(0, 0, _GBOX, _GBOY, 0, 0, _GBOX, _GBOY, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void RESOURCE::DeferredGFBO::bndGFBO_FINAL()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _GFBO);
	glReadBuffer(GL_COLOR_ATTACHMENT3);

	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // not clear screen
}

SHADER::ShaderGBufferFinal * RESOURCE::DeferredGFBO::getShaderGFBOFinal()
{
	return _finalShader;
}

void RESOURCE::DeferredGFBO::unbndGFBO_FINAL()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RESOURCE::DeferredGFBO::setRenderScreenDirect(bool bDirect)
{
	_bRenderOnScreenDirect = bDirect;
}

bool RESOURCE::DeferredGFBO::getRenderScreenDirect() {
	return _bRenderOnScreenDirect;
}

void RESOURCE::DeferredGFBO::shadowDraw(float deltaTime, std::list<std::shared_ptr<std::pair<RENDER_TARGET::NORMAL::NormalFObj*, RigidbodyComponent*>>>& drawObjList)
{
	// ===================== shadow buffer ===================== //

	bindShadowFBO();
	_shadowShader->bind();

	// ----------------- directional light ----------------- //
	ShaderStructDirectionalLight* directionalLightStruct = GLightManager->_directionalLightManager->getLightStruct();
	int textureWidthNum = TEXTURE_SHADOW_WIDTH / TEXTURE_DIRECTIONAL_LIGHT_WIDTH;

	for (int i = 0; i < directionalLightStruct->_lightNum; i++)
	{
		int viewPortLeft = (i % textureWidthNum) * TEXTURE_DIRECTIONAL_LIGHT_WIDTH;
		int viewPortTop = (i / textureWidthNum) * TEXTURE_DIRECTIONAL_LIGHT_WIDTH + TEXTURE_DIRECTIONAL_LIGHT_HEIGHT_START;
		glViewport(viewPortLeft, viewPortTop, TEXTURE_DIRECTIONAL_LIGHT_WIDTH, TEXTURE_DIRECTIONAL_LIGHT_WIDTH);

		glm::mat4 VP = directionalLightStruct->_lightPMat[i] * directionalLightStruct->_lightVMat[i];

		// draw objects
		for (auto it = drawObjList.begin(); it != drawObjList.end(); ) {
			RENDER_TARGET::NORMAL::NormalFObj* normalRenderTarget = (*it)->first;
			RigidbodyComponent* targetRigidbodyComponent = (*it)->second;

			if (normalRenderTarget->isBDeleted())
			{
				it = drawObjList.erase(it);
				continue;
			}

			if (!normalRenderTarget->isRender())
			{
				++it;
				continue;
			}

			glm::mat4 MVP = VP * targetRigidbodyComponent->getWorldMat();
			_shadowShader->loadMatrix4(_shadowShader->MVPMatrixID, MVP);	// bind global old shader

			normalRenderTarget->_model->bind();		// Model buffer bind
			normalRenderTarget->_model->render();
			normalRenderTarget->_model->unbind();

			++it;
		}


		glm::mat4 MVP = VP;
		_shadowShader->loadMatrix4(_shadowShader->MVPMatrixID, MVP);	// bind global old shader

		RENDER_TARGET::NORMAL::NormalFObj* roomModel = new RENDER_TARGET::NORMAL::NormalFObj(
			"data/Model/room.obj", true, "data/Texture/uvmap.DDS", "dds");
		roomModel->_model->bind();
		roomModel->_texture->bind();
		roomModel->_model->render();
		roomModel->_model->unbind();
	}

	// ----------------- spot light ----------------- //
	ShaderStructSpotLight* spotLightStruct = GLightManager->_spotLightManager->getLightStruct();
	textureWidthNum = TEXTURE_SHADOW_WIDTH / TEXTURE_SPOT_LIGHT_WIDTH;

	for (int i = 0; i < spotLightStruct->_lightNum; i++)
	{
		int viewPortLeft = (i % textureWidthNum) * TEXTURE_SPOT_LIGHT_WIDTH;
		int viewPortTop = (i / textureWidthNum) * TEXTURE_SPOT_LIGHT_WIDTH + TEXTURE_SPOT_LIGHT_HEIGHT_START;
		glViewport(viewPortLeft, viewPortTop, TEXTURE_SPOT_LIGHT_WIDTH, TEXTURE_SPOT_LIGHT_WIDTH);
		glm::mat4 VP = spotLightStruct->_lightP[i] * spotLightStruct->_lightV[i];

		// draw objects
		for (auto it = drawObjList.begin(); it != drawObjList.end(); ) {
			RENDER_TARGET::NORMAL::NormalFObj* normalRenderTarget = (*it)->first;
			RigidbodyComponent* targetRigidbodyComponent = (*it)->second;

			if (normalRenderTarget->isBDeleted())
			{
				it = drawObjList.erase(it);
				continue;
			}

			if (!normalRenderTarget->isRender())
			{
				++it;
				continue;
			}

			normalRenderTarget->_model->bind();		// Model buffer bind

			mat4 targetTotalMat = VP * targetRigidbodyComponent->getWorldMat();
			_shadowShader->loadMatrix4(_shadowShader->MVPMatrixID, targetTotalMat);	// bind global old shader
			normalRenderTarget->_model->render();

			normalRenderTarget->_model->unbind();

			++it;
		}

		glm::mat4 MVP = VP;
		_shadowShader->loadMatrix4(_shadowShader->MVPMatrixID, MVP);	// bind global old shader

		RENDER_TARGET::NORMAL::NormalFObj* roomModel = new RENDER_TARGET::NORMAL::NormalFObj(
			"data/Model/room.obj", true, "data/Texture/uvmap.DDS", "dds");
		roomModel->_model->bind();
		roomModel->_texture->bind();
		roomModel->_model->render();
		roomModel->_model->unbind();
	}

	// point light
	ShaderStructPointLight* pointLightStruct = GLightManager->_pointLightManager->getLightStruct();
	textureWidthNum = TEXTURE_SHADOW_WIDTH / TEXTURE_POINT_LIGHT_WIDTH / TEXTURE_POINT_CUBE_NUM;

	for (int i = 0; i < pointLightStruct->_lightNum; i++)
	{
		//_shadowShader->loadMatrix4(_shadowShader->VPMatrixID
		//glm::mat4 (&pointVPMatVec)[MAX_POINTL_LIGHT_NUM][6] = pointLightStruct->_lightVPMat;

		for (int k = 0; k < TEXTURE_POINT_CUBE_NUM; k++)
		{
			int viewPortLeft = (i % textureWidthNum + k) * TEXTURE_POINT_LIGHT_WIDTH;
			int viewPortTop = (i / textureWidthNum) * TEXTURE_POINT_LIGHT_WIDTH + TEXTURE_POINT_LIGHT_HEIGHT_START;
			glViewport(viewPortLeft, viewPortTop, TEXTURE_POINT_LIGHT_WIDTH, TEXTURE_POINT_LIGHT_WIDTH);

			glm::mat4 VPMat = pointLightStruct->_lightVPMat[i * 6 + k];

			// draw objects
			for (auto it = drawObjList.begin(); it != drawObjList.end(); ) {
				RENDER_TARGET::NORMAL::NormalFObj* normalRenderTarget = (*it)->first;
				RigidbodyComponent* targetRigidbodyComponent = (*it)->second;

				if (normalRenderTarget->isBDeleted())
				{
					it = drawObjList.erase(it);
					continue;
				}

				if (!normalRenderTarget->isRender())
				{
					++it;
					continue;
				}

				normalRenderTarget->_model->bind();		// Model buffer bind

				mat4 MVP = VPMat * targetRigidbodyComponent->getWorldMat();
				_shadowShader->loadMatrix4(_shadowShader->MVPMatrixID, MVP);	// bind global old shader
				normalRenderTarget->_model->render();

				normalRenderTarget->_model->unbind();

				++it;
			}
		}
	}

	_shadowShader->unbind();
	unbindShadowFBO();
}

void RESOURCE::DeferredGFBO::geoDraw(float deltaTime, std::list<std::shared_ptr<std::pair<RENDER_TARGET::NORMAL::NormalFObj*, RigidbodyComponent*>>>& drawObjList)
{
	bindGFBO_GEO();
	_geoShader->bind();

	CAMERA::Camera* cam = *(GCameraManager->GetMainCamera());

	// ====================draw object on screen=====================

	_geoShader->bind();

	_geoShader->loadMatrix4(_geoShader->m_cameraVPMatrixID, cam->getRecentVPMat());
	//_geoShader->loadMatrix4(_geoShader->m_textureID, cam->getRecentVPMat());
	//_geoShader->loadMatrix4(_geoShader->m_modelMatrixID, cam->getRecentVPMat());

	for (auto it = drawObjList.begin(); it != drawObjList.end(); ) {
		RENDER_TARGET::NORMAL::NormalFObj* normalRenderTarget = (*it)->first;
		RigidbodyComponent* targetRigidbodyComponent = (*it)->second;

		if (!normalRenderTarget->isRender())
		{
			++it;
			continue;
		}


		glActiveTexture(GL_TEXTURE0);			// active texture #
		normalRenderTarget->_texture->bind();	// real texture bind to #
		_geoShader->loadInt(_geoShader->m_textureID, 0);

		mat4& targetModelMat = targetRigidbodyComponent->getWorldMat();
		_geoShader->loadMatrix4(_geoShader->m_modelMatrixID, targetModelMat);

		normalRenderTarget->_model->bind();		// Model buffer bind
		normalRenderTarget->_model->render();

		normalRenderTarget->_texture->unbind();
		normalRenderTarget->_model->unbind();

		++it;
	}

	_geoShader->loadMatrix4(_geoShader->m_modelMatrixID, glm::mat4());

	RENDER_TARGET::NORMAL::NormalFObj* roomModel = new RENDER_TARGET::NORMAL::NormalFObj(
		"data/Model/room.obj", true, "data/Texture/uvmap.DDS", "dds");
	roomModel->_model->bind();
	roomModel->_texture->bind();
	roomModel->_model->render();
	roomModel->_model->unbind();

	_geoShader->unbind();

	unbindGFBO_GEO();
}

void RESOURCE::DeferredGFBO::lightDraw(float deltaTime, std::list<std::shared_ptr<std::pair<RENDER_TARGET::NORMAL::NormalFObj*, RigidbodyComponent*>>>& drawObjList)
{
	CAMERA::Camera* cam = *(GCameraManager->GetMainCamera());

	bindGFBO_LIGHT();
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	_lightShader->bind();

	// bind shadow texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _lightsShadowDepthTexture);
	_lightShader->loadInt(_lightShader->m_texShadow, 0);

	// bind GFBO textures
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _GFBOWorldPosTexture);
	_lightShader->loadInt(_lightShader->m_texWorld, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _GFBOColorTexture);
	_lightShader->loadInt(_lightShader->m_texColor, 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, _GFBONormalTexture);
	_lightShader->loadInt(_lightShader->m_texNormal, 3);

	_lightShader->loadMatrix4(_lightShader->m_cameraViewMat, cam->getRecentViewMat());
	_lightShader->loadMatrix4(_lightShader->m_cameraVPMat, cam->getRecentVPMat());

	// render quad screen ( (-1, -1) ~ (1, 1) )
	ModelOnlyVertex* modelOnlyVertex = GModelManager->getModelOnlyVertexWIthName("defaultVertex_QuadScreeen");
	modelOnlyVertex->bind();
	modelOnlyVertex->render();
	modelOnlyVertex->unbind();

	_lightShader->unbind();
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	unbndGFBO_LIGHT();
}

void RESOURCE::DeferredGFBO::finalDraw(float deltaTime, std::list<std::shared_ptr<std::pair<RENDER_TARGET::NORMAL::NormalFObj*, RigidbodyComponent*>>>& drawObjList)
{
	bndGFBO_FINAL();
	renderGFBOToScreen();
	unbndGFBO_FINAL();
}

void RESOURCE::DeferredGFBO::createBuffer()
{
	// =============== lights shadow depth FBO =============== //

	glGenFramebuffers(1, &_lightsShadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _lightsShadowFBO);		// render to buffer -> texture
	glGenTextures(1, &_lightsShadowDepthTexture);
	glBindTexture(GL_TEXTURE_2D, _lightsShadowDepthTexture);

	//todo : change large texture size according to User GPU
	//glGetIntegerv(GL_MAX_TEXTURE_SIZE, &testInt);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, _shadowTextureFBOX, _shadowTextureFBOY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _lightsShadowDepthTexture, 0);
	// No color output in the bound framebuffer, only depth.

	GLenum DrawBuffers[1] = { GL_DEPTH_ATTACHMENT };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf_s("[ERR] : ShadowBufferTextureShader::createBuffer glCheckFramebufferStatus error\n");
		return;
	}

	// =============== G Buffer =============== //

	glGenFramebuffers(1, &_GFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _GFBO);		// render to buffer -> texture

	glGenTextures(1, &_GFBOWorldPosTexture);
	glBindTexture(GL_TEXTURE_2D, _GFBOWorldPosTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _GBOX, _GBOY, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _GFBOWorldPosTexture, 0);

	glGenTextures(1, &_GFBOColorTexture);
	glBindTexture(GL_TEXTURE_2D, _GFBOColorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _GBOX, _GBOY, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _GFBOColorTexture, 0);
	
	glGenTextures(1, &_GFBONormalTexture);
	glBindTexture(GL_TEXTURE_2D, _GFBONormalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _GBOX, _GBOY, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, _GFBONormalTexture, 0);

	glGenTextures(1, &_GFBOResultTexture);
	glBindTexture(GL_TEXTURE_2D, _GFBOResultTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _GBOX, _GBOY, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, _GFBOResultTexture, 0);

	glGenTextures(1, &_GFBODepthTexture);
	glBindTexture(GL_TEXTURE_2D, _GFBODepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, _GBOX, _GBOY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _GFBODepthTexture, 0);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);	// renter to screen
}

RESOURCE::DeferredGFBO* GDeferredGFBO = nullptr;
#include "./RNormal.h"
#include "../Camera/CameraManager.h"
#include "../Camera/Camera.h"

#include "../RenderTarget/Normal/NormalFObj.h"
#include "../../ShaderTextureSimple.h"
#include "../Shader/ShaderMain.h"
#include "../Shader/ShaderShadow.h"
#include "../../LightManager.h"

#include "../Resource/ModelManager.h"
#include "../Resource/Model.h"
#include "../../ModelOnlyVertex.h"
#include "../Resource/Texture.h"
#include "../../RigidbodyComponent.h"

#include "../../Option.h"
#include "../../DeferredGFBO.h"
#include "../../ShadowBufferTextureShader.h"
#include "../Shader/ShaderManager.h"

#include "../../ShaderStructDirectionalLight.h"
#include "../../ShaderStructSpotLight.h"
#include "../../ShaderStructPointLight.h"

#include "../../LightManager.h"
#include "../../DirectionalLightManager.h"
#include "../../SpotLightManager.h"
#include "../../PointLightManager.h"
#include "../window.h"

//temp
#include "../Resource/TextureManager.h"


namespace RENDER
{
	RNormal::RNormal(SHADER::ShaderMain * shaderObj)
	{
		_shaderObj = shaderObj;
		_targetCamera = GCameraManager->GetMainCamera();
	}

	void RNormal::drawRRender(float deltaTime)
	{
	
	}

	std::shared_ptr<RNormal::DrawElement> RNormal::addDrawElem(RENDER_TARGET::NORMAL::NormalFObj * normalFObj, RigidbodyComponent * rigidComponent)
	{
		//auto elem = std::make_shared<DrawElement>(normalFObj, rigidComponent);
		auto elem = std::shared_ptr<RENDER::RNormal::DrawElement>(new RENDER::RNormal::DrawElement(normalFObj, rigidComponent), [](auto ptr)
		{
			delete ptr->first;
			delete ptr;
		});
		_normalDrawElemContainer.push_back(elem);
		return elem;
	}

	RNormal::DrawElemContainer& RNormal::getDrawElemList()
	{
		return _normalDrawElemContainer;
	}

	void RNormal::updateRRender()
	{
		
	}

	void RNormal::updateTargetCamera(CAMERA::Camera** cam)
	{
		_targetCamera = cam;
	}

	void RNormal::shadowBufferPreDraw(float deltaTime)
	{
		// ===============draw object on shadow buffer==============

		
		GShadowBufferTexture->bindFBO();		//bind texture
		glClear(GL_DEPTH_BUFFER_BIT);

		GShadowBufferTexture->bindShader();

		// directional light
		ShaderStructDirectionalLight* directionalLightStruct = GLightManager->_directionalLightManager->getLightStruct();
		int textureWidthNum = TEXTURE_SHADOW_WIDTH / TEXTURE_DIRECTIONAL_LIGHT_WIDTH;

		for (int i = 0; i < directionalLightStruct->_lightNum; i++)
		{
			int viewPortLeft = (i % textureWidthNum) * TEXTURE_DIRECTIONAL_LIGHT_WIDTH;
			int viewPortTop = (i / textureWidthNum) * TEXTURE_DIRECTIONAL_LIGHT_WIDTH + TEXTURE_DIRECTIONAL_LIGHT_HEIGHT_START;
			glViewport(viewPortLeft, viewPortTop, TEXTURE_DIRECTIONAL_LIGHT_WIDTH, TEXTURE_DIRECTIONAL_LIGHT_WIDTH);

			glm::mat4 VP = directionalLightStruct->_lightPMat[i] * directionalLightStruct->_lightVMat[i];

			// draw objects
			for (auto it = _normalDrawElemContainer.begin(); it != _normalDrawElemContainer.end(); ) {
				RENDER_TARGET::NORMAL::NormalFObj* normalRenderTarget = (*it)->first;
				RigidbodyComponent* targetRigidbodyComponent = (*it)->second;

				if (normalRenderTarget->isBDeleted())
				{
					it = _normalDrawElemContainer.erase(it);
					continue;
				}

				if (!normalRenderTarget->isRender())
				{
					++it;
					continue;
				}

				GShadowBufferTexture->_shadowShader->loadMatrix4(GShadowBufferTexture->_shadowShader->MMatrixID, targetRigidbodyComponent->getWorldMatRef());	// bind global old shader
				GShadowBufferTexture->_shadowShader->loadMatrix4(GShadowBufferTexture->_shadowShader->VPMatrixID, VP);	// bind global old shader

				normalRenderTarget->_model->bind();		// Model buffer bind
				normalRenderTarget->_model->render();
				normalRenderTarget->_model->unbind();

				++it;
			}


			GShadowBufferTexture->_shadowShader->loadMatrix4(GShadowBufferTexture->_shadowShader->MMatrixID, glm::mat4());	// bind global old shader
			GShadowBufferTexture->_shadowShader->loadMatrix4(GShadowBufferTexture->_shadowShader->VPMatrixID, VP);	// bind global old shader

			RENDER_TARGET::NORMAL::NormalFObj* roomModel = new RENDER_TARGET::NORMAL::NormalFObj(
				"data/Model/room.obj", true, "data/Texture/uvmap.DDS", "dds");
			roomModel->_model->bind();
			roomModel->_texture->bind();
			roomModel->_model->render();
			roomModel->_model->unbind();
		}


		// spot light
		ShaderStructSpotLight* spotLightStruct = GLightManager->_spotLightManager->getLightStruct();
		textureWidthNum = TEXTURE_SHADOW_WIDTH / TEXTURE_SPOT_LIGHT_WIDTH;

		for (int i = 0; i < spotLightStruct->_lightNum; i++)
		{
			int viewPortLeft = (i % textureWidthNum) * TEXTURE_SPOT_LIGHT_WIDTH;
			int viewPortTop = (i / textureWidthNum) * TEXTURE_SPOT_LIGHT_WIDTH + TEXTURE_SPOT_LIGHT_HEIGHT_START;
			glViewport(viewPortLeft, viewPortTop, TEXTURE_SPOT_LIGHT_WIDTH, TEXTURE_SPOT_LIGHT_WIDTH);
			glm::mat4 VP = spotLightStruct->_lightP[i] * spotLightStruct->_lightV[i];

			// draw objects
			for (auto it = _normalDrawElemContainer.begin(); it != _normalDrawElemContainer.end(); ) {
				RENDER_TARGET::NORMAL::NormalFObj* normalRenderTarget = (*it)->first;
				RigidbodyComponent* targetRigidbodyComponent = (*it)->second;

				if (normalRenderTarget->isBDeleted())
				{
					it = _normalDrawElemContainer.erase(it);
					continue;
				}

				if (!normalRenderTarget->isRender())
				{
					++it;
					continue;
				}

				normalRenderTarget->_model->bind();		// Model buffer bind

				GShadowBufferTexture->_shadowShader->loadMatrix4(GShadowBufferTexture->_shadowShader->MMatrixID, targetRigidbodyComponent->getWorldMatRef());	// bind global old shader
				GShadowBufferTexture->_shadowShader->loadMatrix4(GShadowBufferTexture->_shadowShader->VPMatrixID, VP);	// bind global old shader

				normalRenderTarget->_model->render();

				normalRenderTarget->_model->unbind();

				++it;
			}

			GShadowBufferTexture->_shadowShader->loadMatrix4(GShadowBufferTexture->_shadowShader->MMatrixID, glm::mat4());	// bind global old shader
			GShadowBufferTexture->_shadowShader->loadMatrix4(GShadowBufferTexture->_shadowShader->VPMatrixID, VP);	// bind global old shader

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
			//GShadowBufferTexture->_shadowShader->loadMatrix4(GShadowBufferTexture->_shadowShader->VPMatrixID
			//glm::mat4 (&pointVPMatVec)[MAX_POINTL_LIGHT_NUM][6] = pointLightStruct->_lightVPMat;

			for (int k = 0; k < TEXTURE_POINT_CUBE_NUM; k++)
			{
				int viewPortLeft = (i % textureWidthNum + k) * TEXTURE_POINT_LIGHT_WIDTH;
				int viewPortTop =  (i / textureWidthNum) * TEXTURE_POINT_LIGHT_WIDTH + TEXTURE_POINT_LIGHT_HEIGHT_START;
				glViewport(viewPortLeft, viewPortTop, TEXTURE_POINT_LIGHT_WIDTH, TEXTURE_POINT_LIGHT_WIDTH);
				
				glm::mat4 VPMat = pointLightStruct->_lightVPMat[i * 6 + k];

				// draw objects
				for (auto it = _normalDrawElemContainer.begin(); it != _normalDrawElemContainer.end(); ) {
					RENDER_TARGET::NORMAL::NormalFObj* normalRenderTarget = (*it)->first;
					RigidbodyComponent* targetRigidbodyComponent = (*it)->second;

					if (normalRenderTarget->isBDeleted())
					{
						it = _normalDrawElemContainer.erase(it);
						continue;
					}

					if (!normalRenderTarget->isRender())
					{
						++it;
						continue;
					}

					normalRenderTarget->_model->bind();		// Model buffer bind

					GShadowBufferTexture->_shadowShader->loadMatrix4(GShadowBufferTexture->_shadowShader->MMatrixID, targetRigidbodyComponent->getWorldMatRef());	// bind global old shader
					GShadowBufferTexture->_shadowShader->loadMatrix4(GShadowBufferTexture->_shadowShader->VPMatrixID, VPMat);	// bind global old shader

					normalRenderTarget->_model->render();

					normalRenderTarget->_model->unbind();

					++it;
				}
			}
		}

		GShadowBufferTexture->unbindShader();
		GShadowBufferTexture->unbindFBO();

		glViewport(0, 0, GWindow->_windowWidth, GWindow->_windowHeight);
	}

	void RNormal::shadowBufferDraw(float deltaTime)
	{
		SHADER::ShaderTextureSimple* shaderTextureSimple = GShaderManager->m_addShader<SHADER::ShaderTextureSimple>(ENUM_SHADER_TYPE::SHADER_TYPE_TEXTURESIMPLE, "data/Shader/TexturePrint.vertexshader", "data/Shader/TexturePrint.fragmentshader");
		shaderTextureSimple->bind();

		glActiveTexture(GL_TEXTURE4);

		// bind shadow texture
		GShadowBufferTexture->bindTexture();
		shaderTextureSimple->loadInt(shaderTextureSimple->TextureID, 4);

		//RESOURCE::Texture* blockTexture = GTextureManager->getTextureWithFileName("data/Texture/yellowBit.bmp", "bmp");
		//blockTexture->bind();
		//shaderTextureSimple->loadInt(shaderTextureSimple->TextureID, 4);
		
		RESOURCE::ModelOnlyVertex* modelOnlyVertex = GModelManager->getModelOnlyVertexWIthName("defaultVertex_QuadScreeen");

		if (modelOnlyVertex != nullptr)
		{
			modelOnlyVertex->bind();
			modelOnlyVertex->render();
			modelOnlyVertex->unbind();
		}

		shaderTextureSimple->unbind();
	}

	void RNormal::shadowMappingDraw(float deltaTime)
	{

		CAMERA::Camera* cam = *_targetCamera;

		// ====================draw object on screen=====================

		_shaderObj->bind();

		_shaderObj->loadMatrix4(_shaderObj->m_cameraViewMatrixID, cam->getRecentViewMat());
		_shaderObj->loadMatrix4(_shaderObj->m_cameraVPMatrixID, cam->getRecentVPMat());


		// bind shadow texture
		glActiveTexture(GL_TEXTURE1);
		GShadowBufferTexture->bindTexture();
		_shaderObj->loadInt(_shaderObj->m_shadowMapID, 1);

		for (auto it = _normalDrawElemContainer.begin(); it != _normalDrawElemContainer.end(); ) {
			RENDER_TARGET::NORMAL::NormalFObj* normalRenderTarget = (*it)->first;
			RigidbodyComponent* targetRigidbodyComponent = (*it)->second;

			if (normalRenderTarget->isBDeleted())
			{
				it = _normalDrawElemContainer.erase(it);
				continue;
			}

			if (!normalRenderTarget->isRender())
			{
				++it;
				continue;
			}

			normalRenderTarget->_model->bind();		// Model buffer bind

			glActiveTexture(GL_TEXTURE0);			// active texture #
			normalRenderTarget->_texture->bind();	// real texture bind to #
			_shaderObj->loadInt(_shaderObj->m_textureID, 0);	// set shader use for # shader

			mat4& targetModelMat = targetRigidbodyComponent->getWorldMat();
			_shaderObj->loadMatrix4(_shaderObj->m_modelMatrixID, targetModelMat);

			normalRenderTarget->_model->render();
			normalRenderTarget->_texture->unbind();
			normalRenderTarget->_model->unbind();

			++it;
		}

		_shaderObj->loadMatrix4(_shaderObj->m_modelMatrixID, glm::mat4());

		RENDER_TARGET::NORMAL::NormalFObj* roomModel = new RENDER_TARGET::NORMAL::NormalFObj(
			"data/Model/room.obj", true, "data/Texture/uvmap.DDS", "dds");
		roomModel->_model->bind();
		roomModel->_texture->bind();
		roomModel->_model->render();
		roomModel->_model->unbind();

		_shaderObj->unbind();
	}

	void RNormal::deferredDraw(float deltaTime)
	{
		GDeferredGFBO->deferredDraw(deltaTime, _normalDrawElemContainer);
	}

	SHADER::ShaderMain * RNormal::getShader() const
	{
		return _shaderObj;
	}	
}
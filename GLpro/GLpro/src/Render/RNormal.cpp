#include "./RNormal.h"
#include "../Camera/CameraManager.h"
#include "../Camera/Camera.h"

#include "../RenderTarget/Normal/NormalFObj.h"
#include "../Shader/ShaderMain.h"
#include "../Shader/ShaderShadow.h"
#include "../../LightManager.h"

#include "../Resource/Model.h"
#include "../Resource/Texture.h"
#include "../../RigidbodyComponent.h"

#include "../../Option.h"
#include "../../ShadowBufferTextureShader.h"
#include "../Shader/ShaderManager.h"

#include "../window.h"


namespace RENDER
{

	RNormal::RNormal(SHADER::ShaderMain * shaderObj)
	{
		_shaderObj = shaderObj;
		_targetCamera = GCameraManager->GetMainCamera();
	}


	void RNormal::draw(float deltaTime)
	{
		CAMERA::Camera* cam = *_targetCamera;
		if (GOption->_oldLightUse)
		{
			// ===============draw object on shadow buffer==============
			GShadowBufferTexture->bindFBO();
			GShadowBufferTexture->bindShader();
			
			glm::mat4 depthBiasMVP = GLightManager->directionalLightVec[0].GetDepthBiasMVP();

			for (auto elem : _normalDrawElemContainer) {
				RENDER_TARGET::NORMAL::NormalFObj* normalRenderTarget = elem->first;
				Transform* targetTransform = elem->second->_transform;

				if (!normalRenderTarget->isRender()) continue;

				normalRenderTarget->_model->bind();		// Model buffer bind

				mat4 targetTotalMat = depthBiasMVP * targetTransform->getWorldMat();
				GShadowBufferTexture->_shadowShader->loadMatrix4(GShadowBufferTexture->_shadowShader->depthMatrixID, targetTotalMat);	// bind global old shader
				normalRenderTarget->_model->render();

				normalRenderTarget->_model->unbind();
			}
			

			GShadowBufferTexture->unbindShader();
			GShadowBufferTexture->unbindFBO();

			// ====================draw object on screen=====================

			_shaderObj->bind();

			//glm::mat4 modelMatrix = _targetCamera->getCamModelMatRef;
			//glm::mat4 depthBiasMVP = GLightManager->directionalLightVec[0].GetDepthBiasMVP();

			
			_shaderObj->loadMatrix4(_shaderObj->m_cameraViewMatrixID, cam->getRecentViewMat());
			_shaderObj->loadMatrix4(_shaderObj->m_depthBiasID, depthBiasMVP);

			//_oldLightUse
			_shaderObj->loadVector3(_shaderObj->m_lightInvDirID, GLightManager->directionalLightVec[0].GetLightVec3());	// 1 light

			// bind shadow texture
			glActiveTexture(GL_TEXTURE1);
			GShadowBufferTexture->bindTexture();
			_shaderObj->loadInt(_shaderObj->m_shadowMapID, 1);


			for (auto elem : _normalDrawElemContainer) {
				RENDER_TARGET::NORMAL::NormalFObj* normalRenderTarget = elem->first;
				Transform* targetTransform = elem->second->_transform;

				if (!normalRenderTarget->isRender()) continue;

				normalRenderTarget->_model->bind();		// Model buffer bind

				glActiveTexture(GL_TEXTURE0);			// active texture #
				normalRenderTarget->_texture->bind();	// real texture bind to #
				_shaderObj->loadInt(_shaderObj->m_textureID, 0);	// set shader use for # shader

				mat4 targetModelMat = targetTransform->getWorldMat();
				mat4 mvpMat = cam->getRecentVPMat() * targetModelMat;
				_shaderObj->loadMatrix4(_shaderObj->m_modelMatrixID, targetModelMat);
				_shaderObj->loadMatrix4(_shaderObj->m_MVPMatrixID, mvpMat);

				normalRenderTarget->_model->render();

				normalRenderTarget->_texture->unbind();
				normalRenderTarget->_model->unbind();
			}
			
			_shaderObj->unbind();
		}
		
	}

	std::shared_ptr<RNormal::DrawElement> RNormal::addDrawElem(RENDER_TARGET::NORMAL::NormalFObj * normalFObj, RigidbodyComponent * rigidComponent)
	{
		auto elem = std::make_shared<DrawElement>(normalFObj, rigidComponent);
		_normalDrawElemContainer.push_back(elem);
		return elem;
	}

	void RNormal::update(CAMERA::Camera** cam)
	{
		_targetCamera = cam;
	}

	SHADER::ShaderMain * RNormal::getShader() const
	{
		return _shaderObj;
	}

	
	void RNormal::removeDrawElem(std::shared_ptr<DrawElement> shared)
	{
		DrawElement* ptr = shared.get();
		for (auto it = _normalDrawElemContainer.begin(); it != _normalDrawElemContainer.end();)
		{
			if ((*it).get() == ptr)
			{
				_normalDrawElemContainer.erase(it);
				return;
			}
			++it;
		}
	}
	
}
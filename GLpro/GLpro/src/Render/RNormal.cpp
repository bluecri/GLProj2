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

#include "../../RRender.h"

namespace RENDER
{

	RNormal::RNormal(SHADER::ShaderMain * shaderObj) : RReneder()
	{
		_shaderObj = shaderObj;
	}


	void RNormal::draw()
	{
		if (GOption->_oldLightUse)
		{
			// ===============draw object on shadow buffer==============
			GShadowBufferTexture->bindFBO();
			GShadowBufferTexture->bindShader();

			_targetCamera->updateRecentVPAndViewMat();
			glm::mat4 depthBiasMVP = GLightManager->directionalLightVec[0].GetDepthBiasMVP();

			for (auto elem : _normalDrawElemContainer) {
				RENDER_TARGET::NORMAL::NormalFObj* normalRenderTarget = elem->first;
				Transform* targetTransform = elem->second->_transform;

				normalRenderTarget->_model->bind();		// Model buffer bind

				mat4 targetTotalMat = depthBiasMVP * targetTransform->getTotalMat();
				GShadowBufferTexture->_shadowShader->loadMatrix4(GShadowBufferTexture->_shadowShader->depthMatrixID, targetTotalMat);	// bind global old shader
				normalRenderTarget->_model->render();

				normalRenderTarget->_model->unbind();
			}

			GShadowBufferTexture->unbindShader();
			GShadowBufferTexture->unbindFBO();

			// ====================draw object on screen=====================

			glCullFace(GL_BACK);
			_shaderObj->bind();

			_targetCamera->updateRecentVPAndViewMat();
			//glm::mat4 modelMatrix = _targetCamera->getCamModelMatRef;
			//glm::mat4 depthBiasMVP = GLightManager->directionalLightVec[0].GetDepthBiasMVP();

			_shaderObj->loadMatrix4(_shaderObj->m_cameraViewMatrixID, _targetCamera->getRecentViewMat());
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

				normalRenderTarget->_model->bind();		// Model buffer bind

				glActiveTexture(GL_TEXTURE0);			// active texture #
				normalRenderTarget->_texture->bind();	// real texture bind to #
				_shaderObj->loadInt(_shaderObj->m_textureID, 0);	// set shader use for # shader

				mat4 targetModelMat = targetTransform->getTotalMat();
				mat4 mvpMat = _targetCamera->getRecentVPMat() * targetModelMat;
				_shaderObj->loadMatrix4(_shaderObj->m_modelMatrixID, targetModelMat);
				_shaderObj->loadMatrix4(_shaderObj->m_MVPMatrixID, mvpMat);

				normalRenderTarget->_model->render();

				normalRenderTarget->_texture->unbind();
				normalRenderTarget->_model->unbind();
			}

			_shaderObj->unbind();
		}
	}

	std::shared_ptr<RNormalDrawElement> RNormal::addToDrawList(RENDER_TARGET::NORMAL::NormalFObj * normalFObj, RigidbodyComponent * rigidComponent)
	{
		auto elem = std::make_shared<RNormalDrawElement>(normalFObj, rigidComponent);
		_normalDrawElemContainer.push_back(elem);
		return elem;
	}

	void RNormal::update(CAMERA::Camera * cam)
	{
		_targetCamera = cam;
	}

	SHADER::ShaderMain * RNormal::getShader() const
	{
		return _shaderObj;
	}

	void RNormal::destructor(std::shared_ptr<RNormalDrawElement> shared)
	{
		RNormalDrawElement* ptr = shared.get();
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
#include "./RNormal.h"
#include "../Camera/CameraManager.h"
#include "../Camera/Camera.h"

#include "../RenderTarget/Normal/NormalFObj.h"
#include "../Shader/ShaderMain.h"
#include "../../LightManager.h"

#include "../Resource/Model.h"
#include "../Resource/Texture.h"
#include "../../RigidbodyComponent.h"

namespace RENDER
{
	RNormal::RNormal(SHADER::ShaderMain * shaderObj)
	{
		_shaderObj = shaderObj;
	}


	void RNormal::draw()
	{
		glCullFace(GL_BACK);
		_shaderObj->bind();

		_targetCamera->updateRecentVPAndViewMat();
		//glm::mat4 modelMatrix = _targetCamera->getCamModelMatRef;
		glm::mat4 depthBiasMVP = GLightManager->directionalLightVec[0].GetDepthBiasMVP();
		

		_shaderObj->loadMatrix4(_shaderObj->m_cameraViewMatrixID, _targetCamera->getRecentViewMat());
		_shaderObj->loadMatrix4(_shaderObj->m_depthBiasID, depthBiasMVP);
		_shaderObj->loadVector3(_shaderObj->m_lightInvDirID, GLightManager->directionalLightVec[0].GetLightVec3());	// 1 light

		// bind shadow texture
		glActiveTexture(GL_TEXTURE1);
		// todo : glBindTexture(GL_TEXTURE_2D, m_bufferManager->depthTexture);	// previous shadow texture
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

			glDrawElements(
				GL_TRIANGLES,      // mode
				normalRenderTarget->_model->getGLCount(),    // count
				GL_UNSIGNED_SHORT,   // type
				(void*)0         // element array buffer offset
			);
			
			normalRenderTarget->_texture->unbind();
			normalRenderTarget->_model->unbind();
		}

		_shaderObj->unbind();
	}

	RNormalDrawElement* RNormal::addToDrawList(RENDER_TARGET::NORMAL::NormalFObj * normalFObj, RigidbodyComponent * rigidComponent)
	{
		// todo : elem에서 바로 제거 가능하도록 하는 container 생성
		RNormalDrawElement* elem = new RNormalDrawElement(normalFObj, rigidComponent);
		_normalDrawElemContainer.push_back(elem);
		return elem;
	}

	void RNormal::update(CAMERA::Camera * cam)
	{
		_targetCamera = cam;
	}


	void RNormal::chageShader(SHADER::ShaderMain * other)
	{
		_shaderObj = other;
	}

	SHADER::ShaderMain * RNormal::getShader() const
	{
		return _shaderObj;
	}

}
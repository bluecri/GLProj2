#include "../../stdafx.h"

#include "./RSkybox.h"
#include "../Camera/Camera.h"
#include "../Shader/ShaderSkybox.h"
#include "../RenderTarget/Skybox/SkyboxFObj.h"

namespace RENDER
{

	RSkybox::RSkybox(SHADER::ShaderSkybox* shaderObj, float div = 1.1f)
	{
		_shaderObj = shaderObj;
		_div = div;
	}

	RSkybox::~RSkybox() {};

	RSkyboxElement* RSkybox::addToDrawList(RENDER_TARGET::SKYBOX::SkyboxFObj* skyboxFObj, RigidbodyComponent* rigidComponent)
	{
		// todo : elem에서 바로 제거 가능하도록 하는 container 생성
		RSkyboxElement* elem = new RSkyboxElement(skyboxFObj, rigidComponent);
		_skyboxFobjcetContainer.push_back(elem);
		return elem;
	}

	void RSkybox::update(CAMERA::Camera* cam)
	{
		_targetCamera = cam;
	}

	void RSkybox::draw()
	{
		_shaderObj->bind();

		Transform* cameraTransform = _targetCamera->_rigidbodyComponent->_transform;
		const glm::mat4& cameraModelMatrix = cameraTransform->getModelMatrixConstRef();
		const glm::mat4& cameraRotationMatrix= cameraTransform->getRotationMatrixConstRef();

		glm::mat4 tempMVP = _targetCamera->getCamProjMatRef() * cameraRotationMatrix * cameraModelMatrix;
		_shaderObj->loadMatrix4(_shaderObj->m_modelMatrixID, cameraModelMatrix);
		_shaderObj->loadMatrix4(_shaderObj->m_cameraViewMatrixID, cameraRotationMatrix);
		_shaderObj->loadMatrix4(_shaderObj->m_MVPMatrixID, tempMVP);

		glDepthMask(GL_FALSE);

		for (auto elem : _skyboxFobjcetContainer) {
			RENDER_TARGET::SKYBOX::SkyboxFObj* skyboxRenderTarget = elem.first;

			skyboxRenderTarget->_skbModel->bind();	// Model bind

			glActiveTexture(GL_TEXTURE0);
			for (int i = 0; i < 6; i++)
			{
				skyboxRenderTarget->_skbTexture->bind(i);		// Texture bind
				_shaderObj->loadInt(_shaderObj->m_textureID, 0);	//glUniform1i(shaderSkyboxVec[selectedSkyboxShaderIdx]->m_textureID, 0);
				glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
				skyboxRenderTarget->_skbTexture->unbind(i);
			}
			skyboxRenderTarget->_skbModel->unbind();
		}
		glDepthMask(GL_TRUE);
		_shaderObj->unbind();

	}

	void RSkybox::chageShader(SHADER::ShaderSkybox* other)
	{
		_shaderObj = other;
	}
	SHADER::ShaderObj* RSkybox::getShader() const
	{
		return _shaderObj;
	}
}
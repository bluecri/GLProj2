#include "./RSkybox.h"
#include "../Camera/Camera.h"
#include "../Camera/CameraManager.h"
#include "../Shader/ShaderSkybox.h"
#include "../RenderTarget/Skybox/SkyboxFObj.h"
#include "../RenderTarget/Skybox/SkyboxModel.h"
#include "../RenderTarget/Skybox/SkyboxTexture.h"
#include "../window.h"

namespace RENDER
{

	RSkybox::RSkybox(SHADER::ShaderSkybox* shaderObj, float div)
	{
		_targetCamera = GCameraManager->GetMainCamera();
		_shaderObj = shaderObj;
		_div = div;
	}


	std::shared_ptr<RENDER::RSkybox::DrawElement> RSkybox::addToDrawList(RENDER_TARGET::SKYBOX::SkyboxFObj * skyboxFObj, RigidbodyComponent * rigidComponent)
	{
		auto elem = std::make_shared<DrawElement>(skyboxFObj, rigidComponent);
		_skyboxDrawElemContainer.push_back(elem);
		return elem;
	}

	void RSkybox::update(CAMERA::Camera** cam)
	{
		_targetCamera = cam;
	}

	void RSkybox::draw(float deltaTime)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, GWindow->_windowWidth, GWindow->_windowHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right

		CAMERA::Camera* cam = *_targetCamera;

		_shaderObj->bind();

		// set camera view matrix(camera rotate mat * camera position mat)
		Transform* cameraTransform = cam->_rigidbodyComponent->_transform;
		//const glm::mat4& cameraModelMatrix = cameraTransform->getModelMatrixConstRef();
		//const glm::mat4& cameraRotationMatrix= cameraTransform->getLocalRotationMatrixConstRef();
		const glm::mat4& cameraViewMatrix = cameraTransform->getWorldMatRef();

		_shaderObj->loadMatrix4(_shaderObj->m_cameraViewMatrixID, cameraViewMatrix);

		glDepthMask(GL_FALSE);

		for (auto elem : _skyboxDrawElemContainer) {
			RENDER_TARGET::SKYBOX::SkyboxFObj* skyboxRenderTarget = elem->first;
			Transform* targetTransform = elem->second->_transform;

			if (!skyboxRenderTarget->isRender()) continue;

			glm::mat4 totalModelMat = targetTransform->getWorldMat();

			glm::mat4 tempMVP = cam->getCamProjMatRef() * cameraViewMatrix * totalModelMat;
			_shaderObj->loadMatrix4(_shaderObj->m_modelMatrixID, totalModelMat);
			_shaderObj->loadMatrix4(_shaderObj->m_MVPMatrixID, tempMVP);
			
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

	void RSkybox::destructor(std::shared_ptr<DrawElement> shared)
	{
		DrawElement* ptr = shared.get();
		for (auto it = _skyboxDrawElemContainer.begin(); it != _skyboxDrawElemContainer.end();)
		{
			if ((*it).get() == ptr)
			{
				_skyboxDrawElemContainer.erase(it);
				return;
			}
			++it;
		}
	}
	
}
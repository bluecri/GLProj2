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
		auto elem = std::shared_ptr<DrawElement>(new DrawElement(skyboxFObj, rigidComponent), [](auto ptr)
		{
			delete ptr->first;
			delete ptr;
		});
		_skyboxDrawElemContainer.push_back(elem);
		return elem;
	}

	void RSkybox::updateRRender()
	{
	}

	void RSkybox::drawRRender(float deltaTime)
	{
		CAMERA::Camera* cam = *_targetCamera;

		_shaderObj->bind();

		// set camera view matrix(camera rotate mat * camera position mat)
		RigidbodyComponent* cameraRigidbodyComponent = cam->getRigidbodyComponent();
		glm::mat4 cameraViewMatrix = cam->getRecentViewMat();

		// todo : use _div
		_shaderObj->loadMatrix4(_shaderObj->m_cameraViewMatrixID, cameraViewMatrix);

		//glDepthMask(GL_FALSE);

		for (auto it = _skyboxDrawElemContainer.begin(); it != _skyboxDrawElemContainer.end(); ) {
			RENDER_TARGET::SKYBOX::SkyboxFObj* skyboxRenderTarget = (*it)->first;
			RigidbodyComponent* targetRigidbodyComponent = (*it)->second;

			if (skyboxRenderTarget->isBDeleted())
			{
				_skyboxDrawElemContainer.erase(it);
				continue;
			}

			if (!skyboxRenderTarget->isRender())
			{
				++it;
				continue;
			}

			glm::mat4 totalModelMat = targetRigidbodyComponent->getWorldMat();

			glm::mat4 tempMVP = cam->getCamProjMatRef() * cameraViewMatrix * totalModelMat;
			_shaderObj->loadMatrix4(_shaderObj->m_modelMatrixID, totalModelMat);
			_shaderObj->loadMatrix4(_shaderObj->m_MVPMatrixID, tempMVP);
			
			skyboxRenderTarget->_skbModel->bind();	// Model bind

			glActiveTexture(GL_TEXTURE0);
			for (int i = 0; i < 6; i++)
			{
				skyboxRenderTarget->_skbTexture->bind(i);		// Texture bind
				_shaderObj->loadInt(_shaderObj->m_textureID, 0);	//glUniform1i(shaderSkyboxVec[selectedSkyboxShaderIdx]->m_textureID, 0);
				glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);		// direct draw Arrays call
				skyboxRenderTarget->_skbTexture->unbind(i);
			}
			skyboxRenderTarget->_skbModel->unbind();

			++it;
		}
		//glDepthMask(GL_TRUE);
		_shaderObj->unbind();

	}

	void RSkybox::updateTargetCamera(CAMERA::Camera ** cam)
	{
		_targetCamera = cam;
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
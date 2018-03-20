#ifndef __R_SKYBOX_H__
#define __R_SKYBOX_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "./RInterface.h"
#include "../RenderTarget/Skybox/SkyboxRenderTarget.h"
#include "../Shader/ShaderObj.h"

#include <string>

class RSkybox
{
public:
	RSkybox(SHADER::ShaderObj* shaderObj)
	{
		_shaderObj = shaderObj;
	}
	virtual ~RSkybox() {};
	void skyboxUpdate()
	{
		//TODO : move skybox model
	}
	void update(RENDER_TARGET::SKYBOX::SkyboxRenderTarget &skyboxRenderTarget) {

	}
	void draw(const RENDER_TARGET::SKYBOX::SkyboxRenderTarget &skyboxRenderTarget) {
		_shaderObj->bind();

		glm::mat4 tempMVP = m_ProjectionMatrix * m_viewMatrix * m_modelMatrix;

		_shaderObj->loadMatrix4(_shaderObj->m_modelMatrixID, m_modelMatrix);
		_shaderObj->loadMatrix4(_shaderObj->m_cameraViewMatrixID, m_viewMatrix);
		_shaderObj->loadMatrix4(_shaderObj->m_MVPMatrixID, tempMVP);

		glDepthMask(GL_FALSE);
		_skyboxRenderTarget->skbModel->bind();	// model bind

		glActiveTexture(GL_TEXTURE0);
		for (int i = 0; i < 6; i++)
		{
			_skyboxRenderTarget->skbTexture->bind(i);
			_shaderObj->loadInt(_shaderObj->m_textureID, 0);	//glUniform1i(shaderSkyboxVec[selectedSkyboxShaderIdx]->m_textureID, 0);
			glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
			_skyboxRenderTarget->skbTexture->unbind(i);
		}
		_skyboxRenderTarget->skbModel->unbind();
		glDepthMask(GL_TRUE);
	}


private:
	SHADER::ShaderObj* _shaderObj;
};

#endif

/*
		void setUniformModelMatrix(glm::mat4 &modelMatrix) {
			m_modelMatrix = modelMatrix;
		}
		void setUniformModelMatrixWithDivide(glm::mat4 &modelMatrix, float div) {
			m_modelMatrix = modelMatrix;
			m_modelMatrix[3][0] /= div;
			m_modelMatrix[3][1] /= div;
			m_modelMatrix[3][2] /= div;
		}

		void setUniformViewMatrix(glm::mat4 &viewMatrix) {
			m_viewMatrix = viewMatrix;
		}
		void setUniformProjectionMatrix(glm::mat4 &projectionMatrix) {
			m_ProjectionMatrix = projectionMatrix;
		}

		~SkyboxObjManager() {
			for (int i = 0; i < skyboxObjStorageVec.size(); i++) {
				delete skyboxObjStorageVec[i];
			}
			for (int i = 0; i < shaderSkyboxVec.size(); i++) {
				delete shaderSkyboxVec[i];
			}
		}
		*/
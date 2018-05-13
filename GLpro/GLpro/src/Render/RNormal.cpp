#include "./RNormal.h"
#include "../Camera/Camera.h"
#include "../RenderTarget/Normal/NormalFObj.h"
#include "../Shader/ShaderMain.h"
#include "../../LightManager.h"
#include "../Camera/CameraManager.h"

namespace RENDER
{
	RNormal::RNormal(SHADER::ShaderMain * shaderObj, float div)
	{
		_shaderObj = shaderObj;
	}

	void RNormal::addToDrawList(RENDER_TARGET::NORMAL::NormalFObj * skyboxFObj)
	{
		
	}

	void RNormal::draw()
	{
		glCullFace(GL_BACK);
		_shaderObj->bind();

		glm::mat4 modelMatrix = _targetCamera->getCamModelMatRef;
		glm::mat4 depthBiasMVP = GLightManager->directionalLightVec[0].GetDepthBiasMVP();
		// move skybox shorter than real position
		modelMatrix[3][0] /= _div;
		modelMatrix[3][1] /= _div;
		modelMatrix[3][2] /= _div;

		_shaderObj->loadVector3(_shaderObj->m_lightInvDirID, GLightManager->directionalLightVec[0].GetLightVec3());
		_shaderObj->loadMatrix4(_shaderObj->m_cameraViewMatrixID, GCameraManager->GetMainCamera()->getCamViewMatRef());
		_shaderObj->loadMatrix4(_shaderObj->m_depthBiasID, GLightManager->directionalLightVec[0].GetDepthBiasMVP());

		glUniformMatrix4fv(shaderMainPtr->m_cameraViewMatrixID, 1, GL_FALSE, &m_viewMatrix[0][0]);
		glUniformMatrix4fv(shaderMainPtr->m_depthBiasID, 1, GL_FALSE, &depthBiasMVP[0][0]);

		glm::mat4 tempMVP = _targetCamera->getCamProjMatRef() * _targetCamera->getCamViewMatRef() * modelMatrix;
		_shaderObj->loadMatrix4(_shaderObj->m_modelMatrixID, modelMatrix);
		_shaderObj->loadMatrix4(_shaderObj->m_cameraViewMatrixID, _targetCamera->getCamViewMatRef());
		//_shaderObj->loadMatrix4(_shaderObj->m_cameraViewMatrixID, _camViewMatrix);
		_shaderObj->loadMatrix4(_shaderObj->m_MVPMatrixID, tempMVP);

		glDepthMask(GL_FALSE);

		for (int i = 0; i < _normalFobjcetContainer.size(); i++) {
			RENDER_TARGET::NORMAL::NormalFObj* normalRenderTarget = _normalFobjcetContainer[i];

			normalRenderTarget->_model->bind();	// Model bind

			glActiveTexture(GL_TEXTURE0);
			normalRenderTarget->_texture->bind();		// Texture bind

			_shaderObj->loadInt(_shaderObj->m_textureID, 0);	//glUniform1i(shaderSkyboxVec[selectedSkyboxShaderIdx]->m_textureID, 0);
			glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
			normalRenderTarget->_texture->unbind(i);

			normalRenderTarget->_model->unbind();
		}
		glDepthMask(GL_TRUE);
		_shaderObj->unbind();

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
#include "stdafx.h"
#include "LightManager.h"
#include "DirectionalLightManager.h"
#include "SpotLightManager.h"
#include "PointLightManager.h"
#include "DeferredPointLightManager.h"

LightManager::LightManager()
{
	_directionalLightManager = new DirectionalLightManager();
	_directionalLightManager->initBuffer();

	_spotLightManager = new SpotLightManager();
	_spotLightManager->initBuffer();

	_pointLightManager = new PointLightManager();
	_pointLightManager->initBuffer();
	
	_deferredPointLightManager = new DeferredPointLightManager();
	_deferredPointLightManager->initBuffer();
}

void LightManager::AddDirectinalLight(glm::vec3 & dir, glm::vec4 & color)
{
	_directionalLightManager->addNewDirectionalLight(color, dir);
}

void LightManager::AddSpotLightManager(glm::mat4 & rotMat, glm::vec3 & pos, glm::vec4 & color, float lightPower, float fovy, float aspcect, float persNear, float persFar)
{
	_spotLightManager->addNewLight(_bindedGSession, rotMat, pos, color, lightPower, fovy, aspcect, persNear, persFar);
}

void LightManager::AddPointLightManager(glm::vec3 & pos, glm::vec4 & color, float lightPower)
{
	_pointLightManager->addNewLight(_bindedGSession, pos, color, lightPower);
}

void LightManager::AddDeferredPointLight(glm::vec3 & pos, glm::vec4 & color, float lightPower)
{
	_deferredPointLightManager->addNewLight(_bindedGSession, pos, color, lightPower);
}

void LightManager::updateAllLIghts()
{
	GLightManager->_directionalLightManager->updateLightsToBufferData();
	GLightManager->_directionalLightManager->updateBufferToVGA();

	GLightManager->_spotLightManager->updateLightsToBufferData();
	GLightManager->_spotLightManager->updateBufferToVGA();

	GLightManager->_pointLightManager->updateLightsToBufferData();
	GLightManager->_pointLightManager->updateBufferToVGA();

	GLightManager->_deferredPointLightManager->updateLightsToBufferData();
	GLightManager->_deferredPointLightManager->updateBufferToVGA();
}

LightManager* GLightManager = nullptr;
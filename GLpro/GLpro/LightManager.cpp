#include "stdafx.h"
#include "LightManager.h"
#include "DirectionalLightManager.h"
#include "SpotLightManager.h"
#include "PointLightManager.h"
#include "DeferredPointLightManager.h"

void LightManager::AddDirectinalLight(glm::vec3 & dir, glm::vec4 & color)
{
	_directionalLightManager->addNewDirectionalLight(color, dir);
}

void LightManager::AddSpotLightManager(glm::vec3 & dir, glm::vec3 & pos, glm::vec4 & color, float lightPower)
{
	_spotLightManager->addNewLight(_bindedGSession, dir, pos, color, lightPower);
}

void LightManager::AddPointLightManager(glm::vec3 & pos, glm::vec4 & color, float lightPower)
{
	_pointLightManager->addNewLight(_bindedGSession, glm::vec3(), pos, color, lightPower);
}

void LightManager::AddDeferredPointLight(glm::vec3 & pos, glm::vec4 & color, float lightPower)
{
	_deferredPointLightManager->addNewLight(_bindedGSession, glm::vec3(), pos, color, lightPower);
}

LightManager* GLightManager = nullptr;
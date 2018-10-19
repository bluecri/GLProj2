#include "stdafx.h"
#include "LightManager.h"
#include "DirectionalLightManager.h"
#include "SpotLightManager.h"
#include "PointLightManager.h"
#include "DeferredPointLightManager.h"

#include "DirectionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "DeferredPointLight.h"

#include "RenderManager.h"
#include "src/Render/RNormal.h"
#include "OctreeForFrustum.h"


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

std::shared_ptr<LightWithEntity> LightManager::AddSpotLightManager(glm::mat4 & rotMat, glm::vec3 & pos, glm::vec4 & color, float lightPower, float fovy, float aspcect, float persNear, float persFar)
{
	return _spotLightManager->addNewLight(_bindedGSession, rotMat, pos, color, lightPower, fovy, aspcect, persNear, persFar);
}

std::shared_ptr<LightWithEntity> LightManager::AddPointLightManager(glm::vec3 & pos, glm::vec4 & color, float lightPower)
{
	return _pointLightManager->addNewLight(_bindedGSession, pos, color, lightPower);
}

std::shared_ptr<LightWithEntity> LightManager::AddDeferredPointLight(glm::vec3 & pos, glm::vec4 & color, float lightPower)
{
	return _deferredPointLightManager->addNewLight(_bindedGSession, pos, color, lightPower);
}

void LightManager::updateAllLIghts()
{
	GLightManager->_directionalLightManager->updateLights();
	GLightManager->_directionalLightManager->updateLightsToBufferData();
	GLightManager->_directionalLightManager->updateBufferToVGA();

	GLightManager->_spotLightManager->updateLights();
	GLightManager->_spotLightManager->updateLightsToBufferData();
	GLightManager->_spotLightManager->updateBufferToVGA();

	GLightManager->_pointLightManager->updateLights();
	GLightManager->_pointLightManager->updateLightsToBufferData();
	GLightManager->_pointLightManager->updateBufferToVGA();

	GLightManager->_deferredPointLightManager->updateLights();
	GLightManager->_deferredPointLightManager->updateLightsToBufferData();
	GLightManager->_deferredPointLightManager->updateBufferToVGA();


	// Light - object relationship
	auto& rendererMap = GRendermanager->getRendererContainer<RENDER::RNormal>();

	for (auto elem : _directionalLightManager->getLightVec())
	{
		if (!elem->isLightOn())
		{
			continue;
		}

		//for (auto& rendererElem : rendererMap)
		{
			//GOctreeForFrustum->getObjListWithFrustumOb(elem->getFrustumObRef(), elem->getFrustumedDrawElementContainerRef());
		}
	}

	for (auto elem : _spotLightManager->getLightVec())
	{
		if (!elem->isLightOn())
		{
			continue;
		}

		//for (auto& rendererElem : rendererMap)
		{
			GOctreeForFrustum->getObjListWithFrustumOb(std::static_pointer_cast<SpotLight>(elem)->getFrustumObRef(), elem->getFrustumedDrawElementContainerRef());
		}
	}

	for (auto elem : _pointLightManager->getLightVec())
	{
		if (!elem->isLightOn())
		{
			continue;
		}

		//for (auto& rendererElem : rendererMap)
		{
			GOctreeForFrustum->getObjListWithAABBOb(std::static_pointer_cast<PointLight>(elem)->getAABBObRef(), elem->getFrustumedDrawElementContainerRef());
		}
	}

}

void LightManager::deUpdateAllLIghts()
{
	// clear FrustumedDrawElementContainer
	for (auto elem : _directionalLightManager->getLightVec())
	{
		elem->clearFrustumedDrawElementContainer();
	}

	for (auto elem : _spotLightManager->getLightVec())
	{
		elem->clearFrustumedDrawElementContainer();
	}

	for (auto elem : _pointLightManager->getLightVec())
	{
		elem->clearFrustumedDrawElementContainer();
	}
}

LightManager* GLightManager = nullptr;
#pragma once

#include "stdafx.h"

class GameSession;

class DirectionalLightManager;
class SpotLightManager;
class PointLightManager;
class DeferredPointLightManager;
class LightWithEntity;

typedef std::shared_ptr<LightWithEntity> SharedLightWithEntity;

class LightManager
{
public:
	LightManager();
	~LightManager() {};

	void AddDirectinalLight(glm::vec3 & dir, glm::vec4 & color);
	SharedLightWithEntity AddSpotLightManager(glm::mat4 & rotMat, glm::vec3 & pos, glm::vec4 & color, float lightPower, float fovy = 15.0f, float aspcect = 1.0f, float persNear = 0.1f, float persFar = 20.0f);
	SharedLightWithEntity AddPointLightManager(glm::vec3 & pos, glm::vec4 & color, float lightPower);
	SharedLightWithEntity AddDeferredPointLight(glm::vec3 & pos, glm::vec4 & color, float lightPower);

	void updateAllLIghts();
public:
	DirectionalLightManager*	_directionalLightManager;
	SpotLightManager*			_spotLightManager;
	PointLightManager*			_pointLightManager;
	DeferredPointLightManager*	_deferredPointLightManager;

private:
	GameSession*				_bindedGSession;

};

extern LightManager* GLightManager;
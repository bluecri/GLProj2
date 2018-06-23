#pragma once

#include "stdafx.h"

class GameSession;

class DirectionalLightManager;
class SpotLightManager;
class PointLightManager;
class DeferredPointLightManager;

class LightManager
{
public:
	LightManager() {};
	~LightManager() {};

	void AddDirectinalLight(glm::vec3 & dir, glm::vec4 & color);
	void AddSpotLightManager(glm::vec3 & dir, glm::vec3 & pos, glm::vec4 & color, float lightPower);
	void AddPointLightManager(glm::vec3 & pos, glm::vec4 & color, float lightPower);
	void AddDeferredPointLight(glm::vec3 & pos, glm::vec4 & color, float lightPower);

public:
	DirectionalLightManager*	_directionalLightManager;
	SpotLightManager*			_spotLightManager;
	PointLightManager*			_pointLightManager;
	DeferredPointLightManager*	_deferredPointLightManager;

private:
	GameSession*				_bindedGSession;

};

extern LightManager* GLightManager;
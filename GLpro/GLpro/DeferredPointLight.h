#pragma once
/*
#include "light.h"
#include "configs_ubo.h"

class DeferredPointLightManager;
class GameSession;
class LightGameObject;

class DeferredPointLight : public Light, public std::enable_shared_from_this<DeferredPointLight>
{
public:
	mat4&	getBiasMat();
	vec3&	getWorldPos();
	float	getLightPower();
	bool	isValid();

private:
	DeferredPointLight(DeferredPointLightManager* bindedLightManager, GameSession* gSession, int idx, glm::vec3& pos, glm::vec4& color, float lightPower);
	void lightInit();
	DeferredPointLightManager*	getBindedLightManager();
	void destroyLight();

private:
	LightGameObject*			_bindedLightGameObject;
	DeferredPointLightManager*	_bindedLightManager;
	int							_idx;
	bool						_isValid;

	float	_lightPower;
	mat4	_biasMatrix;
	int		_uboType;

public:
	friend class DeferredPointLightManager;
};
*/
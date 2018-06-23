#pragma once

#include "light.h"

class ILightWithEntityManager;
class GameSession;
class LightGameObject;

class LightWithEntity : public Light, public std::enable_shared_from_this<LightWithEntity>
{
public:
	
	LightWithEntity(ILightWithEntityManager* bindedLightManager, GameSession* gSession, int uboType, int idx, glm::vec3& dir, glm::vec3& pos, glm::vec4& color, float lightPower);
	void lightInit();
	void destroyLight();

	bool	isValid();

	glm::mat4&	getBiasMat();
	glm::vec3&	getWorldPos();
	float		getLightPower();
	int			getUboType();
	glm::vec3	getLightDir();

	ILightWithEntityManager*	getBindedLightManager();

private:
	LightGameObject*			_bindedLightGameObject;
	ILightWithEntityManager*	_bindedLightManager;
	int							_idx;
	bool						_isValid;

	float		_lightPower;
	glm::mat4	_biasMatrix;
	glm::vec3	_lightDir;
	int			_uboType;

public:
	friend class ILightWithEntityManager;
};
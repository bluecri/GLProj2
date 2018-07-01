#pragma once

#include "light.h"

class ILightWithEntityManager;
class GameSession;
class LightGameObject;

class LightWithEntity : public Light, public std::enable_shared_from_this<LightWithEntity>
{
public:
	
	LightWithEntity(ILightWithEntityManager* bindedLightManager, GameSession* gSession, int uboType, int idx, glm::mat4& rotMat, glm::vec3& pos, glm::vec4& color, float lightPower);
	void		lightInit();
	void		destroyLight();

	bool		isValid();

	virtual void	lightUpdate() = 0;

	glm::vec3	getWorldPos();
	float		getLightPower();
	int			getUboType();
	LightGameObject*	getLightGameObject();

	ILightWithEntityManager*	getBindedLightManager();

protected:
	glm::mat4	getWorldMat();

protected:
	LightGameObject*			_bindedLightGameObject;
	ILightWithEntityManager*	_bindedLightManager;
	int			_idx;
	bool		_isValid;

	float		_lightPower;
	int			_uboType;


public:
	friend class ILightWithEntityManager;
};
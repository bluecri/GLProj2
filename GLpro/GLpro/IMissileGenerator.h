#pragma once

#include "EnumMissileType.h"

class MissileGeneratorStorage;
class CommonMissileState;
class ISpecifiedMissileState;

class Entity;
class Transform;

class IMissileGenerator
{
public:
	IMissileGenerator(MissileGeneratorStorage* bindedMissileGeneratorStorage, Entity* bindedEntity)
		: _bindedMissileGeneratorStorage(bindedMissileGeneratorStorage), _bindedEntity(bindedEntity)
	{
		_entityTransform = _bindedEntity->_rigidbodyComponent->_transform;
	}
	virtual void genMissile() = 0;
	virtual void updateTimer(float deltaTime, float acc) = 0;

	CommonMissileState*		getCommonMissileState();
	ENUM_MISSILE_TYPE*		getENUM_MISSILE_TYPE();
	ISpecifiedMissileState* getISpecifiedMissileState();
	Entity*					getBindedEntity();
	
public:
	float _curMissileDelay;

protected:
	MissileGeneratorStorage* _bindedMissileGeneratorStorage;

	ENUM_MISSILE_TYPE	 _missileType;
	CommonMissileState*	 _commonMissileState;
	ISpecifiedMissileState*	 _specifiedMissileState;

	Entity* _bindedEntity;
	Transform* _entityTransform;
};
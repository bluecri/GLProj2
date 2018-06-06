#pragma once

#include "EnumMissileType.h"

class MissileGeneratorStorage;
class CommonMissileState;
class ISpecifiedMissileState;

class Entity;
class RigidbodyComponent;

class IMissileGenerator
{
public:
	IMissileGenerator();
	virtual void genMissile() = 0;
	virtual void updateTimer(float deltaTime, float acc) = 0;

	void setBindedEntityAndMissileGenerator(Entity* entity, MissileGeneratorStorage* bindedMissileGeneratorStorage);

	CommonMissileState*		getCommonMissileState();
	ENUM_MISSILE_TYPE		getENUM_MISSILE_TYPE();
	ISpecifiedMissileState* getISpecifiedMissileState();
	Entity*					getBindedEntity();
	
private:
	void setBindedEntity(Entity* entity);
	void setBindedMissileGenerator(MissileGeneratorStorage* bindedMissileGeneratorStorage);

public:
	float _curMissileDelay;

protected:
	MissileGeneratorStorage* _bindedMissileGeneratorStorage;

	ENUM_MISSILE_TYPE	 _missileType;
	CommonMissileState*	 _commonMissileState;
	ISpecifiedMissileState*	 _specifiedMissileState;

	Entity* _bindedEntity;
	RigidbodyComponent* _entityRigidbodyComponent;
};
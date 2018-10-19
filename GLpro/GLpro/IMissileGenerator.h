#pragma once

#include "EnumMissileType.h"

class MissileGeneratorStorage;
class CommonMissileState;
class ISpecifiedMissileState;
class ICommonMissileInfo;
class ISpecifiedMissileInfo;

class Entity;
class RigidbodyComponent;
class BuffSum;

class IMissileGenerator
{
public:
	IMissileGenerator(ISpecifiedMissileState * allocedCurState, ISpecifiedMissileState * allocedOriginState);
	virtual void genMissile() = 0;
	virtual void updateTimer(float deltaTime, float acc) = 0;
	virtual void transferBuffSum(BuffSum* buffSum) = 0;


	void initBindedEntityAndMissileGenerator(Entity* entity, MissileGeneratorStorage* bindedMissileGeneratorStorage);
	void initState(ICommonMissileInfo* iCommonMissileInfo, ISpecifiedMissileInfo* iSpecifiedMissileInfo);

	ENUM_MISSILE_TYPE		getENUM_MISSILE_TYPE();
	CommonMissileState*		getOriginCommonMissileState();
	CommonMissileState*		getCurCommonMissileState();
	ISpecifiedMissileState* getOriginISpecifiedMissileState();
	ISpecifiedMissileState* getCurISpecifiedMissileState();
	Entity*					getBindedEntity();
	

protected:
	MissileGeneratorStorage* _bindedMissileGeneratorStorage;

	ENUM_MISSILE_TYPE			_missileType;
	CommonMissileState*			_originCommonMissileState;
	CommonMissileState*			_curCommonMissileState;
	ISpecifiedMissileState*		_originSpecifiedMissileState;
	ISpecifiedMissileState*		_curSpecifiedMissileState;

	Entity*						_bindedEntity;
	RigidbodyComponent*			_entityRigidbodyComponent;

	float						_timerMissileDelay;
};
#include "stdafx.h"
#include "IMissileGenerator.h"

#include "./src/Entity.h"
#include "RigidbodyComponent.h"
#include "ICommonMissileInfo.h"
#include "CommonMissileState.h"
#include "ISpecifiedMissileState.h"
#include "ISpecifiedMissileInfo.h"


IMissileGenerator::IMissileGenerator(ISpecifiedMissileState* allocedCurState, ISpecifiedMissileState* allocedOriginState)
{
	_bindedEntity = nullptr;
	_entityRigidbodyComponent = nullptr;
	_bindedMissileGeneratorStorage = nullptr;

	_originCommonMissileState = new CommonMissileState();
	_curCommonMissileState = new CommonMissileState();

	_originSpecifiedMissileState = allocedOriginState;
	_curSpecifiedMissileState = allocedCurState;
}

void IMissileGenerator::initBindedEntityAndMissileGenerator(Entity * entity, MissileGeneratorStorage * bindedMissileGeneratorStorage)
{
	_bindedEntity = entity;
	_entityRigidbodyComponent = _bindedEntity->getRigidbodyComponent();
	_bindedMissileGeneratorStorage = bindedMissileGeneratorStorage;
}

void IMissileGenerator::initState(ICommonMissileInfo * iCommonMissileInfo, ISpecifiedMissileInfo * iSpecifiedMissileInfo)
{
	_originCommonMissileState->initCommonState(iCommonMissileInfo);
	_curCommonMissileState->initCommonState(iCommonMissileInfo);

	_originCommonMissileState = new CommonMissileState();
	_originCommonMissileState = new CommonMissileState();

	_originSpecifiedMissileState->initSpecifiedState(iSpecifiedMissileInfo);
	_curSpecifiedMissileState->initSpecifiedState(iSpecifiedMissileInfo);
}


ENUM_MISSILE_TYPE IMissileGenerator::getENUM_MISSILE_TYPE() { return _missileType; }

CommonMissileState * IMissileGenerator::getOriginCommonMissileState()
{
	return _originCommonMissileState;
}

CommonMissileState * IMissileGenerator::getCurCommonMissileState()
{
	return _curCommonMissileState;
}

ISpecifiedMissileState * IMissileGenerator::getOriginISpecifiedMissileState()
{
	return _originSpecifiedMissileState;
}

ISpecifiedMissileState * IMissileGenerator::getCurISpecifiedMissileState()
{
	return _curSpecifiedMissileState;
}


Entity * IMissileGenerator::getBindedEntity() { return _bindedEntity; }

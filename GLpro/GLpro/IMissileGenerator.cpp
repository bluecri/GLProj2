#include "stdafx.h"
#include "IMissileGenerator.h"
#include "./src/Entity.h"
#include "RigidbodyComponent.h"


IMissileGenerator::IMissileGenerator(MissileGeneratorStorage * bindedMissileGeneratorStorage, Entity * bindedEntity)
	: _bindedMissileGeneratorStorage(bindedMissileGeneratorStorage), _bindedEntity(bindedEntity)
{
	_entityTransform = _bindedEntity->_rigidbodyComponent->_transform;
}

CommonMissileState * IMissileGenerator::getCommonMissileState() { return _commonMissileState; }

ENUM_MISSILE_TYPE IMissileGenerator::getENUM_MISSILE_TYPE() { return _missileType; }

ISpecifiedMissileState * IMissileGenerator::getISpecifiedMissileState() { return _specifiedMissileState; }

Entity * IMissileGenerator::getBindedEntity() { return _bindedEntity; }

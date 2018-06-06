#include "stdafx.h"
#include "IMissileGenerator.h"
#include "./src/Entity.h"
#include "RigidbodyComponent.h"


IMissileGenerator::IMissileGenerator()
{
	_bindedEntity = nullptr;
	_entityRigidbodyComponent = nullptr;
	_bindedMissileGeneratorStorage = nullptr;
}

void IMissileGenerator::setBindedEntity(Entity * entity)
{
	_bindedEntity = entity;
	_entityRigidbodyComponent = _bindedEntity->_rigidbodyComponent;
}

void IMissileGenerator::setBindedMissileGenerator(MissileGeneratorStorage * bindedMissileGeneratorStorage)
{
	_bindedMissileGeneratorStorage = bindedMissileGeneratorStorage;
}

void IMissileGenerator::setBindedEntityAndMissileGenerator(Entity * entity, MissileGeneratorStorage * bindedMissileGeneratorStorage)
{
	setBindedEntity(entity);
	setBindedMissileGenerator(bindedMissileGeneratorStorage);
}

CommonMissileState * IMissileGenerator::getCommonMissileState() { return _commonMissileState; }

ENUM_MISSILE_TYPE IMissileGenerator::getENUM_MISSILE_TYPE() { return _missileType; }

ISpecifiedMissileState * IMissileGenerator::getISpecifiedMissileState() { return _specifiedMissileState; }

Entity * IMissileGenerator::getBindedEntity() { return _bindedEntity; }

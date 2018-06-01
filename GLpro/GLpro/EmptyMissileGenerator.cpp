#include "stdafx.h"
#include "EmptyMissileGenerator.h"

EmptyMissileGenerator::EmptyMissileGenerator() : IMissileGenerator()
{
	_missileType = ENUM_MISSILE_TYPE::ENUM_MISSILE_TYPE_EMPTY;
}

void EmptyMissileGenerator::init(Entity * bindedEntity, MissileGeneratorStorage * missileGeneratorStorage)
{
	setBindedEntityAndMissileGenerator(bindedEntity, missileGeneratorStorage);
}

void EmptyMissileGenerator::genMissile()
{
	return;
}

void EmptyMissileGenerator::updateTimer(float deltaTime, float acc)
{
	return;
}
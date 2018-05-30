#include "stdafx.h"
#include "EmptyMissileGenerator.h"

EmptyMissileGenerator::EmptyMissileGenerator(MissileGeneratorStorage * _missileGeneratorStorage, Entity * bindedEntity)
	: IMissileGenerator(_missileGeneratorStorage, bindedEntity)
{
	_missileType = ENUM_MISSILE_TYPE::ENUM_MISSILE_TYPE_EMPTY;
}

void EmptyMissileGenerator::genMissile()
{
	return;
}

void EmptyMissileGenerator::updateTimer(float deltaTime, float acc)
{
	return;
}
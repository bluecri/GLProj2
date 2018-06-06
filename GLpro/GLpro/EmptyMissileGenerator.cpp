#include "stdafx.h"
#include "EmptyMissileGenerator.h"

EmptyMissileGenerator::EmptyMissileGenerator() : IMissileGenerator(nullptr, nullptr)
{
	_missileType = ENUM_MISSILE_TYPE::ENUM_MISSILE_TYPE_EMPTY;
}

void EmptyMissileGenerator::init(Entity * bindedEntity, MissileGeneratorStorage * missileGeneratorStorage)
{
	initBindedEntityAndMissileGenerator(bindedEntity, missileGeneratorStorage);
}

void EmptyMissileGenerator::genMissile()
{
	return;
}

void EmptyMissileGenerator::updateTimer(float deltaTime, float acc)
{
	return;
}

void EmptyMissileGenerator::modifyCurMissileStateWithBuffInfo(BuffInfo * buffInfo)
{
}

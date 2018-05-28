#include "stdafx.h"
#include "MissileGeneratorStorage.h"
#include "IMissileGenerator.h"

void MissileGeneratorStorage::addMissileGenerator(IMissileGenerator * missileGenerator)
{
	_missileGeneratorVec.push_back(missileGenerator);
}

void MissileGeneratorStorage::update(float deltaTime, float acc)
{
	for (int i = 0; i < _missileGeneratorVec.size(); i++)
		_missileGeneratorVec[i]->updateTimer(deltaTime, acc);
}

void MissileGeneratorStorage::shotMissile(int missileIndex)
{
	if(missileIndex >= 0 && missileIndex < _missileGeneratorVec.size())
		_missileGeneratorVec[missileIndex]->genMissile();
}

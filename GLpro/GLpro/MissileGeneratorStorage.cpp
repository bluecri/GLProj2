#include "stdafx.h"
#include "MissileGeneratorStorage.h"
#include "IMissileGenerator.h"
#include "EmptyMissileGenerator.h"
#include "BuffSum.h"

MissileGeneratorStorage::MissileGeneratorStorage(int maxWeaponCount, Entity* bindedEntity)
	:_maxWeaponCount(maxWeaponCount), _bindedEntity(bindedEntity)
{
	_selectedWeaponIndex = 0;
	_bShotDisable = true;
	for (int i = 0; i < _maxWeaponCount; i++)
	{
		EmptyMissileGenerator* emptyGenerator = new EmptyMissileGenerator();
		emptyGenerator->init(bindedEntity, this);
		_missileGeneratorVec.push_back(emptyGenerator);
	}
}

bool MissileGeneratorStorage::addMissileGenerator(IMissileGenerator * missileGenerator)
{
	for (int i = 0; i < _maxWeaponCount; i++)
	{
		if (_missileGeneratorVec[i]->getENUM_MISSILE_TYPE() == ENUM_MISSILE_TYPE::ENUM_MISSILE_TYPE_EMPTY)
		{
			delete _missileGeneratorVec[i];
			_missileGeneratorVec[i] = missileGenerator;
			return true;
		}
	}

	return false;
}

void MissileGeneratorStorage::update(float deltaTime, float acc)
{
	for (int i = 0; i < _missileGeneratorVec.size(); i++)
		_missileGeneratorVec[i]->updateTimer(deltaTime, acc);
}

void MissileGeneratorStorage::shotMissile()
{
	_missileGeneratorVec[_selectedWeaponIndex]->genMissile();
}

void MissileGeneratorStorage::selectMissileIndex(int idx)
{
	if (0 <= idx && idx < _maxWeaponCount)
	{
		_selectedWeaponIndex = idx;
	}
}

void MissileGeneratorStorage::transferBuffSum(BuffSum * buffSum)
{
	_bShotDisable = buffSum->bOnce[ENUM_BUFFSUM_ONCE_SHOT_DISABLE];
	for (int i = 0; i < _maxWeaponCount; i++)
	{
		_missileGeneratorVec[_selectedWeaponIndex]->transferBuffSum(buffSum);
	}
}

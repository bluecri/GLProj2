#include "stdafx.h"
#include "MissileGeneratorStorage.h"
#include "IMissileGenerator.h"
#include "EmptyMissileGenerator.h"

MissileGeneratorStorage::MissileGeneratorStorage(int maxWeaponCount, Entity* bindedEntity)
	:_maxWeaponCount(maxWeaponCount), _bindedEntity(bindedEntity)
{
	_selectedWeaponIndex = 0;
	for (int i = 0; i < maxWeaponCount; i++)
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

inline void MissileGeneratorStorage::selectMissileIndex(int idx)
{
	if (0 <= idx && idx < _maxWeaponCount)
	{
		_selectedWeaponIndex = idx;
	}
}

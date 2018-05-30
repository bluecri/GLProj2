#include "stdafx.h"
#include "SpecifiedNormalMissileState.h"

#include "EnumMissileType.h"
#include "SpecifiedNormalMissileInfo.h"

SpecifiedNormalMissileState::SpecifiedNormalMissileState()
{
	_dmg = 10;
	_hitCount = 1;
	_hitInterval = 1.0f;
	_firstSpeed = 4.0f;
	_deltaSpeed = 0.8f;
	_mass = 1.0f;
	_maxSpeed = 10.0f;

	_missileShotSoundStr = std::string("laser");
	_missileHitSoundStr = std::string("hit");
	_missileGenAddPos = glm::vec3(0.0f, 0.0f, 0.2f);

	_curDmg = _dmg;
	_curHitCount = _hitCount;
	_curHitInterval = _hitInterval;
	_curFirstSpeed = _firstSpeed;
	_curDeltaSpeed = _deltaSpeed;
	_curLifeTime = _lifeTime;
	_curMass = _mass;
	_curMaxSpeed = _maxSpeed;
}

int SpecifiedNormalMissileState::getType()
{
	return ENUM_MISSILE_TYPE::ENUM_MISSILE_TYPE_NORMAL;
}

int SpecifiedNormalMissileState::init(SpecifiedNormalMissileInfo * specifiedMissileInfo)
{
	_dmg = specifiedMissileInfo->_dmg;
	_hitCount = specifiedMissileInfo->_hitCount;
	_hitInterval = specifiedMissileInfo->_hitInterval;
	_firstSpeed = specifiedMissileInfo->_firstSpeed;
	_deltaSpeed = specifiedMissileInfo->_deltaSpeed;
	_lifeTime = specifiedMissileInfo->_lifeTime;
	_mass = specifiedMissileInfo->_mass;
	_maxSpeed = specifiedMissileInfo->_maxSpeed;

	_curDmg = _dmg;
	_curHitCount = _hitCount;
	_curHitInterval = _hitInterval;
	_curFirstSpeed = _firstSpeed;
	_curDeltaSpeed = _deltaSpeed;
	_curLifeTime = _lifeTime;
	_curMass = _mass;
	_curMaxSpeed = _maxSpeed;

	return 0;
}


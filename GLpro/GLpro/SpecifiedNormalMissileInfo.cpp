#include "stdafx.h"
#include "SpecifiedNormalMissileInfo.h"
#include "SpecifiedNormalMissileState.h"

void SpecifiedNormalMissileInfo::initISpecifiedMissileInfo()
{
	_dmg = 10;
	_hitCount = 1;
	_hitInterval = 1.0f;

	_maxSpeed = 10.0f;
	_firstSpeed = 10.0f;
	_deltaSpeed = 0.8f;

	_lifeTime = 4.0f;
	_mass = 1.0f;

	_missileShotSoundStr = std::string("laser");
	_missileHitSoundStr = std::string("hit");

	_missileGenAddPos = glm::vec3(0.0f, 0.0f, 0.2f);
}

void SpecifiedNormalMissileInfo::initISpecifiedMissileInfo(std::string & specifiedMissileInfo)
{
	// read file & get info
}

void SpecifiedNormalMissileInfo::doubleDispatchInit(SpecifiedNormalMissileState * normaState)
{
	normaState->_dmg			= _dmg;
	normaState->_hitCount		= _hitCount;
	normaState->_hitInterval	= _hitInterval;
	normaState->_firstSpeed		= _firstSpeed;
	normaState->_deltaSpeed		= _deltaSpeed;
	normaState->_lifeTime		= _lifeTime;
	normaState->_mass			= _mass;
	normaState->_maxSpeed		= _maxSpeed;
	normaState->_missileShotSoundStr	= _missileShotSoundStr;
	normaState->_missileHitSoundStr		= _missileHitSoundStr;
	normaState->_missileGenAddPos		= _missileGenAddPos;
}

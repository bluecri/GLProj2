#include "stdafx.h"
#include "SpecifiedNormalMissileState.h"

#include "EnumMissileType.h"
#include "SpecifiedNormalMissileInfo.h"

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

	_curDmg = _dmg;
	_curHitCount = _hitCount;
	_curHitInterval = _hitInterval;
	_curFirstSpeed = _firstSpeed;
	_curDeltaSpeed = _deltaSpeed;
	_curLifeTime = _lifeTime;
	
	return 0;
}


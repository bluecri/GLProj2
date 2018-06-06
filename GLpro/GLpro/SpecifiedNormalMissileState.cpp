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
}

int SpecifiedNormalMissileState::getType()
{
	return ENUM_MISSILE_TYPE::ENUM_MISSILE_TYPE_NORMAL;
}
void SpecifiedNormalMissileState::initSpecifiedState(ISpecifiedMissileInfo * info)
{
	info->doubleDispatchInit(this);
	return;
}


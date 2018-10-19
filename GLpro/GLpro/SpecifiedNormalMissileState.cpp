#include "stdafx.h"
#include "SpecifiedNormalMissileState.h"

#include "EnumMissileType.h"
#include "SpecifiedNormalMissileInfo.h"
#include "BuffSum.h"

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

void SpecifiedNormalMissileState::transferBuffSum(BuffSum * buffSum, ISpecifiedMissileState * originState)
{
	SpecifiedNormalMissileState* specifiedOriginState = static_cast<SpecifiedNormalMissileState*>(originState);
	
	if (buffSum->bModify[ENUM_BUFFSUM_MODIFY_SHOT_DMG])
	{
		_dmg = specifiedOriginState->_dmg * buffSum->mult[ENUM_BUFFSUM_MODIFY_SHOT_DMG]
			+ buffSum->acc[ENUM_BUFFSUM_MODIFY_SHOT_DMG];
	}

	if (buffSum->bModify[ENUM_BUFFSUM_MODIFY_SHOT_SPEED])
	{
		_firstSpeed = specifiedOriginState->_firstSpeed * buffSum->mult[ENUM_BUFFSUM_MODIFY_SHOT_SPEED]
			+ buffSum->acc[ENUM_BUFFSUM_MODIFY_SHOT_SPEED];
	}

	if (buffSum->bModify[ENUM_BUFFSUM_MODIFY_SHOT_MAXSPEED])
	{
		_maxSpeed = specifiedOriginState->_maxSpeed * buffSum->mult[ENUM_BUFFSUM_MODIFY_SHOT_MAXSPEED]
			+ buffSum->acc[ENUM_BUFFSUM_MODIFY_SHOT_MAXSPEED];
	}

	if (buffSum->bModify[ENUM_BUFFSUM_MODIFY_SHOT_HITCOUNT])
	{
		_hitCount = specifiedOriginState->_hitCount * buffSum->mult[ENUM_BUFFSUM_MODIFY_SHOT_HITCOUNT]
			+ buffSum->acc[ENUM_BUFFSUM_MODIFY_SHOT_HITCOUNT];
	}

	if (buffSum->bModify[ENUM_BUFFSUM_MODIFY_SHOT_HITINTERVAL])
	{
		_hitInterval = specifiedOriginState->_hitInterval * buffSum->mult[ENUM_BUFFSUM_MODIFY_SHOT_HITINTERVAL]
			+ buffSum->acc[ENUM_BUFFSUM_MODIFY_SHOT_HITINTERVAL];
	}

	if (buffSum->bModify[ENUM_BUFFSUM_MODIFY_SHOT_LIFETIME])
	{
		_lifeTime = specifiedOriginState->_lifeTime * buffSum->mult[ENUM_BUFFSUM_MODIFY_SHOT_LIFETIME]
			+ buffSum->acc[ENUM_BUFFSUM_MODIFY_SHOT_LIFETIME];
	}

	if (buffSum->bModify[ENUM_BUFFSUM_MODIFY_SHOT_MASS])
	{
		_mass = specifiedOriginState->_mass * buffSum->mult[ENUM_BUFFSUM_MODIFY_SHOT_MASS]
			+ buffSum->acc[ENUM_BUFFSUM_MODIFY_SHOT_MASS];
	}
}


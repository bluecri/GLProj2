#include "stdafx.h"
#include "SpecifiedNormalMissileInfo.h"

void SpecifiedNormalMissileInfo::init()
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

void SpecifiedNormalMissileInfo::init(std::string & specifiedMissileInfo)
{
	// read file & get info
}
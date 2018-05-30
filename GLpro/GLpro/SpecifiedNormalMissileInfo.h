#pragma once
#include "stdafx.h"

#include "ISpecifiedMissileInfo.h"

class SpecifiedNormalMissileInfo :public ISpecifiedMissileInfo
{
public:
	SpecifiedNormalMissileInfo()
	{
	}

	// ISpecifiedMissileInfo을(를) 통해 상속됨
	virtual void init() override;
	virtual void init(std::string & specifiedMissileInfo) override;

public:
	int		_dmg;
	int		_hitCount;			// hit count
	float	_hitInterval;		// 연속 hit interval
	float 	_firstSpeed;
	float	_deltaSpeed;
	float	_mass;
	float	_maxSpeed;

	std::string _missileShotSoundStr;
	std::string _missileHitSoundStr;

	glm::vec3	_missileGenAddPos;

	float	_lifeTime;
};
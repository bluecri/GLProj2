#pragma once

#include "ISpecifiedMissileState.h"

class SpecifiedNormalMissileInfo;

class SpecifiedNormalMissileState : public ISpecifiedMissileState
{
public:
	SpecifiedNormalMissileState()
	{
		_dmg = 10;
		_hitCount = 1;
		_hitInterval = 1.0f;
		_firstSpeed = 4.0f;
		_deltaSpeed = 0.8f;

		_missileShotSoundStr = std::string("laser");
		_missileHitSoundStr = std::string("hit");

		_missileGenAddPos = glm::vec3(0.0f, 0.0f, 0.2f);
	}

	// ISpecifiedMissileState을(를) 통해 상속됨
	virtual int getType() override;
	int init(SpecifiedNormalMissileInfo * specifiedMissileInfo);

public:
	int _dmg;
	int _hitCount;
	float _hitInterval;
	float _firstSpeed;
	float _deltaSpeed;
	float _lifeTime;

	int _curDmg;
	int _curHitCount;
	float _curHitInterval;
	float _curFirstSpeed;
	float _curDeltaSpeed;
	float _curLifeTime;

	std::string _missileShotSoundStr;
	std::string _missileHitSoundStr;

	glm::vec3	_missileGenAddPos;
};
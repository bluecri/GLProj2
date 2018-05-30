#pragma once

#include "ISpecifiedMissileState.h"

class SpecifiedNormalMissileInfo;

class SpecifiedNormalMissileState : public ISpecifiedMissileState
{
public:
	SpecifiedNormalMissileState();

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
	float _mass;
	float _maxSpeed;

	int _curDmg;
	int _curHitCount;
	float _curHitInterval;
	float _curFirstSpeed;
	float _curDeltaSpeed;
	float _curLifeTime;
	float _curMass;
	float _curMaxSpeed;

	std::string _missileShotSoundStr;
	std::string _missileHitSoundStr;

	glm::vec3	_missileGenAddPos;
};
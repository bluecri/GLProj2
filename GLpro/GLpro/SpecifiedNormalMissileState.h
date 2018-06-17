#pragma once

#include "ISpecifiedMissileState.h"

class SpecifiedNormalMissileState : public ISpecifiedMissileState
{
public:
	SpecifiedNormalMissileState();

	// ISpecifiedMissileState��(��) ���� ��ӵ�
	virtual int getType() override;
	virtual void initSpecifiedState(ISpecifiedMissileInfo* info) override;
	virtual void transferBuffSum(BuffSum * buffSum, ISpecifiedMissileState * originState) override;

public:
	int _dmg;
	int _hitCount;
	float _hitInterval;
	float _firstSpeed;
	float _deltaSpeed;
	float _lifeTime;
	float _mass;
	float _maxSpeed;

	std::string _missileShotSoundStr;
	std::string _missileHitSoundStr;

	glm::vec3	_missileGenAddPos;

};
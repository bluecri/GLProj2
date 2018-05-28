#pragma once
class NormalMissileState
{
public:
	NormalMissileState()
	{
		_dmg = 10;
		_hitCount = 1;
		_hitInterval = 1.0f;
		_firstSpeed = 4.0f;
		_deltaSpeed = 0.8f;
	}

	int _dmg;
	int _hitCount;
	float _hitInterval;
	float _firstSpeed;
	float _deltaSpeed;
};
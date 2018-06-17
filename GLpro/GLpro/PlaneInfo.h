#pragma once

class BuffSum;

class PlaneInfo
{
public:
	PlaneInfo(int hp, int armor, float speed, float deltaSpeed = 0.1f, float notDmgTime = 3.0f, float angle = 0.01f, float angleSpeed = 0.8f);
	void transferBuffSum(BuffSum* buffSum, PlaneInfo* originBuffSum);

public:
	float			_angle;			// plane angle (quat rotation speed) = 0.01
	float			_angleSpeed;	// plane angle speed (quat rotation speed)
	int				_hp;
	int				_armor;

	float 			_notDmgedTime;

	float			_maxSpeed;
	float			_deltaSpeed;
};
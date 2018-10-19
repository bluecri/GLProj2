#pragma once
#include "stdafx.h"

enum ENUM_BUFFSUM_MODIFY_TYPE
{
	ENUM_BUFFSUM_MODIFY_PLANE_MAXSPEED,
	ENUM_BUFFSUM_MODIFY_PLANE_DELTASPEED,
	ENUM_BUFFSUM_MODIFY_SHOT_DMG,
	ENUM_BUFFSUM_MODIFY_SHOT_MAXSPEED,
	ENUM_BUFFSUM_MODIFY_SHOT_SPEED,			// missile speed
	ENUM_BUFFSUM_MODIFY_SHOT_DELAY,			// shot ration
	ENUM_BUFFSUM_MODIFY_SHOT_HITINTERVAL,	// multi hit interval
	ENUM_BUFFSUM_MODIFY_SHOT_HITCOUNT,
	ENUM_BUFFSUM_MODIFY_SHOT_MASS,
	ENUM_BUFFSUM_MODIFY_SHOT_LIFETIME,
	ENUM_BUFFSUM_MODIFY_NUM
};

enum ENUM_BUFFSUM_ONCE_TYPE		// bool type
{
	ENUM_BUFFSUM_ONCE_PLANE_OVERWHELMING,
	ENUM_BUFFSUM_ONCE_PLANE_RANDOM_DIRECTION,
	ENUM_BUFFSUM_ONCE_PLANE_INVISIBLE,
	ENUM_BUFFSUM_ONCE_SHOT_DISABLE,
	ENUM_BUFFSUM_ONCE_NUM
};

class BuffSum
{
public:
	BuffSum();
	void initBuffSum();
	void partialInitBuffSum();

public:
	std::vector<bool>	bModify;

	std::vector<float>	mult;
	std::vector<float>	acc;

	std::vector<bool>	bOnce;		// like trigger
};
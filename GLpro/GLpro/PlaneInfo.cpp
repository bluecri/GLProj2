#include "stdafx.h"
#include "PlaneInfo.h"
#include "BuffSum.h"

PlaneInfo::PlaneInfo(int hp, int armor, float speed, float deltaSpeed, float notDmgTime, float angle, float angleSpeed)
	:_hp(hp), _armor(armor), _maxSpeed(speed), _deltaSpeed(deltaSpeed), _notDmgedTime(notDmgTime), _angle(angle), _angleSpeed(angleSpeed)
{
}

void PlaneInfo::transferBuffSum(BuffSum * buffSum, PlaneInfo * originBuffSum)
{
	if (buffSum->bModify[ENUM_BUFFSUM_MODIFY_PLANE_DELTASPEED])
	{
		_deltaSpeed = originBuffSum->_deltaSpeed * buffSum->mult[ENUM_BUFFSUM_MODIFY_PLANE_DELTASPEED]
			+ buffSum->acc[ENUM_BUFFSUM_MODIFY_PLANE_DELTASPEED];
	}

	if (buffSum->bModify[ENUM_BUFFSUM_MODIFY_PLANE_MAXSPEED])
	{
		_maxSpeed = originBuffSum->_maxSpeed * buffSum->mult[ENUM_BUFFSUM_MODIFY_PLANE_MAXSPEED]
			+ buffSum->acc[ENUM_BUFFSUM_MODIFY_PLANE_MAXSPEED];
	}
	
	/*
	*	...
	*/
}

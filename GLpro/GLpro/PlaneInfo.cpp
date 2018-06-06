#include "stdafx.h"
#include "PlaneInfo.h"

PlaneInfo::PlaneInfo(int hp, int armor, float speed, float deltaSpeed, float notDmgTime, float angle, float angleSpeed)
	:_hp(hp), _armor(armor), _speed(speed), _deltaSpeed(deltaSpeed), _notDmgedTime(notDmgTime), _angle(angle), _angleSpeed(_angleSpeed)
{
}

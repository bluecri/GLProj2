#include "stdafx.h"
#include "IBuff.h"

IBuff::IBuff(ENUM_BUFF_TYPE buffType, float lifeTime) : _buffType(buffType), _lifeTime(lifeTime)
{}

ENUM_BUFF_TYPE IBuff::getBuffType()
{
	return _buffType;
}

bool IBuff::lifeEnd(float deltaTime, float acc)
{
	_lifeTime -= deltaTime;
	if (_lifeTime < 0.0f)
	{
		return true;
	}
	return false;
}

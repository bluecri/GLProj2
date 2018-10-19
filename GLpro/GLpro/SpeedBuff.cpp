#include "stdafx.h"
#include "SpeedBuff.h"
#include "BuffSum.h"

SpeedBuff::SpeedBuff(float maxSpeedBuffMul, float deltaSpeedBuffMul, float lifeTime)
	: IBuff(ENUM_BUFF_TYPE::ENUM_BUFF_SPEEDBUFF, lifeTime), _maxSpeedBuffMul(maxSpeedBuffMul), _deltaSpeedBuffMul(deltaSpeedBuffMul)
{
}

IBuff * SpeedBuff::getClone()
{
	IBuff* ret = new SpeedBuff(_maxSpeedBuffMul, _deltaSpeedBuffMul, _lifeTime);
	return ret;
}

void SpeedBuff::adjustBuffSum(BuffSum * buffSum)
{
	buffSum->bModify[ENUM_BUFFSUM_MODIFY_PLANE_MAXSPEED]		= true;
	buffSum->mult[ENUM_BUFFSUM_MODIFY_PLANE_MAXSPEED]			*= _maxSpeedBuffMul;
	buffSum->acc[ENUM_BUFFSUM_MODIFY_PLANE_MAXSPEED]			*= _maxSpeedBuffMul;

	buffSum->bModify[ENUM_BUFFSUM_MODIFY_PLANE_DELTASPEED]	= true;
	buffSum->mult[ENUM_BUFFSUM_MODIFY_PLANE_DELTASPEED]		*= _deltaSpeedBuffMul;
	buffSum->acc[ENUM_BUFFSUM_MODIFY_PLANE_DELTASPEED]		*= _deltaSpeedBuffMul;

	return;
}

// update curStatus when buff is removed. (cur states is updated only if bModify is true)
void SpeedBuff::removeBuffSum(BuffSum * buffSum)
{
	buffSum->bModify[ENUM_BUFFSUM_MODIFY_PLANE_MAXSPEED] = true;
	buffSum->bModify[ENUM_BUFFSUM_MODIFY_PLANE_DELTASPEED] = true;
}

float SpeedBuff::getPowerful()
{
	return _maxSpeedBuffMul + _deltaSpeedBuffMul;
}

#pragma once

#include "IBuff.h"

class SpeedBuff : public IBuff
{
public:
	SpeedBuff(float _maxSpeedBuffACC, float _deltaSpeedBuffACC = 2.0f, float lifeTime = 10.0f);

	// IBuff을(를) 통해 상속됨
	virtual IBuff*	getClone() override;
	virtual void	adjustBuffSum(BuffSum * buffSum) override;
	virtual float	getPowerful() override;

	void			removeBuffSum(BuffSum * buffSum);
private:
	float _maxSpeedBuffMul;
	float _deltaSpeedBuffMul;
};
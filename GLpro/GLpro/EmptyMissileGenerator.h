#pragma once
#include "IMissileGenerator.h"

class MissileGeneratorStorage;

class EmptyMissileGenerator : public IMissileGenerator
{
public:
	EmptyMissileGenerator(MissileGeneratorStorage* _missileGeneratorStorage, Entity* bindedEntity);

	// IMissileGenerator을(를) 통해 상속됨
	virtual void genMissile() override;
	virtual void updateTimer(float deltaTime, float acc) override;
	
};
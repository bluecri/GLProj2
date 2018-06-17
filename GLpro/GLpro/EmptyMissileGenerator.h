#pragma once
#include "IMissileGenerator.h"

class MissileGeneratorStorage;

class EmptyMissileGenerator : public IMissileGenerator
{
public:
	EmptyMissileGenerator();

	void init(Entity* bindedEntity, MissileGeneratorStorage* missileGeneratorStorage);
	// IMissileGenerator��(��) ���� ��ӵ�
	virtual void genMissile() override;
	virtual void updateTimer(float deltaTime, float acc) override;
	virtual void transferBuffSum(BuffSum * buffSum) override;

};
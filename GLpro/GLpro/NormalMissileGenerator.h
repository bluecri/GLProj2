#pragma once
#include "IMissileGenerator.h"

class MissileGeneratorStorage;

class NormalMissileGenerator : public IMissileGenerator
{
public:
	NormalMissileGenerator();

	// IMissileGenerator��(��) ���� ��ӵ�
	virtual void genMissile() override;
	virtual void updateTimer(float deltaTime, float acc) override;
	virtual void transferBuffSum(BuffSum * buffSum) override;

	void initNormalMissileGenerator(Entity* bindedEntity, MissileGeneratorStorage* missileGeneratorStorage);
	void initNormalMissileGenerator(Entity* bindedEntity, MissileGeneratorStorage* missileGeneratorStorage, std::string & commonMissileInfoFile, std::string & specifiedMissileInfo);

protected:
	bool	_bShotDisable;
};
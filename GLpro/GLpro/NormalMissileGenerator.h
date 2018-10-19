#pragma once
#include "IMissileGenerator.h"

class MissileGeneratorStorage;

class NormalMissileGenerator : public IMissileGenerator
{
public:
	NormalMissileGenerator();

	// IMissileGenerator을(를) 통해 상속됨
	virtual void genMissile() override;
	virtual void updateTimer(float deltaTime, float acc) override;
	virtual void transferBuffSum(BuffSum * buffSum) override;

	void initNormalMissileGenerator(Entity* bindedEntity, MissileGeneratorStorage* missileGeneratorStorage);
	void initNormalMissileGenerator(Entity* bindedEntity, MissileGeneratorStorage* missileGeneratorStorage, std::string & commonMissileInfoFile, std::string & specifiedMissileInfo);

protected:
	bool	_bShotDisable;
};
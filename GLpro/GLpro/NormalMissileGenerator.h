#pragma once
#include "IMissileGenerator.h"

class MissileGeneratorStorage;

class NormalMissileGenerator : public IMissileGenerator
{
public:
	NormalMissileGenerator(MissileGeneratorStorage* missileGeneratorStorage, Entity* bindedEntity)
		: IMissileGenerator(missileGeneratorStorage, bindedEntity)
	{
		_missileType = ENUM_MISSILE_TYPE::ENUM_MISSILE_TYPE_NORMAL;
	}

	// IMissileGenerator��(��) ���� ��ӵ�
	virtual void genMissile() override;
	virtual void updateTimer(float deltaTime, float acc) override;

	void init();
	void init(std::string & commonMissileInfoFile, std::string & specifiedMissileInfo);

};
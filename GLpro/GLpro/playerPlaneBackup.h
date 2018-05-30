#pragma once
/*
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "./IPlane.h"

#define PLAYER_DEFAULT_WEAPON_MAX_NUM 6

class ALSource;
class MissileGeneratorStorage;

class Player : public IPlane
{
public:
	Player(GameSession* gSession, RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
	virtual ~Player();
	void init();

	void inputProgress(long long transferKeyInput);
	virtual void logicUpdate(float deltaTime, float acc) override;
	virtual void collisionFunc(CollisionComponent* collisionComp) override;
	virtual void doJobWithBeDeleted() override;		// done befoe setBeDeleted (resource bind remove)
	bool isCanGetDmg();

public:
	MissileGeneratorStorage* _missileGeneratorStorage;

	int _curHp;
	int _maxHp;
	int _curArmor;
	int _maxArmor;

	// overwhelming
	float 	_notDmgedTime;
	bool	_bNotDmged;
	float 	_curDmgedTime;

	float _deltaSpeed;
	glm::vec3 _angleSpeedVec;

	ALSource* _explosionSound;

private:
	bool _bShotKeyDown;
};

#endif
*/
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "./IPlane.h"

class ALSource;

class Player : public IPlane
{
public:
	Player(RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
	virtual ~Player();
	void init();

	void inputProgress(long long transferKeyInput);
	virtual void logicUpdate(float deltaTime, float acc) override;
	virtual void collisionFunc(CollisionComponent* collisionComp) override;
	virtual void doJobWithBeDeleted() override;		// done befoe setBeDeleted (resource bind remove)
	bool isCanGetDmg();

public:
	float _shotDelay;
	float _curShotDelay;
	int _shotDmg;
	int _curHp;
	int _maxHp;
	int _curArmor;
	int _maxArmor;

	// overwhelming
	float 	_notDmgedTime;
	bool	_bNotDmged;
	float 	_curDmgedTime;

	float _deltaSpeed;
	float _angleSpeed;

	ALSource* _explosionSound;

private:
	bool _bShotKeyDown;
};

#endif
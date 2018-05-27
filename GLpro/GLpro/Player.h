#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "./IPlane.h"

class Player : public IPlane
{
public:
	Player(RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
	virtual ~Player();
	void init()
	{
		_shotDelay = 0.5f;
		_shotDmg = 10;
		_curHp = 100;
		_maxHp = 100;
		_curArmor = 0;
		_maxArmor = 100;
		_deltaSpeed = 0.1f;
	}
	void inputProgress(long long transferKeyInput);
	virtual void logicUpdate(float deltaTime, float acc) override;
	virtual void collisionFunc(CollisionComponent* collisionComp) override;
	bool isCanGetDmg();

public:
	float _shotDelay;
	float _curShotDelay;
	int _shotDmg;
	int _curHp;
	int _maxHp;
	int _curArmor;
	int _maxArmor;

	float _deltaSpeed;
	float _angleSpeed;

};

#endif
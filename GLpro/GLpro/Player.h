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
		_hp = 100;
		_maxHp = 100;
		_armor = 0;
		_maxArmor = 100;
		_deltaSpeed = 0.1f;
	}
	void inputProgress(long long transferKeyInput);
	virtual void logicUpdate(float deltaTime, float acc) override;
	virtual void collisionFunc(CollisionComponent* collisionComp) override;

	bool isCanGetDmg()
	{
		return true;
	}
public:
	float _shotDelay;
	int _shotDmg;
	int _hp;
	int _maxHp;
	int _armor;
	int _maxArmor;

	float _deltaSpeed;

};

#endif
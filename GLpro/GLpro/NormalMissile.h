#pragma once

#include "IMissile.h"

class NormalMissile : public IMissile
{
public:
	NormalMissile(Entity* fromEntity, RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
	virtual ~NormalMissile();
	void init()
	{
		_dmg = 10;
		_hitCount = 1;
		_hitInterval = 1.0f;
		_deltaSpeed = 0.8f;
		_lifeTime = 3.0f;

		_curLifeTime = 0.0f;
		_curHitInterval = 0.0f;
	}
	virtual void logicUpdate(float deltaTime, float acc) override;
	virtual void collisionFunc(CollisionComponent* collisionComp) override;

protected:
	void afterDmgOther();	// dmg �� �� ��ó��

public:
	int		_dmg;
	int		_hitCount;			// hit count
	float	_hitInterval;		// ���� hit interval
	float	_curHitInterval;
	float	_deltaSpeed;
	float	_lifeTime;
	float	_curLifeTime;

	// IMissile��(��) ���� ��ӵ�
	virtual bool isDmgValid() override;
	virtual int getDmg() override;
	virtual int accHitCount(int acc) override;
};
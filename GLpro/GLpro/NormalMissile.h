#pragma once

#include "IMissile.h"

class NormalMissile : public IMissile
{
public:
	NormalMissile(Entity* fromEntity, RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
	virtual ~NormalMissile();
	void init(glm::mat4& localMissilePos);
	void init(glm::mat4 & localMissilePos, int dmg, int hitCount, float hitInterval, float firstSpeed, float deltaSpeed, float lifeTime, std::string & startSound, std::string & hitSound);
	
	// Entity을(를) 통해 상속됨
	virtual void logicUpdate(float deltaTime, float acc) override;
	virtual void collisionFunc(CollisionComponent* collisionComp) override;
	virtual void doJobWithBeDeleted() override;		// done befoe setBeDeleted (resource bind remove)

protected:
	void afterDmgOther();	// dmg 준 뒤 후처리

public:
	int		_dmg;
	int		_hitCount;			// hit count
	float	_hitInterval;		// 연속 hit interval
	float	_curHitInterval;
	float 	_firstSpeed;
	float	_deltaSpeed;
	float	_lifeTime;
	float	_curLifeTime;

	ALSource* _startSound;
	ALSource* _hitSound;


	// IMissile을(를) 통해 상속됨
	virtual bool isDmgValid() override;
	virtual int getDmg() override;
	virtual int accHitCount(int acc) override;
};
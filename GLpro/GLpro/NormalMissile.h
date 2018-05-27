#pragma once

#include "IMissile.h"

class NormalMissile : public IMissile
{
public:
	NormalMissile(Entity* fromEntity, RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
	virtual ~NormalMissile();
	void init(glm::mat4& localMissilePos)
	{
		_dmg = 10;
		_hitCount = 1;
		_hitInterval = 1.0f;
		_firstSpeed = 4.0f;
		_deltaSpeed = 0.8f;
		_lifeTime = 3.0f;

		_curLifeTime = 0.0f;
		_curHitInterval = 0.0f;

		_startSound = GALManager->getNewALSource(std::string("laser"), _rigidbodyComponent->_transform);
		_startSound.play();
		_hitSound = GALManager->getNewALSource(std::string("hit"), _rigidbodyComponent->_transform);

		_rigidbodyComponent->_transform->setLocalMatWithWorldMat(localMissilePos);
		_rigidbodyComponent->_transform->speedAdd(_firstSpeed);
	}
	void init(glm::mat4& localMissilePos, int dmg, int hitCount, float hitInterval, float firstSpeed, float deltaSpeed, float lifeTime)
	{
		_dmg = dmg;
		_hitCount = hitCount;
		_hitInterval = hitInterval;
		_firstSpeed = firstSpeed;
		_deltaSpeed = deltaSpeed;
		_lifeTime = lifeTime;

		_curLifeTime = 0.0f;
		_curHitInterval = 0.0f;

		_startSound = GALManager->getNewALSource(std::string("laser"), _rigidbodyComponent->_transform);
		_hitSound = GALManager->getNewALSource(std::string("hit"), _rigidbodyComponent->_transform);

		_rigidbodyComponent->_transform->setLocalMatWithWorldMat(localMissilePos);
		_rigidbodyComponent->_transform->speedAdd(_firstSpeed);

	}
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
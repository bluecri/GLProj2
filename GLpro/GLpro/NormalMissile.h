#pragma once

#include "IMissile.h"

class SpecifiedNormalMissileState;
class GameSession;
class ALSource;
class CommonMissileState;

class NormalMissile : public IMissile
{
public:
	NormalMissile(Entity* fromEntity, GameSession* gSession, CommonMissileState* commonMissileState);
	virtual ~NormalMissile();
	void initNormalMissile(const glm::mat4& localMissileMat);
	void initNormalMissile(const glm::mat4 & localMissileMat, SpecifiedNormalMissileState* specifiedNormalMissileState);

	// Entity을(를) 통해 상속됨
	virtual void logicUpdate(float deltaTime, float acc) override;
	virtual void collisionFunc(CollisionComponent* collisionComp) override;
	virtual void doJobWithBeDeleted() override;		// done befoe setBeDeleted (resource bind remove)

	bool isDmgValid();
	int getDmg();
	int accHitCount(int acc);

protected:
	void afterDmgOther();	// dmg 준 뒤 후처리

public:
	SpecifiedNormalMissileState* _missileState;		// can exist 2 state (origin / cur)

	float	_curHitInterval;
	float	_curLifeTime;

	ALSource* _startSound;
	ALSource* _hitSound;

};
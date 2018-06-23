#pragma once

#include "IGameObject.h"

class GameSession;

class LightGameObject	:	public IGameObject
{
public:
	LightGameObject(GameSession * gSession);

	// IGameObject을(를) 통해 상속됨
	virtual void doJobWithBeDeleted() override;
	virtual void logicUpdate(float deltaTime, float acc) override;
	virtual void collisionFunc(CollisionComponent * collisionComp) override;

};
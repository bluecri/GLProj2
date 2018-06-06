#pragma once

#include "IPlane.h"

// todo : change to gameobj
class IMissile : public IPlane
{
public:
	IMissile(int type, GameSession* gSession, Entity* fromEntity, RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
	virtual ~IMissile();

	virtual bool isDmgValid() = 0;
	virtual int getDmg() = 0;
	virtual int accHitCount(int acc) = 0;

public:
	Entity* _fromEntity;		// who shot

protected:
	virtual void collisionFunc(CollisionComponent* collisionComp) override = 0;
	virtual void logicUpdate(float deltaTime, float acc) override;
	// collisionLogicUpdate에서 사용하는 collision 처리 함수
};
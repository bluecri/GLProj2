#pragma once

#include "IPlane.h"

class IMissile : public IPlane
{
public:
	IMissile(int type, Entity* fromEntity, RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
	virtual ~IMissile();

	virtual bool isDmgValid() = 0;
	virtual int getDmg() = 0;
	virtual int accHitCount(int acc) = 0;


public:
	Entity* _fromEntity;		// who shot

protected:
	virtual void collisionFunc(CollisionComponent* collisionComp) override = 0;		// collisionLogicUpdate에서 사용하는 collision 처리 함수
};
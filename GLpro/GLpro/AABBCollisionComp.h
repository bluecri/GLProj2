#pragma once
#include "stdafx.h"
#include "CollisionComponent.h"

class AABBCollisionComp : public CollisionComponent
{
public:
	AABBCollisionComp(RigidbodyComponent* rigidComp, glm::vec3& localVec, glm::vec3& axisLen);

	// CollisionComponent을(를) 통해 상속됨
	virtual void updateCollisionComp() override;
	virtual bool collideTestToOther(CollisionComponent* comp) override;
	virtual void setAxisLen(vec3& halfSxisVec) override;

public:

};
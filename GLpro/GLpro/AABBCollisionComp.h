#pragma once
#include "stdafx.h"
#include "CollisionComponent.h"

class AABBCollisionComp : public CollisionComponent
{
public:
	AABBCollisionComp(RigidbodyComponent* rigidComp, glm::vec3& localVec, glm::vec3& axisLen);

	// CollisionComponent을(를) 통해 상속됨
	virtual void updateWithRigidComp() override;
	virtual void updateAABBForOctree() override;
	virtual bool collideTestToOther(CollisionComponent* comp) override;

public:

};
#pragma once
#include "stdafx.h"
#include "CollisionComponent.h"

class OBBCollisionComp : public CollisionComponent
{
public:
	OBBCollisionComp(RigidbodyComponent* rigidComp, glm::mat4& localMat, glm::vec3& axisLen);

	// CollisionComponent을(를) 통해 상속됨
	virtual void updateWithRigidComp() override;
	virtual void updateAABBForOctree() override;
	virtual bool collideTestToOther(CollisionComponent* comp) override;

private:
	float _axisTotalLen;	// for aabb axis len
};
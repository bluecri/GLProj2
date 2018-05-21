#pragma once
#include "stdafx.h"
#include "CollisionComponent.h"

class OBBCollisionComp : public CollisionComponent
{
public:
	OBBCollisionComp(RigidbodyComponent* rigidComp, glm::mat4& localMat, glm::vec3& axisLen)
		: CollisionComponent(rigidComp), _localMat(localMat), _axisLen(_axisLen)
	{
		collisionType = COLLISION_TYPE::COLLISION_OBB;
	}

	// CollisionComponent을(를) 통해 상속됨
	virtual void updateWithRigidComp() override;
	virtual void updateAABBForOctree() override;
	virtual bool collideTestToOther(CollisionComponent* comp) override;

public:
	glm::mat4 _localMat;	//used for axis vec : mat[0]. mat[1], mat[2]	// pos : mat[3]
	glm::vec3 _axisLen;

	glm::mat4 _worldlMat;	// update by updateWithRigidComp()

	// CollisionComponent을(를) 통해 상속됨 (for AABB - octree)
	//glm::vec3 _center;
	//glm::vec3 _halfAxisSize;
};
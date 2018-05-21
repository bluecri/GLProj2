#pragma once

#include <list>
class RigidbodyComponent;
class CollisionComponentManager;

enum COLLISION_TYPE
{
	COLLISION_AABB,
	COLLISION_OBB
};

class CollisionComponent
{
public:
	CollisionComponent(RigidbodyComponent* rigidComp)
	{
		_rigidComp = rigidComp;
	};

	RigidbodyComponent* _rigidComp;

protected:
	virtual void updateWithRigidComp() = 0;		// collision local + rigidComp(world transform) -> collision world
	virtual void updateAABBForOctree() = 0;		// collision world -> AABB for oct

	virtual bool collideTestToOther(CollisionComponent* comp) = 0;
	
public:
	COLLISION_TYPE collisionType;

	// for AABB
	glm::vec3 _center;
	glm::vec3 _halfAxisSize;

	std::list<CollisionComponent*> _collisionLogList;

	friend CollisionComponentManager;
};

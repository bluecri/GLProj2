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
		_bDeleted = false;
		_bCollisionTest = true;
		_bCollideVelocityUpdate = true;
		_bAlreadyVelocityUpdated = false;
	};

	void setDeleted(bool bDeleted) {
		_bDeleted = bDeleted;
	}
	void setCollisionTest(bool bCollisionTest)
	{
		_bCollisionTest = bCollisionTest;
	}

protected:
	virtual void updateWithRigidComp() = 0;		// collision local + rigidComp(world transform) -> collision world
	virtual void updateAABBForOctree() = 0;		// collision world -> AABB for oct
	virtual bool collideTestToOther(CollisionComponent* comp) = 0;
	
public:
	COLLISION_TYPE collisionType;
	RigidbodyComponent* _rigidComp;

	// for AABB
	glm::vec3 _center;
	glm::vec3 _halfAxisSize;

	std::list<CollisionComponent*> _collisionLogList;	// collision event(logic update)¿Í collision physics ºÐ¸®

private:
	bool _bDeleted;						// component delete
	bool _bCollisionTest;	// in octree for test or not
	bool _bAlreadyVelocityUpdated;		// for CollisionComponentManager 
	bool _bCollideVelocityUpdate;	// force with collision or not

	friend CollisionComponentManager;
};

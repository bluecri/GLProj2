#pragma once

#include "stdafx.h"
#include "AABBOb.h"

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
	CollisionComponent(RigidbodyComponent* rigidComp);;

	const glm::mat4&	getWorldMatRef();
	const glm::vec3&	getAxisLenRef();

	void setDeleted(bool bDeleted);
	void setCollisionTest(bool bCollisionTest);
	void setCollisionVelocityUpdate(bool bVelUpdate);


	static bool sIsBoxCollisionCheck(const glm::mat4 & wolrd1, const glm::mat4 & wolrd2, const glm::vec3 & axisLen1, const glm::vec3 & axisLen2);

protected:
	virtual void updateWithRigidComp() = 0;		// collision local + rigidComp(world RigidbodyComponent) -> collision world
	virtual void updateAABBForOctree() = 0;		// collision world -> AABB for oct
	virtual bool collideTestToOther(CollisionComponent* comp) = 0;

public:
	COLLISION_TYPE collisionType;
	RigidbodyComponent* _rigidComp;

	// for AABB in OctreeForCollision	(opt : AABB collision comp인 경우 data 중복)
	AABBOb _aabbObForOctree;

	std::list<CollisionComponent*> _collisionLogList;	// collision event(logic update)와 collision physics 분리

protected:
	glm::mat4 _localMat;	// collision component local matrix (trans & rotate)
	glm::mat4 _worldMat;	// update by updateWithRigidComp()
	glm::vec3 _axisLen;		// collision box axis len


private:
	bool _bDeleted;						// component delete
	bool _bCollisionTest;				// in octree for test or not
	bool _bCollideVelocityUpdate;		// force with collision or not
	bool _bAlreadyVelocityUpdated;		// for CollisionComponentManager 

	friend CollisionComponentManager;
};

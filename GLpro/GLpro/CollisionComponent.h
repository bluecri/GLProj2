#pragma once

#include "stdafx.h"
#include "AABBOb.h"

class RigidbodyComponent;
class CollisionComponentManager;
class OctreeForCollision;

enum COLLISION_TYPE
{
	COLLISION_AABB,
	COLLISION_OBB
};

class CollisionComponent : public VectorPElem
{
public:
	CollisionComponent(RigidbodyComponent* rigidComp);

	const glm::mat4&	getWorldMatRef();
	const glm::vec3&	getAxisLenForAABBRef();

	void setDeleted(bool bDeleted);
	void setCollisionTest(bool bCollisionTest);
	void setCollisionVelocityUpdate(bool bVelUpdate);

	virtual void setAxisLen(vec3& halfSxisVec) = 0;
	void setLocalRotation(glm::mat4& rotMat);
	void setLocalPos(glm::vec3& posVec);

	bool isAABBForOctreeDirty();
	static bool sIsBoxCollisionCheck(const glm::mat4 & wolrd1, const glm::mat4 & wolrd2, const glm::vec3 & axisLen1, const glm::vec3 & axisLen2);

protected:
	/*
	*	collision local + rigidComp(world RigidbodyComponent) -> collision world
	*	return whether is modified
	*/
	virtual void updateCollisionComp() = 0;
	virtual bool collideTestToOther(CollisionComponent* comp) = 0;

public:
	COLLISION_TYPE collisionType;
	RigidbodyComponent* _rigidComp;

	// for AABB in OctreeForCollision	(opt : AABB collision comp인 경우 data 중복)
	AABBOb		_aabbObForOctree;
	int			_octreeElemIndex;

	std::list<CollisionComponent*> _collisionLogList;	// collision event(logic update)와 collision physics 분리

protected:
	glm::mat4 _localMat;	// collision component local matrix (trans & rotate)
	glm::mat4 _worldMat;	// update by updateWithRigidComp()

	bool _bDirty;						// is comp is dirty. update when updateCollisionComp()
	bool _bAxisModified;				// localMat Axis is modified
	bool _bPosModified;					// localMat Pos is modified
	bool _bRotateModified;				// localMat Rotate is modified

private:
	bool _bDeleted;						// component delete
	bool _bCollisionTest;				// in octree for test or not
	bool _bCollideVelocityUpdate;		// force with collision or not
	bool _bAlreadyVelocityUpdated;		// for CollisionComponentManager 

	friend CollisionComponentManager;
	friend OctreeForCollision;
};

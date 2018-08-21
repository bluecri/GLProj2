#pragma once

#include "stdafx.h"

class RigidbodyComponent;
class CollisionComponentManager;
class OctreeForCollision;
class DynamicCollisionSubComp;

enum COLLISION_TYPE
{
	COLLISION_AABB,
	COLLISION_OBB,
	COLLISION_SPHERE,
	COLLISION_LINE
};

class CollisionComponent : public VectorPElem
{
public:
	CollisionComponent(RigidbodyComponent* rigidComp);

	void setDeleted(bool bDeleted);
	void setCollisionTest(bool bCollisionTest);		// do collision test?
	void setCollisionVelocityUpdate(bool bVelUpdate);	// do velocity update result by collision?

	bool	isCollisionComponentDirty();
	void	resetCollisionComponentDirty();

	void	setCollisionCategoryBit(int bit);
	void	setCollisionCategoryMaskBit(int maskBit);
	int		getCollisionCategoryBit();
	int		getCollisionCategoryMaskBit();

	bool	testCollisionCategoryBit(const CollisionComponent* otherComp);
	//bool	testCollisionCategoryBit(int otherMastBit);
	//bool	testCollisionCategoryMaskBit(int otherBit);
	
	int		getOctreeElemIndex();
	void	setOctreeElemIdx(int idx);

	bool	isDynamicComp();
	void	makeDynamicComp(DynamicCollisionSubComp* subComp);
	DynamicCollisionSubComp* getDynamicSubComp();
	virtual void	updateDynamicLap() = 0;

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

	std::list<CollisionComponent*> _collisionLogList;	// collision event(logic update)¿Í collision physics ºÐ¸®

protected:
	bool _bDirty;							// is comp is dirty. update when updateCollisionComp()
	DynamicCollisionSubComp*		_dynamicSubComp;

private:
	int		_octreeElemIdx;					// Octree.h's OctreeElem index.

	bool	_bDeleted;						// component delete
	bool	_bCollisionTest;				// in octree for test or not
	bool	_bAlreadyVelocityUpdated;		// for CollisionComponentManager 

	int		_collisionCategoryBit;
	int		_collisionCategoryMaskBit;
	bool	_bCollideVelocityUpdate;		// force with collision or not

	friend CollisionComponentManager;
	friend OctreeForCollision;
};

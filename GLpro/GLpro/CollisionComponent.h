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
	
	bool	isCollisionComponentDirty();
	void	resetCollisionComponentDirty();

	void	setCollisionCategoryBit(int bit);
	void	setCollisionCategoryMaskBit(int maskBit);
	int		getCollisionCategoryBit() const;
	int		getCollisionCategoryMaskBit() const;

	bool	testCollisionCategoryBit(const CollisionComponent* otherComp);
	//bool	testCollisionCategoryBit(int otherMastBit);
	//bool	testCollisionCategoryMaskBit(int otherBit);
	
	int		getOctreeElemIndex();
	void	setOctreeElemIdx(int idx);

	bool	isDynamicComp();
	void	makeDynamicComp(DynamicCollisionSubComp* subComp);
	DynamicCollisionSubComp* getDynamicSubComp();
	virtual void	updateDynamicLap() = 0;

	//virtual bool collideTestToOther(CollisionComponent* comp) = 0;
	virtual bool collideStaticTestToOtherStatic(CollisionComponent* staticComp) = 0;
	virtual bool collideStaticTestToOtherDynamic(CollisionComponent* staticComp, float collideTime) = 0;
	//virtual bool collideDynamicTestToOtherStatic(CollisionComponent* staticComp, float collideTime) = 0;
	virtual bool collideDynamicTestToOtherDynamic(CollisionComponent* staticComp, float collideTime) = 0;

	virtual bool lapStaticTestToOtherStatic(CollisionComponent* staticComp) = 0;
	virtual bool lapStaticTestToOtherDynamic(CollisionComponent* staticComp) = 0;
	//virtual bool lapDynamicTestToOtherStatic(CollisionComponent* staticComp) = 0;
	virtual bool lapDynamicTestToOtherDynamic(CollisionComponent* staticComp) = 0;

	virtual void resolveStaticStaticCollide(CollisionComponent* staticComp) = 0;
	//virtual void resolveStaticDynamicCollide(float time, CollisionComponent* dynamicComp) = 0;
	virtual void resolveDynamicStaticCollide(float time, CollisionComponent* staticComp) = 0;
	virtual void resolveDynamicDynamicCollide(float time, CollisionComponent* dynamicComp) = 0;

	bool isTrigger();
	void setTrigger(bool bTrigger);

protected:
	/*
	*	collision local + rigidComp(world RigidbodyComponent) -> collision world
	*	return whether is modified
	*/
	virtual void updateCollisionComp() = 0;

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

	int		_collisionCategoryBit;
	int		_collisionCategoryMaskBit;
	bool	_bIsTrigger;		// force with collision or not

	friend CollisionComponentManager;
	friend OctreeForCollision;
};

#include "stdafx.h"
#include "CollisionComponent.h"
#include "DynamicCollisionSubComp.h"

CollisionComponent::CollisionComponent(RigidbodyComponent * rigidComp)
	: VectorPElem()
{
	_dynamicSubComp = nullptr;

	_rigidComp = rigidComp;
	_bDeleted = false;
	_bCollisionTest = true;
	_bIsTrigger = false;

	_collisionLogList = std::list<CollisionComponent*>();

	_collisionCategoryBit = 1;
	_collisionCategoryMaskBit = -1;
}

void CollisionComponent::makeDynamicComp(DynamicCollisionSubComp * subComp)
{
	_dynamicSubComp = subComp;
}

void CollisionComponent::setDeleted(bool bDeleted) 
{
	_bDeleted = bDeleted;
}

void CollisionComponent::setCollisionTest(bool bCollisionTest)
{
	_bCollisionTest = bCollisionTest;
}

bool CollisionComponent::isCollisionComponentDirty()
{
	return _bDirty;
}

void CollisionComponent::resetCollisionComponentDirty()
{
	_bDirty = false;
}

void CollisionComponent::setCollisionCategoryBit(int bit)
{
	_collisionCategoryBit = bit;
}

void CollisionComponent::setCollisionCategoryMaskBit(int maskBit)
{
	_collisionCategoryMaskBit = maskBit;
}

int CollisionComponent::getCollisionCategoryBit() const
{
	return _collisionCategoryBit;
}

int CollisionComponent::getCollisionCategoryMaskBit() const
{
	return _collisionCategoryMaskBit;
}

bool CollisionComponent::testCollisionCategoryBit(const CollisionComponent * otherComp)
{
	bool cond1 = _collisionCategoryMaskBit & otherComp->getCollisionCategoryBit();
	bool cond2 = _collisionCategoryBit & otherComp->getCollisionCategoryMaskBit();

	return (cond1 && cond2);
}

int CollisionComponent::getOctreeElemIndex()
{
	return _octreeElemIdx;
}

void CollisionComponent::setOctreeElemIdx(int idx)
{
	_octreeElemIdx = idx;;
}

bool CollisionComponent::isDynamicComp()
{
	return (_dynamicSubComp != nullptr);
}

DynamicCollisionSubComp * CollisionComponent::getDynamicSubComp()
{
	return _dynamicSubComp;
}

bool CollisionComponent::isTrigger()
{
	return _bIsTrigger;
}

void CollisionComponent::setTrigger(bool bTrigger)
{
	_bIsTrigger = bTrigger;
}

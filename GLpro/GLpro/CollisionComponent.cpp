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
	_bCollideVelocityUpdate = true;
	_bAlreadyVelocityUpdated = false;

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

void CollisionComponent::setCollisionVelocityUpdate(bool bVelUpdate)
{
	_bCollideVelocityUpdate = bVelUpdate;
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

int CollisionComponent::getCollisionCategoryBit()
{
	return _collisionCategoryBit;
}

int CollisionComponent::getCollisionCategoryMaskBit()
{
	return _collisionCategoryMaskBit;
}

bool CollisionComponent::testCollisionCategoryBit(const CollisionComponent * otherComp)
{
	bool cond1 = _collisionCategoryMask & otherComp->getCollisionCategoryBit();
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

int CollisionComponent::getOctreeElemIndexPrevDynamic()
{
	return _dynamicSubComp->getOctreeElemIndexPrevDynamic();

}

int CollisionComponent::getOctreeElemIndexNextDynamic()
{
	return _dynamicSubComp->getOctreeElemIndexNextDynamic();
}

void CollisionComponent::setOctreeElemIndexPrevDynamic(int index)
{
	_dynamicSubComp->setOctreeElemIndexPrevDynamic(index);

}

void CollisionComponent::setOctreeElemIndexNextDynamic(int index)
{
	_dynamicSubComp->setOctreeElemIndexNextDynamic(index);
}

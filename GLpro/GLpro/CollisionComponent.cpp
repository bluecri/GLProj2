#include "stdafx.h"
#include "CollisionComponent.h"
#include "DynamicCollisionSubComp.h"
#include "RigidbodyComponent.h"
#include "CollisionFuncStatic.h"

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

CollisionComponent::~CollisionComponent()
{
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

void CollisionComponent::getRotateQuatAfterCollisionAndSetSpeed(CollisionComponent* otherComp, const glm::vec3 & collisionCenterDiff, glm::quat & rotateQuat1, glm::quat & rotateQuat2)
{
	float m1 = _rigidComp->getMass();
	float m2 = otherComp->_rigidComp->getMass();

	const glm::vec3& v1 = _rigidComp->getVelocityRef();
	const glm::vec3& v2 = otherComp->_rigidComp->getVelocityRef();

	// get rotated Velocity
	glm::vec3 zVec1 = v1;
	glm::vec3 zVec2 = v2;
	glm::vec3 distBetween2Collide = glm::normalize(collisionCenterDiff);	// collide center - center
																							//glm::vec3 distBetween2Collide = glm::normalize( getAABBConstRef().getCenterConstRef() - staticComp->getAABBConstRef().getCenterConstRef() );	// collide center - center

	float zVecOrtho1 = glm::dot(zVec1, distBetween2Collide);
	float zVecOrtho2 = glm::dot(zVec2, distBetween2Collide);

	float zVecOrthoRet1 = zVecOrtho1 + m2*(zVecOrtho2 - zVecOrtho1) * 2.0f / (m1 + m2);
	float zVecOrthoRet2 = zVecOrtho2 + m2*(zVecOrtho1 - zVecOrtho2) * 2.0f / (m1 + m2);

	glm::vec3 zVecRet1 = zVec1 + (zVecOrthoRet1 - zVecOrtho1) * distBetween2Collide;
	glm::vec3 zVecRet2 = zVec2 + (zVecOrthoRet2 - zVecOrtho2) * distBetween2Collide;

	rotateQuat1 = CollisionFuncStatic::rotationBetweenVectors(zVec1, zVecRet1);
	rotateQuat2 = CollisionFuncStatic::rotationBetweenVectors(zVec2, zVecRet2);

	// get speed from rotated Velocity
	float retSpeed1 = glm::length(zVecRet1);
	float retSpeed2 = glm::length(zVecRet2);

	// set speed
	_rigidComp->speedSet(retSpeed1);
	otherComp->_rigidComp->speedSet(retSpeed2);
}

void CTBB_DirtyBItInit_do::operator()(CollisionComponent * item) const
{
	item->resetCollisionComponentDirty();
}

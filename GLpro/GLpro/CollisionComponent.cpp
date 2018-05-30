#include "stdafx.h"
#include "CollisionComponent.h"

void CollisionComponent::setDeleted(bool bDeleted) {
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

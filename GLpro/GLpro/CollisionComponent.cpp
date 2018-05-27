#include "stdafx.h"
#include "CollisionComponent.h"

inline void CollisionComponent::setDeleted(bool bDeleted) {
	_bDeleted = bDeleted;
}

inline void CollisionComponent::setCollisionTest(bool bCollisionTest)
{
	_bCollisionTest = bCollisionTest;
}

inline void CollisionComponent::setCollisionVelocityUpdate(bool bVelUpdate)
{
	_bCollideVelocityUpdate = bVelUpdate;
}

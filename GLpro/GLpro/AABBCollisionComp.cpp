#include "stdafx.h"
#include "OBBCollisionComp.h"
#include "AABBCollisionComp.h"
#include "RigidbodyComponent.h"
#include "./src/Transform.h"

void AABBCollisionComp::updateWithRigidComp()
{
	const glm::mat4& worldMatRef = _rigidComp->_transform->getWorldMatRef();

	// if this is AABB.. only update pos
	for (int i = 0; i < 3; i++)
	{
		_worldlMat[3][i] = worldMatRef[3][i] + _localMat[3][i];
	}
}

void AABBCollisionComp::updateAABBForOctree()
{
	_center = _worldlMat[3];
	_halfAxisSize = _axisLen;
}

bool AABBCollisionComp::collideTestToOther(CollisionComponent * comp)
{
	switch (comp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
		return sIsBoxCollisionCheck(_worldlMat, static_cast<AABBCollisionComp*>(comp)->_worldlMat, _axisLen, static_cast<AABBCollisionComp*>(comp)->_axisLen);

	case COLLISION_TYPE::COLLISION_OBB:
		return sIsBoxCollisionCheck(_worldlMat, static_cast<OBBCollisionComp*>(comp)->_worldlMat, _axisLen, static_cast<OBBCollisionComp*>(comp)->_axisLen);
		break;

	default:
		printf_s("[LOG] : default case inAABBCollisionComp:: collideTestToOther\n");
		return false;
	}

	return false;
}

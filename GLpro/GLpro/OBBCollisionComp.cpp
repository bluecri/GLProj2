#include "stdafx.h"
#include "OBBCollisionComp.h"
#include "AABBCollisionComp.h"
#include "RigidbodyComponent.h"


void OBBCollisionComp::updateWithRigidComp()
{
	_worldlMat = _rigidComp->getWorldMatRef() * _localMat;
}

void OBBCollisionComp::updateAABBForOctree()
{
	_center = _worldlMat[3];
	_halfAxisSize = glm::vec3(_axisTotalLen, _axisTotalLen, _axisTotalLen);
}

bool OBBCollisionComp::collideTestToOther(CollisionComponent * comp)
{
	switch (comp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
		return sIsBoxCollisionCheck(_worldlMat, static_cast<AABBCollisionComp*>(comp)->_worldlMat, _axisLen, static_cast<AABBCollisionComp*>(comp)->_axisLen);
	
	case COLLISION_TYPE::COLLISION_OBB:
		return sIsBoxCollisionCheck(_worldlMat, static_cast<OBBCollisionComp*>(comp)->_worldlMat, _axisLen, static_cast<OBBCollisionComp*>(comp)->_axisLen);
		break;
	
	default:
		printf_s("[LOG] : default case in OBBCollisionComp::collideTestToOther\n");
		return false;
	}

	return false;
}

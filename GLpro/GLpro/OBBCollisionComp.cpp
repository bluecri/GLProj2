#include "stdafx.h"
#include "OBBCollisionComp.h"
#include "AABBCollisionComp.h"
#include "RigidbodyComponent.h"


OBBCollisionComp::OBBCollisionComp(RigidbodyComponent * rigidComp, glm::mat4 & localMat, glm::vec3 & axisLen)
	: CollisionComponent(rigidComp)
{
	_localMat = localMat;
	_axisLen = _axisLen;

	collisionType = COLLISION_TYPE::COLLISION_OBB;
	_axisTotalLen = glm::length(_axisLen);
}

void OBBCollisionComp::updateWithRigidComp()
{
	_worldMat = _rigidComp->getWorldMatRef() * _localMat;
	_aabbObForOctree.updateAABBObAxis(_axisTotalLen);
}

void OBBCollisionComp::updateAABBForOctree()
{
	_aabbObForOctree.updateAABBObCenter(_worldMat[3]);
}

bool OBBCollisionComp::collideTestToOther(CollisionComponent * comp)
{
	switch (comp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
	case COLLISION_TYPE::COLLISION_OBB:
		return sIsBoxCollisionCheck(_worldMat, comp->getWorldMatRef(), _axisLen, comp->getAxisLenRef());
		break;

	default:
		printf_s("[LOG] : default case inAABBCollisionComp:: collideTestToOther\n");
		return false;
	}

	return false;
}

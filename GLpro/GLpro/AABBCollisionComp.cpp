#include "stdafx.h"
#include "OBBCollisionComp.h"
#include "AABBCollisionComp.h"
#include "RigidbodyComponent.h"

AABBCollisionComp::AABBCollisionComp(RigidbodyComponent * rigidComp, glm::vec3 & localVec, glm::vec3 & axisLen)
	: CollisionComponent(rigidComp)
{
	collisionType = COLLISION_TYPE::COLLISION_AABB;

	_localMat = glm::mat4();
	for (int i = 0; i < 3; i++)
	{
		_localMat[3][i] = localVec[i];
	}

	_worldMat = glm::mat4();
	_axisLen = axisLen;

	_aabbObForOctree.updateAABBObAxis(_axisLen);
}

void AABBCollisionComp::updateWithRigidComp()
{
	const glm::mat4& worldMatRef = _rigidComp->getWorldMatRef();

	// if this is AABB.. only update pos
	for (int i = 0; i < 3; i++)
	{
		_worldMat[3][i] = worldMatRef[3][i] + _localMat[3][i];
	}
}

void AABBCollisionComp::updateAABBForOctree()
{
	_aabbObForOctree.updateAABBObCenter(_worldMat[3]);
}

bool AABBCollisionComp::collideTestToOther(CollisionComponent * comp)
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

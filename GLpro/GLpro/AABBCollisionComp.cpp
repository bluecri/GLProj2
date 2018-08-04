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

	_aabbObForOctree.updateAABBObAxis(axisLen);
}

void AABBCollisionComp::updateCollisionComp()
{
	//No test _bRotateModified : AABB

	_bDirty = false;

	if (_bAxisModified)
	{
		_bDirty = true;
	}

	// no need to update _worldMat
	if (!(_rigidComp->isDirtyByRigidLoopUpdate()) && !_bPosModified)		//opt : rigid dirty check only pos
	{
		return;
	}

	const glm::mat4& worldMatRef = _rigidComp->getWorldMatRef();

	// if this is AABB.. only update pos
	for (int i = 0; i < 3; i++)
	{
		_worldMat[3][i] = worldMatRef[3][i] + _localMat[3][i];
	}

	_aabbObForOctree.updateAABBObCenter(_worldMat[3]);
	_bDirty = true;
}

bool AABBCollisionComp::collideTestToOther(CollisionComponent * comp)
{
	switch (comp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
	case COLLISION_TYPE::COLLISION_OBB:
		return sIsBoxCollisionCheck(_worldMat, comp->getWorldMatRef(), _aabbObForOctree.getAxisConstRef(), comp->getAxisLenForAABBRef());
		break;

	default:
		printf_s("[LOG] : default case inAABBCollisionComp:: collideTestToOther\n");
		return false;
	}

	return false;
}

void AABBCollisionComp::setAxisLen(vec3& halfSxisVec)
{
	_bAxisModified = true;
	_aabbObForOctree.updateAABBObAxis(halfSxisVec);
}

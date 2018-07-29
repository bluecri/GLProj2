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

void OBBCollisionComp::updateCollisionComp()
{
	_bDirty = false;

	if (_bAxisModified)
	{
		_bDirty = true;
	}

	// no need to update _worldMat
	if (!(_rigidComp->isDirty()) && !_bPosModified && !_bRotateModified)		//opt : rigid dirty check only pos
	{
		return;
	}

	_worldMat = _rigidComp->getWorldMatRef() * _localMat;
	_aabbObForOctree.updateAABBObCenter(_worldMat[3]);
	_bDirty = true;

	return;
}

bool OBBCollisionComp::collideTestToOther(CollisionComponent * comp)
{
	switch (comp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
	case COLLISION_TYPE::COLLISION_OBB:
		return sIsBoxCollisionCheck(_worldMat, comp->getWorldMatRef(), _axisLen, comp->getAxisLenForAABBRef());
		break;

	default:
		printf_s("[LOG] : default case inAABBCollisionComp:: collideTestToOther\n");
		return false;
	}

	return false;
}

void OBBCollisionComp::setAxisLen(vec3& halfSxisVec)
{
	_bAxisModified = true;
	_axisTotalLen = glm::length(halfSxisVec);
	_aabbObForOctree.updateAABBObAxis(_axisTotalLen);
}

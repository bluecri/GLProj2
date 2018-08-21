#include "stdafx.h"
#include "OBBCollisionComp.h"
#include "AABBCollisionComp.h"
#include "RigidbodyComponent.h"

#include "DynamicCollisionSubCompVec.h"

AABBCollisionComp::AABBCollisionComp(RigidbodyComponent * rigidComp, glm::vec3 & localVec, glm::vec3 & axisLen, bool bDynamic = false)
	: CollisionComponent(rigidComp)
{
	collisionType = COLLISION_TYPE::COLLISION_AABB;
	_localVec = localVec;
	_aabb.updateAABBObAxis(axisLen);

	if (bDynamic)
		_dynamicSubComp = new DynamicCollisionSubCompVec();
}

void AABBCollisionComp::updateCollisionComp()
{
	//No test _bRotateModified : AABB

	// no need to update _worldMat
	if (!_rigidComp->isDirtyAll() && !_bDirty)
	{
		return;
	}
	_bDirty = true;

	const glm::mat4& worldMatRef = _rigidComp->getWorldMatRef();

	// if this is AABB.. only update pos
	glm::vec3 tempWorldVec;
	for (int i = 0; i < 3; i++)
	{
		tempWorldVec[i] = worldMatRef[3][i] + _localVec[i];
	}

	_aabb.updateAABBObCenter(tempWorldVec);

}

bool AABBCollisionComp::collideTestToOther(CollisionComponent * comp)
{
	switch (comp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
	case COLLISION_TYPE::COLLISION_OBB:
	case COLLISION_TYPE::COLLISION_LINE:
	case COLLISION_TYPE::COLLISION_SPHERE:
		return sIsBoxCollisionCheck(_worldMat, comp->getWorldMatRef(), _aabbObForOctree.getAxisConstRef(), comp->getAxisLenForAABBRef());
		break;

	default:
		printf_s("[LOG] : default case inAABBCollisionComp:: collideTestToOther\n");
		return false;
	}

	return false;
}

void AABBCollisionComp::updateDynamicLap()
{
	DynamicCollisionSubCompVec* prevWorldSubVec = getDynamicSubCompVec();
	prevWorldSubVec->updateLapWithAABBOb(_aabb);
}

void AABBCollisionComp::updateAABBObAxis(glm::vec3 & halfAxisSize)
{
	_bDirty = true;
	_aabb.updateAABBObAxis(halfAxisSize);
}

void AABBCollisionComp::updateAABBObAxis(float & halfAxisSize)
{
	_bDirty = true;
	_aabb.updateAABBObAxis(halfAxisSize);
}

{
	_bDirty = true;
	_aabb.updateAABBObCenter(center);
}


void AABBCollisionComp::setLocalVec(glm::vec3 &localVec)
{
	_localVec = localVec;
}

const AABBOb & AABBCollisionComp::getAABBConstRef()
{
	return _aabb;
}

DynamicCollisionSubCompVec * AABBCollisionComp::getDynamicSubCompVec()
{
	return static_cast<DynamicCollisionSubCompVec*>(_dynamicSubComp);
}

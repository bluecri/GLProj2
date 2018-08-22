#include "stdafx.h"
#include "OBBCollisionComp.h"
#include "AABBCollisionComp.h"
#include "RigidbodyComponent.h"


OBBCollisionComp::OBBCollisionComp(RigidbodyComponent * rigidComp, glm::mat4 & localMat, glm::vec3 & axisLen)
	: CollisionComponent(rigidComp)
{
	collisionType = COLLISION_TYPE::COLLISION_OBB;

	_localMat = localMat;
	_obb.updateOBBObAxis(axisLen);
	_aabb.updateAABBObAxis(glm::length(axisLen));
}

void OBBCollisionComp::updateCollisionComp()
{
	// no need to update _worldMat
	if (!(_rigidComp->isDirtyAll()) && !_bDirty)
	{
		return;
	}

	glm::mat4 tempWorldMat = _rigidComp->getWorldMatRef() * _localMat;
	_obb.updateOBBObMat(tempWorldMat);
	_aabb.updateAABBObCenter(tempWorldMat);

	_bDirty = true;

	return;
}

bool OBBCollisionComp::collideTestToOther(CollisionComponent * comp)
{
	switch (comp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
	case COLLISION_TYPE::COLLISION_OBB:
	case COLLISION_TYPE::COLLISION_SPHERE:
	case COLLISION_TYPE::COLLISION_LINE:
		return sIsBoxCollisionCheck(_worldMat, comp->getWorldMatRef(), _axisLen, comp->getAxisLenForAABBRef());
		break;

	default:
		printf_s("[LOG] : default case inAABBCollisionComp:: collideTestToOther\n");
		return false;
	}

	return false;
}

void OBBCollisionComp::updateOBBOb(const glm::mat4 & worldMat, const glm::vec3 & halfAxisSize)
{
	_bDirty = true;
	_obb.updateOBBOb(worldMat, halfAxisSize);
	_aabb.updateAABBObCenter(worldMat);
	_aabb.updateAABBObAxis(glm::length(halfAxisSize));
}

void OBBCollisionComp::updateOBBObAxis(const glm::vec3 & halfAxisSize)
{
	_bDirty = true;
	_obb.updateOBBObAxis(halfAxisSize);
	_aabb.updateAABBObAxis(glm::length(halfAxisSize));
}

void OBBCollisionComp::setLocalMat(glm::mat4 & localMat)
{
	_bDirty = true;
	_localMat = localMat;
}

void OBBCollisionComp::setLocalOnlyRotation(glm::mat4 & rotMat)
{
	_bDirty = true;
	for (int i = 0; i < 3; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			_localMat[i][k] = rotMat[i][k];
		}
	}
}

void OBBCollisionComp::setLocalPos(glm::vec3 & posVec)
{
	_bDirty = true;
	for (int i = 0; i < 3; i++)
		_localMat[3][i] = posVec[i];
}

void OBBCollisionComp::setLocalPos(glm::mat4 & posMat)
{
	_bDirty = true;
	for (int i = 0; i < 3; i++)
		_localMat[3][i] = posMat[3][i];
}

const AABBOb & OBBCollisionComp::getAABBObConstRef() const
{
	return _aabb;
}

const OBBOb & OBBCollisionComp::getOBBObConstRef() const
{
	return _obb;
}

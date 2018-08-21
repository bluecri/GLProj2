#include "SphereCollisionComp.h"
#include "RigidbodyComponent.h"

SphereCollisionComp::SphereCollisionComp(RigidbodyComponent * rigidComp, glm::vec3 & localVec, float radius)
{
	_sphereOb.updateSphereOb(radius);
	_rigidComp = rigidComp;
	_localPosVec = localVec;
}

void SphereCollisionComp::updateCollisionComp()
{
	if (!_rigidComp->isDirtyAll() && !_bDirty)
	{
		return;
	}
	_bDirty = true;

	const glm::mat4& worldMatRef = _rigidComp->getWorldMatRef();

	glm::vec3 tempWorldPos;

	for (int i = 0; i < 3; i++)
	{
		tempWorldPos[i] = worldMatRef[3][i] + _localPosVec[i];
	}

	_sphereOb.updateSphereOb(tempWorldPos);
}

bool SphereCollisionComp::collideTestToOther(CollisionComponent * comp)
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

void SphereCollisionComp::setLocalPosVec(glm::vec3 & localVec)
{
	_bDirty = true;
	_localPosVec = localVec;
}

void SphereCollisionComp::setSphereRadius(float radius)
{
	_bDirty = true;
	_sphereOb.updateSphereOb(radius);
}

const LineOb & SphereCollisionComp::getSphereObConstRef()
{
	return _sphereOb;
}

const glm::vec3 & SphereCollisionComp::getLocalVecConstRef()
{
	return _localPosVec;
}

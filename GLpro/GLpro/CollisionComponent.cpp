#include "stdafx.h"
#include "CollisionComponent.h"

CollisionComponent::CollisionComponent(RigidbodyComponent * rigidComp)
	: VectorPElem()
{
	_rigidComp = rigidComp;
	_bDeleted = false;
	_bCollisionTest = true;
	_bCollideVelocityUpdate = true;
	_bAlreadyVelocityUpdated = false;

	_collisionLogList = std::list<CollisionComponent*>();
}

const glm::mat4& CollisionComponent::getWorldMatRef()
{
	return _worldMat;
}

const glm::vec3& CollisionComponent::getAxisLenForAABBRef()
{
	return _aabbObForOctree.getAxisConstRef();
}

void CollisionComponent::setDeleted(bool bDeleted) 
{
	_bDeleted = bDeleted;
}

void CollisionComponent::setCollisionTest(bool bCollisionTest)
{
	_bCollisionTest = bCollisionTest;
}

void CollisionComponent::setCollisionVelocityUpdate(bool bVelUpdate)
{
	_bCollideVelocityUpdate = bVelUpdate;
}

void CollisionComponent::setLocalRotation(glm::mat4& rotMat)
{
	_bRotateModified = true;

	for (int i = 0; i < 3; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			_localMat[i][k] = rotMat[i][k];
		}
	}
}

void CollisionComponent::setLocalPos(glm::vec3& posVec)
{
	_bPosModified = true;

	for (int i = 0; i < 3; i++)
	{
		_localMat[3][i] = posVec[i];
	}
}

bool CollisionComponent::isAABBForOctreeDirty()
{
	return _aabbObForOctree.getDirty();
}

bool CollisionComponent::sIsBoxCollisionCheck(const glm::mat4 & wolrd1, const glm::mat4 & wolrd2, const glm::vec3 & axisLen1, const glm::vec3 & axisLen2)
{
	double c[3][3];	//ob1 3 axis vs otherOBB 3 axis consine value
	double absC[3][3];	//abs of c[][]
	double d[3];
	double r, r0, r1;	//r = 한 축을 기준으로 obBox 두 중심점의 거리
	const float cutoff = 0.9999f;
	bool existParallelPair = false;	//paralell 한가?

	glm::vec3 axisVec[3];
	glm::vec3 otheraxisVec[3];

	for (int i = 0; i < 3; i++)
	{
		axisVec[i] = wolrd1[i];
		otheraxisVec[i] = wolrd2[i];
	}
	glm::vec3 centerDiff = wolrd1[3] - wolrd2[3];

	//case COLLISION_TYPE::COLLISION_AABB:
	//case COLLISION_TYPE::COLLISION_OBB:

	//ob1 1 face vs otherOBB 3 faces
	for (int i = 0; i < 3; i++) {
		c[0][i] = glm::dot(axisVec[0], otheraxisVec[i]);
		absC[0][i] = glm::abs(c[0][i]);
		if (absC[0][i] > cutoff) {
			existParallelPair = true;
		}
	}

	d[0] = glm::dot(centerDiff, axisVec[0]);
	r = abs(d[0]);
	r0 = axisLen1[0];
	r1 = axisLen2[0] * absC[0][0] + axisLen2[1] * absC[0][1] + axisLen2[2] * absC[0][2];
	if (r > r0 + r1) {
		return false;
	}

	for (int i = 0; i < 3; i++) {
		c[1][i] = glm::dot(axisVec[1], otheraxisVec[i]);
		absC[1][i] = glm::abs(c[1][i]);
		if (absC[1][i] > cutoff) {
			existParallelPair = true;
		}
	}
	d[1] = glm::dot(centerDiff, axisVec[1]);
	r = abs(d[1]);
	r0 = axisLen1[1];
	r1 = axisLen2[1] * absC[1][0] + axisLen2[1] * absC[1][1] + axisLen2[2] * absC[1][2];
	if (r > r0 + r1) {
		return false;
	}

	for (int i = 0; i < 3; i++) {
		c[2][i] = glm::dot(axisVec[2], otheraxisVec[i]);
		absC[2][i] = glm::abs(c[2][i]);
		if (absC[2][i] > cutoff) {
			existParallelPair = true;
		}
	}
	d[2] = glm::dot(centerDiff, axisVec[2]);
	r = abs(d[2]);
	r0 = axisLen1[2];
	r1 = axisLen2[0] * absC[2][0] + axisLen2[1] * absC[2][1] + axisLen2[2] * absC[2][2];
	if (r > r0 + r1) {
		return false;
	}

	//ob1 3 face vs otherOBB 1 faces
	r = abs(glm::dot(centerDiff, otheraxisVec[0]));
	r0 = axisLen2[0];
	r1 = axisLen1[0] * absC[0][0] + axisLen1[1] * absC[1][0] + axisLen1[2] * absC[2][0];
	if (r > r0 + r1) {
		return false;
	}

	r = abs(glm::dot(centerDiff, otheraxisVec[1]));
	r0 = axisLen2[1];
	r1 = axisLen1[0] * absC[0][1] + axisLen1[1] * absC[1][1] + axisLen1[2] * absC[2][1];
	if (r > r0 + r1) {
		return false;
	}

	r = abs(glm::dot(centerDiff, otheraxisVec[2]));
	r0 = axisLen2[2];
	r1 = axisLen1[0] * absC[0][2] + axisLen1[1] * absC[1][2] + axisLen1[2] * absC[2][2];
	if (r > r0 + r1) {
		return false;
	}

	if (existParallelPair) {
		return true;	//pararell & r0, r1의 거리차가 존재하지 않음 => collision
	}

	//ob1의 axis와 otherOBB의 axis에 수직인 축을 기준으로 check. (3 axis * 3 axis)
	r = abs(d[2] * c[1][0] - d[1] * c[2][0]);
	r0 = axisLen1[1] * absC[2][0] + axisLen1[2] * absC[1][0];
	r1 = axisLen2[1] * absC[0][2] + axisLen1[2] * absC[0][1];
	if (r > r0 + r1) {
		return false;
	}
	r = abs(d[2] * c[1][1] - d[1] * c[2][1]);
	r0 = axisLen1[1] * absC[2][1] + axisLen1[2] * absC[1][1];
	r1 = axisLen2[0] * absC[0][2] + axisLen1[2] * absC[0][0];
	if (r > r0 + r1) {
		return false;
	}
	r = abs(d[2] * c[1][2] - d[1] * c[2][2]);
	r0 = axisLen1[1] * absC[2][2] + axisLen1[2] * absC[1][2];
	r1 = axisLen2[0] * absC[0][1] + axisLen1[2] * absC[0][0];
	if (r > r0 + r1) {
		return false;
	}

	r = abs(d[0] * c[2][0] - d[2] * c[0][0]);
	r0 = axisLen1[0] * absC[2][0] + axisLen1[2] * absC[0][0];
	r1 = axisLen2[1] * absC[1][2] + axisLen1[2] * absC[1][1];
	if (r > r0 + r1) {
		return false;
	}
	r = abs(d[0] * c[2][1] - d[2] * c[0][1]);
	r0 = axisLen1[0] * absC[2][1] + axisLen1[2] * absC[0][1];
	r1 = axisLen2[0] * absC[1][2] + axisLen1[2] * absC[1][0];
	if (r > r0 + r1) {
		return false;
	}
	r = abs(d[0] * c[2][2] - d[2] * c[0][2]);
	r0 = axisLen1[0] * absC[2][2] + axisLen1[2] * absC[0][2];
	r1 = axisLen2[0] * absC[1][1] + axisLen1[1] * absC[1][0];
	if (r > r0 + r1) {
		return false;
	}

	r = abs(d[1] * c[0][0] - d[0] * c[1][0]);
	r0 = axisLen1[0] * absC[1][0] + axisLen1[1] * absC[0][0];
	r1 = axisLen2[1] * absC[2][2] + axisLen1[2] * absC[2][1];
	if (r > r0 + r1) {
		return false;
	}
	r = abs(d[1] * c[0][1] - d[0] * c[1][1]);
	r0 = axisLen1[0] * absC[1][1] + axisLen1[1] * absC[0][1];
	r1 = axisLen2[0] * absC[2][2] + axisLen1[2] * absC[2][0];
	if (r > r0 + r1) {
		return false;
	}
	r = abs(d[1] * c[0][2] - d[0] * c[1][2]);
	r0 = axisLen1[0] * absC[1][2] + axisLen1[1] * absC[0][2];
	r1 = axisLen2[0] * absC[2][1] + axisLen1[1] * absC[2][0];
	if (r > r0 + r1) {
		return false;
	}
	return true;
}

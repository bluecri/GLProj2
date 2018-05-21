#include "stdafx.h"
#include "OBBCollisionComp.h"
#include "RigidbodyComponent.h"
#include "./src/Transform.h"

void OBBCollisionComp::updateWithRigidComp()
{
	// if this is AABB.. only update pos
	_worldlMat = _rigidComp->_transform->getWorldMatRef() * _localMat;
}

void OBBCollisionComp::updateAABBForOctree()
{
	// if this is AABB.. empty
	// axisVec; _axisLen; -> _center, _halfAxisSize update
	_center = _worldlMat[3];
	// todo _halfAxisSize
}

bool OBBCollisionComp::collideTestToOther(CollisionComponent * comp)
{
	double c[3][3];	//ob1 3 axis vs otherOBB 3 axis consine value
	double absC[3][3];	//abs of c[][]
	double d[3];
	double r, r0, r1;	//r = 한 축을 기준으로 obBox 두 중심점의 거리
	const float cutoff = 0.9999;
	bool existParallelPair = false;	//paralell 한가?

	OBBCollisionComp* otherOBB = static_cast<OBBCollisionComp*>(comp);

	glm::vec3 axisVec[3];
	glm::vec3 otheraxisVec[3];

	for (int i = 0; i < 3; i++)
	{
		axisVec[i] = _worldlMat[i];
		otheraxisVec[i] = otherOBB->_worldlMat[i];
	}
	glm::vec3 centerDiff = ((_worldlMat[3]) - (otherOBB->_worldlMat[3]));

	switch (comp->collisionType)
	{
	//case COLLISION_TYPE::COLLISION_AABB:
	case COLLISION_TYPE::COLLISION_OBB :
		
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
		r0 = _axisLen[0];
		r1 = otherOBB->_axisLen[0] * absC[0][0] + otherOBB->_axisLen[1] * absC[0][1] + otherOBB->_axisLen[2] * absC[0][2];
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
		r0 = _axisLen[1];
		r1 = otherOBB->_axisLen[1] * absC[1][0] + otherOBB->_axisLen[1] * absC[1][1] + otherOBB->_axisLen[2] * absC[1][2];
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
		r0 = _axisLen[2];
		r1 = otherOBB->_axisLen[0] * absC[2][0] + otherOBB->_axisLen[1] * absC[2][1] + otherOBB->_axisLen[2] * absC[2][2];
		if (r > r0 + r1) {
			return false;
		}

		//ob1 3 face vs otherOBB 1 faces
		r = abs(glm::dot(centerDiff, otheraxisVec[0]));
		r0 = otherOBB->_axisLen[0];
		r1 = _axisLen[0] * absC[0][0] + _axisLen[1] * absC[1][0] + _axisLen[2] * absC[2][0];
		if (r > r0 + r1) {
			return false;
		}

		r = abs(glm::dot(centerDiff, otheraxisVec[1]));
		r0 = otherOBB->_axisLen[1];
		r1 = _axisLen[0] * absC[0][1] + _axisLen[1] * absC[1][1] + _axisLen[2] * absC[2][1];
		if (r > r0 + r1) {
			return false;
		}

		r = abs(glm::dot(centerDiff, otheraxisVec[2]));
		r0 = otherOBB->_axisLen[2];
		r1 = _axisLen[0] * absC[0][2] + _axisLen[1] * absC[1][2] + _axisLen[2] * absC[2][2];
		if (r > r0 + r1) {
			return false;
		}

		if (existParallelPair) {
			return true;	//pararell & r0, r1의 거리차가 존재하지 않음 => collision
		}

		//ob1의 axis와 otherOBB의 axis에 수직인 축을 기준으로 check. (3 axis * 3 axis)
		r = abs(d[2] * c[1][0] - d[1] * c[2][0]);
		r0 = _axisLen[1] * absC[2][0] + _axisLen[2] * absC[1][0];
		r1 = otherOBB->_axisLen[1] * absC[0][2] + _axisLen[2] * absC[0][1];
		if (r > r0 + r1) {
			return false;
		}
		r = abs(d[2] * c[1][1] - d[1] * c[2][1]);
		r0 = _axisLen[1] * absC[2][1] + _axisLen[2] * absC[1][1];
		r1 = otherOBB->_axisLen[0] * absC[0][2] + _axisLen[2] * absC[0][0];
		if (r > r0 + r1) {
			return false;
		}
		r = abs(d[2] * c[1][2] - d[1] * c[2][2]);
		r0 = _axisLen[1] * absC[2][2] + _axisLen[2] * absC[1][2];
		r1 = otherOBB->_axisLen[0] * absC[0][1] + _axisLen[2] * absC[0][0];
		if (r > r0 + r1) {
			return false;
		}

		r = abs(d[0] * c[2][0] - d[2] * c[0][0]);
		r0 = _axisLen[0] * absC[2][0] + _axisLen[2] * absC[0][0];
		r1 = otherOBB->_axisLen[1] * absC[1][2] + _axisLen[2] * absC[1][1];
		if (r > r0 + r1) {
			return false;
		}
		r = abs(d[0] * c[2][1] - d[2] * c[0][1]);
		r0 = _axisLen[0] * absC[2][1] + _axisLen[2] * absC[0][1];
		r1 = otherOBB->_axisLen[0] * absC[1][2] + _axisLen[2] * absC[1][0];
		if (r > r0 + r1) {
			return false;
		}
		r = abs(d[0] * c[2][2] - d[2] * c[0][2]);
		r0 = _axisLen[0] * absC[2][2] + _axisLen[2] * absC[0][2];
		r1 = otherOBB->_axisLen[0] * absC[1][1] + _axisLen[1] * absC[1][0];
		if (r > r0 + r1) {
			return false;
		}

		r = abs(d[1] * c[0][0] - d[0] * c[1][0]);
		r0 = _axisLen[0] * absC[1][0] + _axisLen[1] * absC[0][0];
		r1 = otherOBB->_axisLen[1] * absC[2][2] + _axisLen[2] * absC[2][1];
		if (r > r0 + r1) {
			return false;
		}
		r = abs(d[1] * c[0][1] - d[0] * c[1][1]);
		r0 = _axisLen[0] * absC[1][1] + _axisLen[1] * absC[0][1];
		r1 = otherOBB->_axisLen[0] * absC[2][2] + _axisLen[2] * absC[2][0];
		if (r > r0 + r1) {
			return false;
		}
		r = abs(d[1] * c[0][2] - d[0] * c[1][2]);
		r0 = _axisLen[0] * absC[1][2] + _axisLen[1] * absC[0][2];
		r1 = otherOBB->_axisLen[0] * absC[2][1] + _axisLen[1] * absC[2][0];
		if (r > r0 + r1) {
			return false;
		}
		return true;
		break;
	default:
		return false;
		break;
	}
}

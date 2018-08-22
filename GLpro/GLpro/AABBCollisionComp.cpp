#include "stdafx.h"
#include "OBBCollisionComp.h"
#include "AABBCollisionComp.h"
#include "LineCollisionComp.h"
#include "SphereCollisionComp.h"
#include "RigidbodyComponent.h"

#include "DynamicCollisionSubCompVec.h"
#include "CollisionFuncStatic.h"

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
void AABBCollisionComp::updateDynamicLap()
{
	DynamicCollisionSubCompVec* subCompVec = getDynamicSubCompVec();
	subCompVec->updateLapWithAABBOb(_aabb);
}

void AABBCollisionComp::updateAABBObAxis(const glm::vec3 & halfAxisSize)
{
	_bDirty = true;
	_aabb.updateAABBObAxis(halfAxisSize);
}

void AABBCollisionComp::updateAABBObAxis(float halfAxisSize)
{
	_bDirty = true;
	_aabb.updateAABBObAxis(halfAxisSize);
}

void AABBCollisionComp::setLocalVec(const glm::vec3 &localVec)
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

bool AABBCollisionComp::collideStaticTestToOtherStatic(CollisionComponent * staticComp)
{
	switch (staticComp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
		return CollisionFuncStatic::staticCheck_AABB_AABB_OVERLAP(_aabb, static_cast<AABBCollisionComp*>(staticComp)->_aabb);
		break;
	case COLLISION_TYPE::COLLISION_OBB:
		return CollisionFuncStatic::staticCheck_AABB_OBB_OVERLAP(_aabb, static_cast<OBBCollisionComp*>(staticComp)->getOBBObConstRef());
		break;
	case COLLISION_TYPE::COLLISION_LINE:
		return CollisionFuncStatic::staticCheck_AABB_LINE_OVERLAP(_aabb, static_cast<LineCollisionComp*>(staticComp)->getLineObConstRef());
		break;
	case COLLISION_TYPE::COLLISION_SPHERE:
		return CollisionFuncStatic::staticCheck_SPHERE_AABB_OVERLAP(static_cast<SphereCollisionComp*>(staticComp)->getSphereObConstRef(), _aabb);
		break;

	default:
		printf_s("[LOG] : default case inAABBCollisionComp:: collideTestToOther\n");
		return false;
	}

	return false;
}

bool AABBCollisionComp::collideStaticTestToOtherDynamic(CollisionComponent * staticComp, float collideTime)
{
	return false;
}

bool AABBCollisionComp::collideDynamicTestToOtherDynamic(CollisionComponent * staticComp, float collideTime)
{
	return false;
}





bool AABBCollisionComp::lapStaticTestToOtherStatic(CollisionComponent * staticComp)
{
	switch (staticComp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
		return CollisionFuncStatic::staticCheck_AABB_AABB_OVERLAP(_aabb, static_cast<AABBCollisionComp*>(staticComp)->_aabb);

	case COLLISION_TYPE::COLLISION_OBB:
		return CollisionFuncStatic::staticCheck_AABB_AABB_OVERLAP(_aabb, static_cast<OBBCollisionComp*>(staticComp)->getAABBObConstRef());

	case COLLISION_TYPE::COLLISION_LINE:
	{
		AABBOb lineLap;
		CollisionFuncStatic::CreateAAABBLAP_LINE(static_cast<LineCollisionComp*>(staticComp)->getLineObConstRef(), lineLap);
		return CollisionFuncStatic::staticCheck_AABB_AABB_OVERLAP(lineLap, _aabb);
	}

	case COLLISION_TYPE::COLLISION_SPHERE:
	{
		AABBOb sphereLap;
		CollisionFuncStatic::CreateAAABBLAP_SPHERE(static_cast<SphereCollisionComp*>(staticComp)->getSphereObConstRef(), sphereLap);
		return CollisionFuncStatic::staticCheck_AABB_AABB_OVERLAP(sphereLap, _aabb);
	}

	default:
		printf_s("[LOG] : default case inAABBCollisionComp:: collideTestToOther\n");
		return false;
	}

	return false;
}

bool AABBCollisionComp::lapStaticTestToOtherDynamic(CollisionComponent * staticComp)
{
	return CollisionFuncStatic::staticCheck_AABB_AABB_OVERLAP(_aabb, staticComp->getDynamicSubComp()->getLapConstRef());
}

bool AABBCollisionComp::lapDynamicTestToOtherDynamic(CollisionComponent * staticComp)
{
	return CollisionFuncStatic::staticCheck_AABB_AABB_OVERLAP(getDynamicSubComp()->getLapConstRef(), staticComp->getDynamicSubComp()->getLapConstRef());
}



void AABBCollisionComp::resolveStaticStaticCollide(CollisionComponent * staticComp)
{
	/*
	elem->_bAlreadyVelocityUpdated = true;
	potentialComp->_bAlreadyVelocityUpdated = true;

	glm::vec3& v1 = elem->_rigidComp->getVelocityRef();
	glm::vec3& v2 = potentialComp->_rigidComp->getVelocityRef();
	float m1 = elem->_rigidComp->getMass();
	float m2 = potentialComp->_rigidComp->getMass();

	glm::vec3 v1Ret = v1 + m2*(v2 - v1) * 2.0f / (m1 + m2);
	glm::vec3 v2Ret = v2 + m2*(v1 - v2) * 2.0f / (m1 + m2);
	*/
}

void AABBCollisionComp::resolveDynamicStaticCollide(float time, CollisionComponent * staticComp)
{
	//todo : if no dynamic time collision, copy result(post) to prev
}

void AABBCollisionComp::resolveDynamicDynamicCollide(float time, CollisionComponent * dynamicComp)
{
	//todo : if no dynamic time collision, copy result(post) to prev
}

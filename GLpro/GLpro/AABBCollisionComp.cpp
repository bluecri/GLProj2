#include "stdafx.h"
#include "OBBCollisionComp.h"
#include "AABBCollisionComp.h"
#include "LineCollisionComp.h"
#include "SphereCollisionComp.h"
#include "RigidbodyComponent.h"

#include "DynamicCollisionSubCompVec.h"
#include "CollisionFuncStatic.h"
#include "CollisionFuncDynamic.h"

#include "src\window.h"

AABBCollisionComp::AABBCollisionComp(RigidbodyComponent * rigidComp, glm::vec3 & localVec, glm::vec3 & axisLen, bool bDynamic)
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

void AABBCollisionComp::modifyWorldAABB(const glm::vec3 worldVec)
{
	_aabb.updateAABBObCenter(worldVec);
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
		return CollisionFuncStatic::staticCheck_LINE_AABB(static_cast<LineCollisionComp*>(staticComp)->getLineObConstRef(), _aabb);
		break;
	case COLLISION_TYPE::COLLISION_SPHERE:
		return CollisionFuncStatic::staticCheck_SPHERE_AABB_OVERLAP(static_cast<SphereCollisionComp*>(staticComp)->getSphereObConstRef(), _aabb);
		break;

	default:
		printf_s("[LOG] : default case inAABBCollisionComp:: collideStaticTestToOtherStatic\n");
		return false;
	}

	return false;
}

bool AABBCollisionComp::collideStaticTestToOtherDynamic(CollisionComponent * dynamicComp, float& collideTime)
{
	switch (dynamicComp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
	{
		AABBCollisionComp* other = static_cast<AABBCollisionComp*>(dynamicComp);
		return CollisionFuncDynamic::dynamicCheck_DAABB_SAABB(other->getAABBConstRef(), other->getDynamicSubCompVec()->getPrevInfo(), _aabb, collideTime);
	}
	case COLLISION_TYPE::COLLISION_OBB:
	{
		OBBCollisionComp* other = static_cast<OBBCollisionComp*>(dynamicComp);
		return CollisionFuncDynamic::dynamicCheck_DOBB_SAABB(other->getOBBObConstRef(), _aabb, collideTime);
	}
	case COLLISION_TYPE::COLLISION_LINE:
	{
		LineCollisionComp* other = static_cast<LineCollisionComp*>(dynamicComp);
		return CollisionFuncDynamic::dynamicCheck_DLINE_SAABB(other->getLineObConstRef(), _aabb, collideTime);
	}
	case COLLISION_TYPE::COLLISION_SPHERE:
	{
		SphereCollisionComp* other = static_cast<SphereCollisionComp*>(dynamicComp);
		return CollisionFuncDynamic::dynamicCheck_DSPHERE_SAABB(other->getSphereObConstRef(), other->getDynamicSubCompVec()->getPrevInfo(), _aabb, collideTime);
	}
	default:
		printf_s("[LOG] : default case inAABBCollisionComp:: collideStaticTestToOtherDynamic\n");
		return false;
	}

	return false;
}

bool AABBCollisionComp::collideDynamicTestToOtherDynamic(CollisionComponent * dynamicComp, float& collideTime)
{
	printf_s("[LOG] : AABBCollisionComp::collideDynamicTestToOtherDynamic implemented but should not use..\n");
	switch (dynamicComp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
	{
		AABBCollisionComp* other = static_cast<AABBCollisionComp*>(dynamicComp);
		printf_s("[LOG] : AABBCollisionComp::collideDynamicTestToOtherDynamic COLLISION_AABB implemented but should not use..\n");
		return CollisionFuncDynamic::dynamicCheck_DAABB_DAABB(other->getAABBConstRef(), other->getDynamicSubCompVec()->getPrevInfo(), _aabb, getDynamicSubCompVec()->getPrevInfo(), collideTime);
	}
	case COLLISION_TYPE::COLLISION_OBB:
	{
		OBBCollisionComp* other = static_cast<OBBCollisionComp*>(dynamicComp);
		printf_s("[LOG] : AABBCollisionComp::collideDynamicTestToOtherDynamic COLLISION_OBB not implemented\n");
		return false;
		//return CollisionFuncDynamic::dynamicCheck_DAABB_DOBB(_aabb, getDynamicSubCompVec()->getPrevInfo(), other->getOBBObConstRef(), other->getDynamicSubCompMat()->getPrevInfo(),  collideTime);
	}
	case COLLISION_TYPE::COLLISION_LINE:
	{
		LineCollisionComp* other = static_cast<LineCollisionComp*>(dynamicComp);
		printf_s("[LOG] : AABBCollisionComp::collideDynamicTestToOtherDynamic COLLISION_LINE not implemented\n");
		return false;
	}
	case COLLISION_TYPE::COLLISION_SPHERE:
	{
		SphereCollisionComp* other = static_cast<SphereCollisionComp*>(dynamicComp);
		return CollisionFuncDynamic::dynamicCheck_DSPHERE_DAABB(other->getSphereObConstRef(), other->getDynamicSubCompVec()->getPrevInfo(), _aabb, getDynamicSubCompVec()->getPrevInfo(), collideTime);
	}
	default:
		printf_s("[LOG] : default case inAABBCollisionComp:: collideDynamicTestToOtherDynamic\n");
		return false;
	}

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
		printf_s("[LOG] : default case inAABBCollisionComp:: lapStaticTestToOtherStatic\n");
		return false;
	}

	return false;
}

bool AABBCollisionComp::lapStaticTestToOtherDynamic(CollisionComponent * dynamicComp)
{
	return CollisionFuncStatic::staticCheck_AABB_AABB_OVERLAP(_aabb, dynamicComp->getDynamicSubComp()->getLapConstRef());
}

bool AABBCollisionComp::lapDynamicTestToOtherDynamic(CollisionComponent * dynamicComp)
{
	return CollisionFuncStatic::staticCheck_AABB_AABB_OVERLAP(getDynamicSubComp()->getLapConstRef(), dynamicComp->getDynamicSubComp()->getLapConstRef());
}



void AABBCollisionComp::resolveStaticStaticCollide(CollisionComponent * staticComp)
{
	// if dynamic...
	//glm::vec3 collidePosition1 = glm::lerp(getDynamicSubCompVec()->getPrevInfo(), _aabb.getCenterConstRef(), time);
	//glm::vec3 collidePosition2 = glm::lerp(other->getDynamicSubCompVec()->getPrevInfo(), other->_aabb.getCenterConstRef(), time);
	
	glm::vec3 collidePosition1 = getAABBConstRef().getCenterConstRef();
	glm::vec3 collidePosition2;

	switch (staticComp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
	{
		AABBCollisionComp* other = static_cast<AABBCollisionComp*>(staticComp);
		collidePosition2 = other->getAABBConstRef().getCenterConstRef();
		break;
	}
	case COLLISION_TYPE::COLLISION_OBB:
	{
		OBBCollisionComp* other = static_cast<OBBCollisionComp*>(staticComp);
		collidePosition2 = glm::vec3(other->getOBBObConstRef().getMatConstRef()[3]);
		break;
	}
	case COLLISION_TYPE::COLLISION_LINE:
	{
		LineCollisionComp* other = static_cast<LineCollisionComp*>(staticComp);
		collidePosition2 = other->getLineObConstRef().getLineCenterPos();
		break;
	}
	case COLLISION_TYPE::COLLISION_SPHERE:
	{
		SphereCollisionComp* other = static_cast<SphereCollisionComp*>(staticComp);
		collidePosition2 = other->getSphereObConstRef().getCenterConstRef();
		break;
	}

	default:
		printf_s("[LOG] : default case AABBCollisionComp::resolveStaticStaticCollide\n");
		return;
	}

	glm::quat rotateQuat1, rotateQuat2;
	// collide center - center
	getRotateQuatAfterCollisionAndSetSpeed(staticComp, collidePosition1 - collidePosition2, rotateQuat1, rotateQuat2);

	resolveThisStatic(rotateQuat1);
	staticComp->resolveThisStatic(rotateQuat2);

	return;
}

void AABBCollisionComp::resolveDynamicStaticCollide(float time, CollisionComponent * staticComp)
{
	printf_s("[LOG] : AABBCollisionComp::resolveDynamicStaticCollide is implemented but should not use..\n");
	// if dynamic...
	//glm::vec3 collidePosition2 = glm::lerp(other->getDynamicSubCompVec()->getPrevInfo(), other->_aabb.getCenterConstRef(), time);

	glm::vec3 collidePosition1 = glm::lerp(getDynamicSubCompVec()->getPrevInfo(), _aabb.getCenterConstRef(), time);
	glm::vec3 collidePosition2;

	switch (staticComp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
	{
		AABBCollisionComp* other = static_cast<AABBCollisionComp*>(staticComp);
		collidePosition2 = other->getAABBConstRef().getCenterConstRef();
		break;
	}
	case COLLISION_TYPE::COLLISION_OBB:
	{
		OBBCollisionComp* other = static_cast<OBBCollisionComp*>(staticComp);
		collidePosition2 = glm::vec3(other->getOBBObConstRef().getMatConstRef()[3]);
		break;
	}
	case COLLISION_TYPE::COLLISION_LINE:
	{
		LineCollisionComp* other = static_cast<LineCollisionComp*>(staticComp);
		collidePosition2 = other->getLineObConstRef().getLineCenterPos();
		break;
	}
	case COLLISION_TYPE::COLLISION_SPHERE:
	{
		SphereCollisionComp* other = static_cast<SphereCollisionComp*>(staticComp);
		collidePosition2 = other->getSphereObConstRef().getCenterConstRef();
		break;
	}

	default:
		printf_s("[LOG] : default case AABBCollisionComp::resolveStaticStaticCollide\n");
		return;
	}

	glm::quat rotateQuat1, rotateQuat2;
	// collide center - center
	getRotateQuatAfterCollisionAndSetSpeed(staticComp, collidePosition1 - collidePosition2, rotateQuat1, rotateQuat2);

	resolveThisDynamic(collidePosition1, rotateQuat1);
	staticComp->resolveThisStatic(rotateQuat2);

	return;
}

void AABBCollisionComp::resolveDynamicDynamicCollide(float time, CollisionComponent * dynamicComp)
{
	printf_s("[LOG] : AABBCollisionComp::resolveDynamicDynamicCollide is implemented but should not use..\n");
	glm::vec3 collidePosition1 = glm::lerp(getDynamicSubCompVec()->getPrevInfo(), _aabb.getCenterConstRef(), time);
	glm::vec3 collidePosition2;

	switch (dynamicComp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
	{
		AABBCollisionComp* other = static_cast<AABBCollisionComp*>(dynamicComp);
		collidePosition2 = glm::lerp(other->getDynamicSubCompVec()->getPrevInfo(), other->_aabb.getCenterConstRef(), time);
		break;
	}
	case COLLISION_TYPE::COLLISION_OBB:
	{
		/*
		OBBCollisionComp* other = static_cast<OBBCollisionComp*>(dynamicComp);
		collidePosition2 = glm::lerp(glm::vec3(other->getDynamicSubCompMat()->getPrevInfo()[3]), glm::vec3(other->getOBBObConstRef().getMatConstRef()[3]), time);
		*/
		printf_s("[LOG] : AABBCollisionComp::resolveDynamicDynamicCollide COLLISION_OBB not implemented\n");
		return;
		break;
	}
	case COLLISION_TYPE::COLLISION_LINE:
	{
		/*
		LineCollisionComp* other = static_cast<LineCollisionComp*>(dynamicComp);
		collidePosition2 = glm::lerp(glm::vec3(other->getDynamicSubCompMat()->getPrevInfo()[3]), glm::vec3(other->getOBBObConstRef().getMatConstRef()[3]), time);
		*/
		printf_s("[LOG] : AABBCollisionComp::resolveDynamicDynamicCollide COLLISION_OBB not implemented\n");
		return;
		break;
	}
	case COLLISION_TYPE::COLLISION_SPHERE:
	{
		SphereCollisionComp* other = static_cast<SphereCollisionComp*>(dynamicComp);
		collidePosition2 = glm::lerp(glm::vec3(other->getDynamicSubCompVec()->getPrevInfo()[3]), glm::vec3(other->getSphereObConstRef().getCenterConstRef()), time);
		break;
	}

	default:
		printf_s("[LOG] : default case AABBCollisionComp::resolveStaticStaticCollide\n");
		return;
	}

	glm::quat rotateQuat1, rotateQuat2;
	// collide center - center
	getRotateQuatAfterCollisionAndSetSpeed(dynamicComp, collidePosition1 - collidePosition2, rotateQuat1, rotateQuat2);

	resolveThisDynamic(collidePosition1, rotateQuat1);
	dynamicComp->resolveThisDynamic(collidePosition2, rotateQuat2);

	return;
}

void AABBCollisionComp::saveDynamicRetCollideInfoToPrevInfo()
{
	printf_s("[LOG] : AABBCollisionComp::saveDynamicRetCollideInfoToPrevInfo is implemented but should not use..\n");
	getDynamicSubCompVec()->saveCollideInfoToPrevInfo(_aabb.getCenterConstRef());
}

void AABBCollisionComp::resolveThisStatic(glm::quat & rotateQuat)
{
	/*
	Rigidbody_worldMat * collisionLocalMat = collisionWorldMat
	collisionWorldMat is modified by collision & modified to collisionWorldMat' like localMat.
	Rigidbody_worldMat' * collisionLocalMat = collisionWorldMat'
	Rigidbody_worldMat' = collisionWorldMat' * collisionLocalMat^(-1)
	*/

	// get collision worldMat(rotate, transpos)
	glm::mat3 beforeCollisionWorldMatRot = glm::mat3(_rigidComp->getWorldMatRef());	// rigidWorldMat * collision local rotMat(glm::mat3)
	glm::vec3 beforeCollisionWorldMatVec = _aabb.getCenterConstRef();	// rigidWorldMat * collision local Vec

	// multiply fixed collision worldMat
	glm::mat4 afterCollisionWorldMat;
	afterCollisionWorldMat = glm::mat4(beforeCollisionWorldMatRot * glm::toMat3(rotateQuat));
	afterCollisionWorldMat[3] = glm::vec4(beforeCollisionWorldMatVec, 1);

	// adjust collision worldMat : static = update rotate, dynamic = update pos, rotate

	// make invisible Collsion worldMat multiplying inverse Collision local to invisible mat
	glm::mat4 collisionInvisivleMat = afterCollisionWorldMat;
	glm::mat4 collisionLocal = getCollisionLocalMat();
	
	collisionInvisivleMat *= glm::inverse(collisionLocal);

	// adjust with invisible mat to rigidbody
	_rigidComp->setQuaternion(collisionInvisivleMat);
	_rigidComp->setModelMatrix(glm::vec3(collisionInvisivleMat[3]));

	return;
}

void AABBCollisionComp::resolveThisDynamic(const glm::vec3 & collidePosition, glm::quat & rotateQuat)
{
	printf_s("[LOG] : AABBCollisionComp::resolveThisDynamic is implemented but should not use..\n");

	/*
	Rigidbody_worldMat * collisionLocalMat = collisionWorldMat
	collisionWorldMat is modified by collision & modified to collisionWorldMat' like localMat.
	Rigidbody_worldMat' * collisionLocalMat = collisionWorldMat'
	Rigidbody_worldMat' = collisionWorldMat' * collisionLocalMat^(-1)
	*/

	// get collision worldMat(rotate, transpos)
	glm::mat3 beforeCollisionWorldMatRot = glm::mat3(_rigidComp->getWorldMatRef()) * glm::mat3();	// rigidWorldMat * collision local rotMat(glm::mat3)
	//glm::vec3 beforeCollisionWorldMatVec = _aabb.getCenterConstRef();	// rigidWorldMat * collision local Vec

	// multiply fixed worldMat after collision
	glm::mat4 afterCollisionWorldMat;
	afterCollisionWorldMat = glm::mat4(beforeCollisionWorldMatRot * glm::toMat3(rotateQuat));
	afterCollisionWorldMat[3] = glm::vec4(collidePosition, 1);

	// adjust collision worldMat : static = update rotate, dynamic = update pos, rotate
	_aabb.updateAABBObCenter(collidePosition);	// = afterCollisionWorldMat[3]

	// if dynamic, save prevInfo(adjusted collision info) 
	saveDynamicRetCollideInfoToPrevInfo();

	// make invisible Collsion worldMat multiplying inverse Collision local to invisible mat
	glm::mat4 collisionInvisivleMat = afterCollisionWorldMat;
	glm::mat4 collisionLocal = getCollisionLocalMat();

	collisionInvisivleMat *= glm::inverse(collisionLocal);

	// adjust with invisible mat to rigidbody
	_rigidComp->setQuaternion(collisionInvisivleMat);
	_rigidComp->setModelMatrix(glm::vec3(collisionInvisivleMat[3]));

	return;
}

glm::mat4 AABBCollisionComp::getCollisionLocalMat()
{
	glm::mat4 collisionLocalRet = glm::mat4();
	collisionLocalRet[3] = glm::vec4(_localVec, 1);

	return collisionLocalRet;
}

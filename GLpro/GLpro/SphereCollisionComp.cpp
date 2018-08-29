#include "SphereCollisionComp.h"

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

SphereCollisionComp::SphereCollisionComp(RigidbodyComponent * rigidComp, glm::vec3 & localVec, float radius, bool bDynamic)
	: CollisionComponent(rigidComp)
{
	_sphereOb.updateSphereOb(radius);
	_rigidComp = rigidComp;
	_localPosVec = localVec;

	if (bDynamic)
		_dynamicSubComp = new DynamicCollisionSubCompVec();
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

void SphereCollisionComp::updateDynamicLap()
{
	DynamicCollisionSubCompVec* subCompVec = getDynamicSubCompVec();
	subCompVec->updateLapWithSphere(_sphereOb);
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

void SphereCollisionComp::modifyWorldSphereOb(const glm::vec3 & center)
{
	_sphereOb.updateSphereOb(center);
}

const SphereOb & SphereCollisionComp::getSphereObConstRef()
{
	return _sphereOb;
}

const glm::vec3 & SphereCollisionComp::getLocalVecConstRef()
{
	return _localPosVec;
}

DynamicCollisionSubCompVec * SphereCollisionComp::getDynamicSubCompVec()
{
	return static_cast<DynamicCollisionSubCompVec*>(_dynamicSubComp);
}

bool SphereCollisionComp::collideStaticTestToOtherStatic(CollisionComponent * staticComp)
{
	switch (staticComp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
		return CollisionFuncStatic::staticCheck_SPHERE_AABB_OVERLAP(_sphereOb, static_cast<AABBCollisionComp*>(staticComp)->getAABBConstRef());
		break;
	case COLLISION_TYPE::COLLISION_OBB:
		return CollisionFuncStatic::staticCheck_SPHERE_OBB(_sphereOb, static_cast<OBBCollisionComp*>(staticComp)->getOBBObConstRef());
		break;
	case COLLISION_TYPE::COLLISION_LINE:
		return CollisionFuncStatic::staticCheck_LINE_SPHERE(static_cast<LineCollisionComp*>(staticComp)->getLineObConstRef(), _sphereOb);
		break;
	case COLLISION_TYPE::COLLISION_SPHERE:
		return CollisionFuncStatic::staticCheck_SPHERE_SPHERE_OVERLAP(static_cast<SphereCollisionComp*>(staticComp)->getSphereObConstRef(), _sphereOb);
		break;

	default:
		printf_s("[LOG] : default case collideStaticTestToOtherStatic:: collideStaticTestToOtherStatic\n");
		return false;
	}

	return false;
}

bool SphereCollisionComp::collideStaticTestToOtherDynamic(CollisionComponent * dynamicComp, float& collideTime)
{
	switch (dynamicComp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
	{
		AABBCollisionComp* other = static_cast<AABBCollisionComp*>(dynamicComp);
		return CollisionFuncDynamic::dynamicCheck_DAABB_SSPHERE(other->getAABBConstRef(), other->getDynamicSubCompVec()->getPrevInfo(), _sphereOb, collideTime);
	}
	case COLLISION_TYPE::COLLISION_OBB:
	{
		OBBCollisionComp* other = static_cast<OBBCollisionComp*>(dynamicComp);
		return CollisionFuncDynamic::dynamicCheck_DOBB_SSPHERE(other->getOBBObConstRef(), _sphereOb, collideTime);
	}
	case COLLISION_TYPE::COLLISION_LINE:
	{
		LineCollisionComp* other = static_cast<LineCollisionComp*>(dynamicComp);
		return CollisionFuncDynamic::dynamicCheck_DLINE_SSPHERE(other->getLineObConstRef(), _sphereOb, collideTime);
	}
	case COLLISION_TYPE::COLLISION_SPHERE:
	{
		SphereCollisionComp* other = static_cast<SphereCollisionComp*>(dynamicComp);
		return CollisionFuncDynamic::dynamicCheck_DSPHERE_SSPHERE(other->getSphereObConstRef(), other->getDynamicSubCompVec()->getPrevInfo(), _sphereOb, collideTime);
	}
	default:
		printf_s("[LOG] : default case inAABBCollisionComp:: collideStaticTestToOtherDynamic\n");
		return false;
	}

	return false;
}

bool SphereCollisionComp::collideDynamicTestToOtherDynamic(CollisionComponent * dynamicComp, float& collideTime)
{
	switch (dynamicComp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
	{
		AABBCollisionComp* other = static_cast<AABBCollisionComp*>(dynamicComp);
		return CollisionFuncDynamic::dynamicCheck_DSPHERE_DAABB(_sphereOb, getDynamicSubCompVec()->getPrevInfo(), other->getAABBConstRef(), other->getDynamicSubCompVec()->getPrevInfo(), collideTime);
	}
	case COLLISION_TYPE::COLLISION_OBB:
	{
		OBBCollisionComp* other = static_cast<OBBCollisionComp*>(dynamicComp);
		printf_s("[LOG] : SphereCollisionComp::collideDynamicTestToOtherDynamic COLLISION_OBB not implemented\n");
		//return CollisionFuncDynamic::dynamicCheck_DSPHERE_DOBB(_sphereOb, getDynamicSubCompVec()->getPrevInfo(), other->getOBBObConstRef(), other->getDynamicSubCompMat(), collideTime);
	}
	case COLLISION_TYPE::COLLISION_LINE:
	{
		LineCollisionComp* other = static_cast<LineCollisionComp*>(dynamicComp);
		printf_s("[LOG] : SphereCollisionComp::collideDynamicTestToOtherDynamic COLLISION_LINE not implemented\n");
		return false;
	}
	case COLLISION_TYPE::COLLISION_SPHERE:
	{
		SphereCollisionComp* other = static_cast<SphereCollisionComp*>(dynamicComp);
		return CollisionFuncDynamic::dynamicCheck_DSPHERE_DSPHERE(other->getSphereObConstRef(), other->getDynamicSubCompVec()->getPrevInfo(), _sphereOb, getDynamicSubCompVec()->getPrevInfo(), collideTime);
	}
	default:
		printf_s("[LOG] : default case inAABBCollisionComp:: collideDynamicTestToOtherDynamic\n");
		return false;
	}

	return false;
}

bool SphereCollisionComp::lapStaticTestToOtherStatic(CollisionComponent * staticComp)
{
	AABBOb thisSphereLap;
	CollisionFuncStatic::CreateAAABBLAP_SPHERE(getSphereObConstRef(), thisSphereLap);

	switch (staticComp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
		return CollisionFuncStatic::staticCheck_AABB_AABB_OVERLAP(thisSphereLap, static_cast<AABBCollisionComp*>(staticComp)->getAABBConstRef());


	case COLLISION_TYPE::COLLISION_OBB:
		return CollisionFuncStatic::staticCheck_AABB_AABB_OVERLAP(thisSphereLap, static_cast<OBBCollisionComp*>(staticComp)->getAABBObConstRef());

	case COLLISION_TYPE::COLLISION_LINE:
	{
		AABBOb lineLap;
		CollisionFuncStatic::CreateAAABBLAP_LINE(static_cast<LineCollisionComp*>(staticComp)->getLineObConstRef(), lineLap);
		return CollisionFuncStatic::staticCheck_AABB_AABB_OVERLAP(lineLap, thisSphereLap);
	}

	case COLLISION_TYPE::COLLISION_SPHERE:
	{
		AABBOb sphereLap;
		CollisionFuncStatic::CreateAAABBLAP_SPHERE(static_cast<SphereCollisionComp*>(staticComp)->getSphereObConstRef(), sphereLap);
		return CollisionFuncStatic::staticCheck_AABB_AABB_OVERLAP(sphereLap, thisSphereLap);
	}

	default:
		printf_s("[LOG] : default case inAABBCollisionComp:: lapStaticTestToOtherStatic\n");
		return false;
	}

	return false;
}

bool SphereCollisionComp::lapStaticTestToOtherDynamic(CollisionComponent * dynamicComp)
{
	AABBOb thisSphereLap;
	CollisionFuncStatic::CreateAAABBLAP_SPHERE(getSphereObConstRef(), thisSphereLap);

	return CollisionFuncStatic::staticCheck_AABB_AABB_OVERLAP(thisSphereLap, dynamicComp->getDynamicSubComp()->getLapConstRef());
}

bool SphereCollisionComp::lapDynamicTestToOtherDynamic(CollisionComponent * dynamicComp)
{
	return CollisionFuncStatic::staticCheck_AABB_AABB_OVERLAP(getDynamicSubComp()->getLapConstRef(), dynamicComp->getDynamicSubComp()->getLapConstRef());
}

void SphereCollisionComp::resolveStaticStaticCollide(CollisionComponent * staticComp)
{
	glm::vec3 collidePosition1 = getSphereObConstRef().getCenterConstRef();
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

void SphereCollisionComp::resolveDynamicStaticCollide(float time, CollisionComponent * staticComp)
{
	glm::vec3 collidePosition1 = glm::lerp(getDynamicSubCompVec()->getPrevInfo(), getSphereObConstRef().getCenterConstRef(), time);
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

void SphereCollisionComp::resolveDynamicDynamicCollide(float time, CollisionComponent * dynamicComp)
{
	glm::vec3 collidePosition1 = glm::lerp(getDynamicSubCompVec()->getPrevInfo(), getSphereObConstRef().getCenterConstRef(), time);
	glm::vec3 collidePosition2;

	switch (dynamicComp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
	{
		AABBCollisionComp* other = static_cast<AABBCollisionComp*>(dynamicComp);
		collidePosition2 = glm::lerp(other->getDynamicSubCompVec()->getPrevInfo(), other->getAABBConstRef().getCenterConstRef(), time);
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

void SphereCollisionComp::saveDynamicRetCollideInfoToPrevInfo()
{
	getDynamicSubCompVec()->saveCollideInfoToPrevInfo(_sphereOb.getCenterConstRef());
	return;
}

void SphereCollisionComp::resolveThisStatic(glm::quat & rotateQuat)
{
	// get collision worldMat(rotate, transpos)
	glm::mat3 beforeCollisionWorldMatRot = glm::mat3(_rigidComp->getWorldMatRef());	// rigidWorldMat * collision local rotMat(glm::mat3)
	glm::vec3 beforeCollisionWorldMatVec = _sphereOb.getCenterConstRef();	// rigidWorldMat * collision local Vec

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

void SphereCollisionComp::resolveThisDynamic(const glm::vec3 & collidePosition, glm::quat & rotateQuat)
{
	// get collision worldMat(rotate, transpos)
	glm::mat3 beforeCollisionWorldMatRot = glm::mat3(_rigidComp->getWorldMatRef()) * glm::mat3();	// rigidWorldMat * collision local rotMat(glm::mat3)
																									//glm::vec3 beforeCollisionWorldMatVec = _aabb.getCenterConstRef();	// rigidWorldMat * collision local Vec

																									// multiply fixed worldMat after collision
	glm::mat4 afterCollisionWorldMat;
	afterCollisionWorldMat = glm::mat4(beforeCollisionWorldMatRot * glm::toMat3(rotateQuat));
	afterCollisionWorldMat[3] = glm::vec4(collidePosition, 1);

	// adjust collision worldMat : static = update rotate, dynamic = update pos, rotate
	_sphereOb.updateSphereOb(collidePosition);	// = afterCollisionWorldMat[3]

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

glm::mat4 SphereCollisionComp::getCollisionLocalMat()
{
	glm::mat4 collisionLocalRet = glm::mat4();
	collisionLocalRet[3] = glm::vec4(_localPosVec, 1);

	return collisionLocalRet;
}

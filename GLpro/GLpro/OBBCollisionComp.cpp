#include "stdafx.h"
#include "OBBCollisionComp.h"
#include "AABBCollisionComp.h"
#include "LineCollisionComp.h"
#include "SphereCollisionComp.h"
#include "RigidbodyComponent.h"

#include "DynamicCollisionSubCompMat.h"
#include "DynamicCollisionSubCompVec.h"
#include "CollisionFuncStatic.h"
#include "CollisionFuncDynamic.h"

OBBCollisionComp::OBBCollisionComp(RigidbodyComponent * rigidComp, glm::mat4 & localMat, glm::vec3 & axisLen)
	: CollisionComponent(rigidComp)
{
	collisionType = COLLISION_TYPE::COLLISION_OBB;

	_localMat = localMat;
	_obb.updateOBBObAxis(axisLen);
	_aabb.updateAABBObAxis(glm::length(axisLen));

	_dynamicSubComp = nullptr;
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

void OBBCollisionComp::updateOBBOb(const glm::mat4 & worldMat, const glm::vec3 & halfAxisSize)
{
	_bDirty = true;
	_obb.updateOBBOb(worldMat, halfAxisSize);
	_aabb.updateAABBObCenter(worldMat);
	_aabb.updateAABBObAxis(glm::length(halfAxisSize));
}

void OBBCollisionComp::modifyOBBObMat(const glm::mat4 & worldMat)
{
	_obb.updateOBBObMat(worldMat);
}

void OBBCollisionComp::updateOBBObAxis(const glm::vec3 & halfAxisSize)
{
	_bDirty = true;
	_obb.updateOBBObAxis(halfAxisSize);
	_aabb.updateAABBObAxis(glm::length(halfAxisSize));
}

void OBBCollisionComp::modifyWorld(const glm::mat4 & worldMat)
{
	_aabb.updateAABBObCenter(worldMat);
	_obb.updateOBBObMat(worldMat);
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

DynamicCollisionSubCompMat * OBBCollisionComp::getDynamicSubCompMat()
{
	return static_cast<DynamicCollisionSubCompMat*>(_dynamicSubComp);
}

void OBBCollisionComp::updateDynamicLap()
{
	DynamicCollisionSubCompMat* subCompMat = getDynamicSubCompMat();
	subCompMat->updateLapWithOBBOb(_obb);
}

bool OBBCollisionComp::collideStaticTestToOtherStatic(CollisionComponent * staticComp)
{
	switch (staticComp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
		return CollisionFuncStatic::staticCheck_AABB_OBB_OVERLAP(static_cast<AABBCollisionComp*>(staticComp)->getAABBConstRef(), _obb);
		break;
	case COLLISION_TYPE::COLLISION_OBB:
		return CollisionFuncStatic::staticCheck_OBB_OBB_OVERLAP(_obb, static_cast<OBBCollisionComp*>(staticComp)->getOBBObConstRef());
		break;
	case COLLISION_TYPE::COLLISION_LINE:
		return CollisionFuncStatic::staticCheck_LINE_OBB(static_cast<LineCollisionComp*>(staticComp)->getLineObConstRef(), _obb);
		break;
	case COLLISION_TYPE::COLLISION_SPHERE:
		return CollisionFuncStatic::staticCheck_SPHERE_OBB(static_cast<SphereCollisionComp*>(staticComp)->getSphereObConstRef(), _obb);
		break;

	default:
		printf_s("[LOG] : default case inAABBCollisionComp:: collideTestToOther\n");
		return false;
	}

	return false;
}

bool OBBCollisionComp::collideStaticTestToOtherDynamic(CollisionComponent * dynamicComp, float& collideTime)
{
	switch (dynamicComp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
	{
		AABBCollisionComp* other = static_cast<AABBCollisionComp*>(dynamicComp);
		return CollisionFuncDynamic::dynamicCheck_DAABB_SOBB(other->getAABBConstRef(), other->getDynamicSubCompVec()->getPrevInfo(), _obb, collideTime);
	}
	case COLLISION_TYPE::COLLISION_OBB:
	{
		printf_s("[LOG] : OBBCollisionComp::collideStaticTestToOtherDynamic COLLISION_OBB not implemented\n");
		return false;
	}
	case COLLISION_TYPE::COLLISION_LINE:
	{
		printf_s("[LOG] : OBBCollisionComp::collideStaticTestToOtherDynamic COLLISION_LINE not implemented\n");
		return false;
	}
	case COLLISION_TYPE::COLLISION_SPHERE:
	{
		SphereCollisionComp* other = static_cast<SphereCollisionComp*>(dynamicComp);
		return CollisionFuncDynamic::dynamicCheck_DSPHERE_SOBB(other->getSphereObConstRef(), other->getDynamicSubCompVec()->getPrevInfo(), _obb, collideTime);
	}
	default:
		printf_s("[LOG] : default case inAABBCollisionComp:: collideTestToOther\n");
		return false;
	}

	return false;
}

bool OBBCollisionComp::collideDynamicTestToOtherDynamic(CollisionComponent * dynamicComp, float& collideTime)
{
	printf_s("[LOG] : OBBCollisionComp::collideDynamicTestToOtherDynamic not implemented\n");
	return false;
}

bool OBBCollisionComp::lapStaticTestToOtherStatic(CollisionComponent * staticComp)
{
	switch (staticComp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
		return CollisionFuncStatic::staticCheck_AABB_AABB_OVERLAP(_aabb, static_cast<AABBCollisionComp*>(staticComp)->getAABBConstRef());


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

bool OBBCollisionComp::lapStaticTestToOtherDynamic(CollisionComponent * dynamicComp)
{
	return CollisionFuncStatic::staticCheck_AABB_AABB_OVERLAP(getAABBObConstRef(), dynamicComp->getDynamicSubComp()->getLapConstRef());
}

bool OBBCollisionComp::lapDynamicTestToOtherDynamic(CollisionComponent * dynamicComp)
{
	printf_s("[LOG] : OBBCollisionComp::lapDynamicTestToOtherDynamic not implemented\n");
	return false;
}

void OBBCollisionComp::resolveStaticStaticCollide(CollisionComponent * staticComp)
{

	glm::vec3 collidePosition1 = glm::vec3(getOBBObConstRef().getMatConstRef()[3]);
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

void OBBCollisionComp::resolveDynamicStaticCollide(float time, CollisionComponent * staticComp)
{
	printf_s("[LOG] : OBBCollisionComp::resolveDynamicStaticCollide not implemented\n");
}

void OBBCollisionComp::resolveDynamicDynamicCollide(float time, CollisionComponent * dynamicComp)
{
	printf_s("[LOG] : OBBCollisionComp::resolveDynamicDynamicCollide not implemented\n");
}

void OBBCollisionComp::saveDynamicRetCollideInfoToPrevInfo()
{
	//getDynamicSubCompMat()->saveCollideInfoToPrevInfo(_obb.getMatConstRef());
}

void OBBCollisionComp::resolveThisStatic(glm::quat & rotateQuat)
{
	// get collision worldMat(rotate, transpos)
	glm::mat3 beforeCollisionWorldMatRot = glm::mat3(_rigidComp->getWorldMatRef()) * glm::mat3(_localMat);	// rigidWorldMat * collision local rotMat(glm::mat3)
	glm::vec3 beforeCollisionWorldMatVec = glm::vec3(_obb.getMatConstRef()[3]);	// rigidWorldMat * collision local Vec

																		// multiply fixed collision worldMat
	glm::mat4 afterCollisionWorldMat;
	afterCollisionWorldMat = glm::mat4(beforeCollisionWorldMatRot * glm::toMat3(rotateQuat));
	afterCollisionWorldMat[3] = glm::vec4(beforeCollisionWorldMatVec, 1);

	// adjust collision worldMat : static = update rotate, dynamic = update pos, rotate
	_obb.updateOBBObMat(afterCollisionWorldMat);

	// make invisible Collsion worldMat multiplying inverse Collision local to invisible mat
	glm::mat4 collisionInvisivleMat = afterCollisionWorldMat;
	glm::mat4 collisionLocal = getCollisionLocalMat();

	collisionInvisivleMat *= glm::inverse(collisionLocal);

	// adjust with invisible mat to rigidbody
	_rigidComp->setQuaternion(collisionInvisivleMat);
	_rigidComp->setModelMatrix(glm::vec3(collisionInvisivleMat[3]));

	return;
}

void OBBCollisionComp::resolveThisDynamic(const glm::vec3 & collidePosition, glm::quat & rotateQuat)
{
	printf_s("[LOG] : OBBCollisionComp::resolveThisDynamic not implemented\n");
}

glm::mat4 OBBCollisionComp::getCollisionLocalMat()
{
	return _localMat;
}

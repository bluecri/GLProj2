#include "LineCollisionComp.h"

#include "stdafx.h"
#include "OBBCollisionComp.h"
#include "AABBCollisionComp.h"
#include "LineCollisionComp.h"
#include "SphereCollisionComp.h"
#include "RigidbodyComponent.h"

#include "DynamicCollisionSubCompLine.h"
#include "DynamicCollisionSubCompVec.h"

#include "DynamicCollisionSubCompLine.h"
#include "CollisionFuncStatic.h"
#include "CollisionFuncDynamic.h"

#include "src\window.h"

LineCollisionComp::LineCollisionComp(RigidbodyComponent * rigidComp, glm::vec3 & localPosVec, glm::vec3 & localUnitVec, float len, LINEOB_TYPE_ENUM lineType)
	: CollisionComponent(rigidComp), _lineOb(lineType)
{
	_rigidComp = rigidComp;
	_localPosVec = localPosVec;
	_localUnitVec = localUnitVec;
	_lineOb.setLineLen(len);

	_dynamicSubComp = new DynamicCollisionSubCompLine();
}

void LineCollisionComp::updateCollisionComp()
{
	if (!_rigidComp->isDirtyAll() && !_bDirty)
	{
		return;
	}
	_bDirty = true;

	const glm::mat4& worldMatRef = _rigidComp->getWorldMatRef();

	glm::vec3 tempWorldUnitVec;
	glm::vec3 tempWorldPos;

	for (int i = 0; i < 3; i++)
	{
		tempWorldPos[i] = worldMatRef[3][i] + _localPosVec[i];
	}

	for (int i = 0; i < 3; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			tempWorldUnitVec[i] = worldMatRef[k][i] * _localUnitVec[k];
		}
	}

	_lineOb.setStartPos(tempWorldPos);
	_lineOb.setNormalizedUnitVec(tempWorldUnitVec);
}

void LineCollisionComp::updateDynamicLap()
{
	DynamicCollisionSubCompLine* subCompVec = getDynamicSubCompLine();
	subCompVec->updateLapWithLineOb(_lineOb);
}

void LineCollisionComp::setLocalPosVec(glm::vec3 & localVec)
{
	_bDirty = true;
	_localPosVec = localVec;
}

void LineCollisionComp::setNormalizedUnitVec(glm::vec3 & normalizedUnitVec)
{
	_bDirty = true;
	_localUnitVec = normalizedUnitVec;
}

void LineCollisionComp::setLineLen(float & len)
{
	_bDirty = true;
	_lineOb.setLineLen(len);
}

void LineCollisionComp::setNotNormalizedUnitVec(glm::vec3 & notNormalizedUnitVec, bool isContainLen)
{
	_bDirty = true;
	_localUnitVec = glm::normalize(notNormalizedUnitVec);
	if (isContainLen)
	{
		_lineOb.setLineLen(glm::length(notNormalizedUnitVec));
	}
}

const LineOb & LineCollisionComp::getLineObConstRef()
{
	return _lineOb;
}

const glm::vec3 & LineCollisionComp::getLocalVecConstRef()
{
	return _localPosVec;
}

const glm::vec3 & LineCollisionComp::getUnitVecConstRef()
{
	return _localUnitVec;
}

DynamicCollisionSubCompLine * LineCollisionComp::getDynamicSubCompLine()
{
	return static_cast<DynamicCollisionSubCompLine*>(_dynamicSubComp);
}

void LineCollisionComp::saveDynamicRetCollideInfoToPrevInfo()
{
	// getDynamicSubCompLine()->saveCollideInfoToPrevInfo(_lineOb);
}

bool LineCollisionComp::collideStaticTestToOtherStatic(CollisionComponent * staticComp)
{
	switch (staticComp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
		return CollisionFuncStatic::staticCheck_LINE_AABB(_lineOb, static_cast<AABBCollisionComp*>(staticComp)->getAABBConstRef());
	case COLLISION_TYPE::COLLISION_OBB:
		return CollisionFuncStatic::staticCheck_LINE_OBB(_lineOb, static_cast<OBBCollisionComp*>(staticComp)->getOBBObConstRef());
	case COLLISION_TYPE::COLLISION_LINE:
		float dist;
		CollisionFuncStatic::staticCheck_DIST_LINE_LINE(_lineOb, static_cast<LineCollisionComp*>(staticComp)->getLineObConstRef(), dist);
		if (dist < 0.0001)
			return true;
		return false;
	case COLLISION_TYPE::COLLISION_SPHERE:
		return CollisionFuncStatic::staticCheck_LINE_SPHERE(_lineOb, static_cast<SphereCollisionComp*>(staticComp)->getSphereObConstRef());

	default:
		printf_s("[LOG] : default case inAABBCollisionComp:: collideStaticTestToOtherStatic\n");
		return false;
	}

	return false;
}

bool LineCollisionComp::collideStaticTestToOtherDynamic(CollisionComponent * dynamicComp, float& collideTime)
{
	switch (dynamicComp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
	{
		AABBCollisionComp* other = static_cast<AABBCollisionComp*>(dynamicComp);
		//return CollisionFuncDynamic::dynamicCheck_DLINE_SAABB(other->getAABBConstRef(), other->getDynamicSubCompVec()->getPrevInfo(), _aabb, collideTime);
		printf_s("[LOG] : LineCollisionComp:: collideStaticTestToOtherDynamic COLLISION_AABB not implemented\n");
		return false;
	}
	case COLLISION_TYPE::COLLISION_OBB:
	{
		OBBCollisionComp* other = static_cast<OBBCollisionComp*>(dynamicComp);
		printf_s("[LOG] : LineCollisionComp:: collideStaticTestToOtherDynamic COLLISION_OBB not implemented\n");
		return false;
	}
	case COLLISION_TYPE::COLLISION_LINE:
	{
		LineCollisionComp* other = static_cast<LineCollisionComp*>(dynamicComp);
		printf_s("[LOG] : LineCollisionComp:: collideStaticTestToOtherDynamic COLLISION_LINE not implemented\n");
		return false;
	}
	case COLLISION_TYPE::COLLISION_SPHERE:
	{
		SphereCollisionComp* other = static_cast<SphereCollisionComp*>(dynamicComp);
		return CollisionFuncDynamic::dynamicCheck_DSPHERE_SLINE(other->getSphereObConstRef(), other->getDynamicSubCompVec()->getPrevInfo(), _lineOb, collideTime);
	}
	default:
		printf_s("[LOG] : default case LineCollisionComp:: collideStaticTestToOtherDynamic\n");
		return false;
	}

	return false;
}

bool LineCollisionComp::collideDynamicTestToOtherDynamic(CollisionComponent * dynamicComp, float& collideTime)
{
	printf_s("[LOG] :LineCollisionComp::collideDynamicTestToOtherDynamic not implemneted\n");
	return false;
}

bool LineCollisionComp::lapStaticTestToOtherStatic(CollisionComponent * staticComp)
{
	AABBOb lapAABB;
	CollisionFuncStatic::CreateAAABBLAP_LINE(_lineOb, lapAABB);
	switch (staticComp->collisionType)
	{
	case COLLISION_TYPE::COLLISION_AABB:
		return CollisionFuncStatic::staticCheck_AABB_AABB_OVERLAP(lapAABB, static_cast<AABBCollisionComp*>(staticComp)->getAABBConstRef());


	case COLLISION_TYPE::COLLISION_OBB:
		return CollisionFuncStatic::staticCheck_AABB_AABB_OVERLAP(lapAABB, static_cast<OBBCollisionComp*>(staticComp)->getAABBObConstRef());

	case COLLISION_TYPE::COLLISION_LINE:
	{
		AABBOb lineLap;
		CollisionFuncStatic::CreateAAABBLAP_LINE(static_cast<LineCollisionComp*>(staticComp)->getLineObConstRef(), lineLap);
		return CollisionFuncStatic::staticCheck_AABB_AABB_OVERLAP(lineLap, lapAABB);
	}

	case COLLISION_TYPE::COLLISION_SPHERE:
	{
		AABBOb sphereLap;
		CollisionFuncStatic::CreateAAABBLAP_SPHERE(static_cast<SphereCollisionComp*>(staticComp)->getSphereObConstRef(), sphereLap);
		return CollisionFuncStatic::staticCheck_AABB_AABB_OVERLAP(sphereLap, lapAABB);
	}

	default:
		printf_s("[LOG] : default case LineCollisionComp:: lapStaticTestToOtherStatic\n");
		return false;
	}

	return false;
}

bool LineCollisionComp::lapStaticTestToOtherDynamic(CollisionComponent * dynamicComp)
{
	AABBOb lapAABB;
	CollisionFuncStatic::CreateAAABBLAP_LINE(_lineOb, lapAABB);
	return CollisionFuncStatic::staticCheck_AABB_AABB_OVERLAP(lapAABB, dynamicComp->getDynamicSubComp()->getLapConstRef());
}

bool LineCollisionComp::lapDynamicTestToOtherDynamic(CollisionComponent * dynamicComp)
{
	printf_s("[LOG] :LineCollisionComp::lapDynamicTestToOtherDynamic not implemneted\n");
	return false;
}

void LineCollisionComp::resolveStaticStaticCollide(CollisionComponent * staticComp)
{

	glm::vec3 collidePosition1 = (_lineOb.getStartPosConstRef() + _lineOb.getEndPos() )/ 2.0f;
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

void LineCollisionComp::resolveDynamicStaticCollide(float time, CollisionComponent * staticComp)
{
	printf_s("[LOG] :LineCollisionComp::resolveDynamicStaticCollide not implemneted\n");
}

void LineCollisionComp::resolveDynamicDynamicCollide(float time, CollisionComponent * dynamicComp)
{
	printf_s("[LOG] : LineCollisionComp::resolveDynamicDynamicCollide not implemented\n");
	return;
}

void LineCollisionComp::resolveThisStatic(glm::quat & rotateQuat)
{
	// get collision worldMat(rotate, transpos)
	glm::mat3 beforeCollisionWorldMatRot = glm::mat3(_rigidComp->getWorldMatRef());	// rigidWorldMat * collision local rotMat(glm::mat3)
	glm::vec3 beforeCollisionWorldMatVec = (_lineOb.getStartPosConstRef() + _lineOb.getEndPos()) / 2.0f;	// rigidWorldMat * collision local Vec

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

void LineCollisionComp::resolveThisDynamic(const glm::vec3 & collidePosition, glm::quat & rotateQuat)
{
	printf_s("[LOG] :LineCollisionComp::resolveThisDynamic not implemneted\n");
}

glm::mat4 LineCollisionComp::getCollisionLocalMat()
{
	glm::mat4 collisionLocalRet = glm::mat4();
	collisionLocalRet[3] = glm::vec4(_localPosVec, 1);

	return collisionLocalRet;
}

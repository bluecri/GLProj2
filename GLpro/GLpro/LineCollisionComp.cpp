#include "LineCollisionComp.h"
#include "RigidbodyComponent.h"


LineCollisionComp::LineCollisionComp(RigidbodyComponent * rigidComp, glm::vec3 & localPosVec, glm::vec3 & localUnitVec, float len, LINEOB_TYPE_ENUM lineType)
{
	_lineOb.setType(lineType);
	_rigidComp = rigidComp;
	_localPosVec = localPosVec;
	_localUnitVec = localUnitVec;
	_lineOb.setLineLen(len);
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
		tempWorldPos[i] = worldMatRef[3][i] + _localVec[i];
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

bool LineCollisionComp::collideTestToOther(CollisionComponent * comp)
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
		_lineOb.setLineLen(glm::length(notNormalizedUnitVec))
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

#pragma once
#include "stdafx.h"
#include "CollisionComponent.h"

#include "LineOb.h"

class LineCollisionComp : public CollisionComponent
{
public:
	LineCollisionComp(RigidbodyComponent* rigidComp, glm::vec3& localPosVec, glm::vec3& localUnitVec, float len, LINEOB_TYPE_ENUM lineType);

	// CollisionComponent을(를) 통해 상속됨
	virtual void updateCollisionComp() override;
	virtual bool collideTestToOther(CollisionComponent* comp) override;

	void		setLocalPosVec(glm::vec3& localVec);

	void		setNormalizedUnitVec(glm::vec3& normalizedUnitVec);
	void		setLineLen(float& len);
	void		setNotNormalizedUnitVec(glm::vec3& notNormalizedUnitVec, bool isContainLen = false);

	const LineOb&		getLineObConstRef();
	const glm::vec3&	getLocalVecConstRef();
	const glm::vec3&	getUnitVecConstRef();

private:
	LineOb		_lineOb;
	glm::vec3	_localPosVec;
	glm::vec3	_localUnitVec;
};
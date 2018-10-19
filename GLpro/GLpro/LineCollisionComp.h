#pragma once
#include "stdafx.h"
#include "CollisionComponent.h"

class DynamicCollisionSubCompLine;

#include "LineOb.h"

class LineCollisionComp : public CollisionComponent
{
public:
	LineCollisionComp(RigidbodyComponent* rigidComp, glm::vec3& localPosVec, glm::vec3& localUnitVec, float len, LINEOB_TYPE_ENUM lineType);

	// CollisionComponent을(를) 통해 상속됨
	virtual void updateCollisionComp() override;
	virtual void updateDynamicLap() override;

	void		setLocalPosVec(glm::vec3& localVec);
	void		setNormalizedUnitVec(glm::vec3& normalizedUnitVec);
	void		setLineLen(float& len);
	void		setNotNormalizedUnitVec(glm::vec3& notNormalizedUnitVec, bool isContainLen = false);

	const LineOb&		getLineObConstRef();
	const glm::vec3&	getLocalVecConstRef();
	const glm::vec3&	getUnitVecConstRef();
	DynamicCollisionSubCompLine* getDynamicSubCompLine();

	// CollisionComponent을(를) 통해 상속됨
	virtual bool collideStaticTestToOtherStatic(CollisionComponent * staticComp) override;
	virtual bool collideStaticTestToOtherDynamic(CollisionComponent * dynamicComp, float& collideTime) override;
	virtual bool collideDynamicTestToOtherDynamic(CollisionComponent * dynamicComp, float& collideTime) override;
	
	virtual bool lapStaticTestToOtherStatic(CollisionComponent * staticComp) override;
	virtual bool lapStaticTestToOtherDynamic(CollisionComponent * dynamicComp) override;
	virtual bool lapDynamicTestToOtherDynamic(CollisionComponent * dynamicComp) override;

	virtual void resolveStaticStaticCollide(CollisionComponent * staticComp) override;
	virtual void resolveDynamicStaticCollide(float time, CollisionComponent * staticComp) override;
	virtual void resolveDynamicDynamicCollide(float time, CollisionComponent * dynamicComp) override;

	virtual void saveDynamicRetCollideInfoToPrevInfo() override;
	virtual void resolveThisStatic(glm::quat & rotateQuat) override;
	virtual void resolveThisDynamic(const glm::vec3 & collidePosition, glm::quat & rotateQuat) override;

private:
	glm::mat4 getCollisionLocalMat();

private:
	LineOb		_lineOb;
	glm::vec3	_localPosVec;
	glm::vec3	_localUnitVec;

};
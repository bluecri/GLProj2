#pragma once
#include "stdafx.h"
#include "CollisionComponent.h"

#include "SphereOb.h"

class DynamicCollisionSubCompVec;

class SphereCollisionComp : public CollisionComponent
{
public:
	SphereCollisionComp(RigidbodyComponent * rigidComp, glm::vec3 & localVec, float radius, bool bDynamic = false);

	// CollisionComponent을(를) 통해 상속됨
	virtual void updateCollisionComp() override;
	virtual void updateDynamicLap() override;

	void		setLocalPosVec(glm::vec3& localVec);
	void		setSphereRadius(float radius);
	void		modifyWorldSphereOb(const glm::vec3& center);

	const SphereOb&		getSphereObConstRef();
	const glm::vec3&	getLocalVecConstRef();
	DynamicCollisionSubCompVec* getDynamicSubCompVec();

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
	SphereOb		_sphereOb;
	glm::vec3		_localPosVec;
};
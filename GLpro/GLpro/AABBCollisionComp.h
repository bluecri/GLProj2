#pragma once
#include "stdafx.h"
#include "CollisionComponent.h"
#include "AABBOb.h"

class DynamicCollisionSubCompVec;

class AABBCollisionComp : public CollisionComponent
{
public:
	AABBCollisionComp(RigidbodyComponent* rigidComp, glm::vec3& localVec, glm::vec3& axisLen, bool bDynamic = false);

	// CollisionComponent을(를) 통해 상속됨
	virtual void updateCollisionComp() override;
	virtual void updateDynamicLap() override;

	void	updateAABBObAxis(const glm::vec3& halfAxisSize);
	void	updateAABBObAxis(float halfAxisSize);
	void	setLocalVec(const glm::vec3& localVec);

	const AABBOb& getAABBConstRef();
	DynamicCollisionSubCompVec* getDynamicSubCompVec();

	// CollisionComponent을(를) 통해 상속됨
	virtual bool collideStaticTestToOtherStatic(CollisionComponent * staticComp) override;
	virtual bool collideStaticTestToOtherDynamic(CollisionComponent * staticComp, float collideTime) override;
	virtual bool collideDynamicTestToOtherDynamic(CollisionComponent * staticComp, float collideTime) override;

	virtual bool lapStaticTestToOtherStatic(CollisionComponent * staticComp) override;
	virtual bool lapStaticTestToOtherDynamic(CollisionComponent * staticComp) override;
	virtual bool lapDynamicTestToOtherDynamic(CollisionComponent * staticComp) override;

	virtual void resolveStaticStaticCollide(CollisionComponent * staticComp) override;
	virtual void resolveDynamicStaticCollide(float time, CollisionComponent * staticComp) override;
	virtual void resolveDynamicDynamicCollide(float time, CollisionComponent * dynamicComp) override;

private:
	AABBOb _aabb;

	// collision component local matrix (trans & rotate)
	glm::vec3 _localVec;

};
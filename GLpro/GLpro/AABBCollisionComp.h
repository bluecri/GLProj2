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
	void	modifyWorldAABB(const glm::vec3 worldVec);

	const AABBOb& getAABBConstRef();
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
	AABBOb _aabb;

	// collision component local matrix (trans & rotate)
	glm::vec3 _localVec;



};
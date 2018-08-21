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
	virtual bool collideTestToOther(CollisionComponent* comp) override;
	virtual void updateDynamicLap() override;

	void	updateAABBObAxis(glm::vec3& halfAxisSize);
	void	updateAABBObAxis(float& halfAxisSize);

	void setLocalVec(glm::vec3& localVec);

	const AABBOb& getAABBConstRef();
	DynamicCollisionSubCompVec* getDynamicSubCompVec();

private:
	AABBOb _aabb;
	glm::vec3 _localVec;	// collision component local matrix (trans & rotate)
};
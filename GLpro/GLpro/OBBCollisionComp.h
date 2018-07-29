#pragma once
#include "stdafx.h"
#include "CollisionComponent.h"

class OBBCollisionComp : public CollisionComponent
{
public:
	OBBCollisionComp(RigidbodyComponent* rigidComp, glm::mat4& localMat, glm::vec3& axisLen);

	// CollisionComponent을(를) 통해 상속됨
	virtual void updateCollisionComp() override;
	virtual bool collideTestToOther(CollisionComponent* comp) override;
	virtual void setAxisLen(vec3& halfSxisVec) override;

private:
	float _axisTotalLen;	// for aabb axis len
	glm::vec3 _axisLen;		// collision box axis len
};
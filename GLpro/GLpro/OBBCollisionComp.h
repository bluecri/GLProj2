#pragma once
#include "stdafx.h"
#include "CollisionComponent.h"

#include "OBBOb.h"
#include "AABBOb.h"

class OBBCollisionComp : public CollisionComponent
{
public:
	OBBCollisionComp(RigidbodyComponent* rigidComp, glm::mat4& localMat, glm::vec3& axisLen);

	// CollisionComponent을(를) 통해 상속됨
	virtual void updateCollisionComp() override;
	virtual bool collideTestToOther(CollisionComponent* comp) override;

	void	updateOBBOb(const glm::mat4 & worldMat, const glm::vec3 & halfAxisSize);
	void	updateOBBObAxis(const glm::vec3& halfAxisSize);


	void setLocalMat(glm::mat4& localMat);
	void setLocalOnlyRotation(glm::mat4& rotMat);
	void setLocalPos(glm::vec3& posVec);
	void setLocalPos(glm::mat4& posMat);

	const AABBOb& getAABBObConstRef() const;
	const OBBOb& getOBBObConstRef() const;

private:
	AABBOb _aabb;	// 3 same axis
	OBBOb _obb;

	glm::mat4 _localMat;
};
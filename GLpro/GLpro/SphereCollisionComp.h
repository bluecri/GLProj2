#pragma once
#include "stdafx.h"
#include "CollisionComponent.h"

#include "SphereOb.h"

class SphereCollisionComp : public CollisionComponent
{
public:
	SphereCollisionComp(RigidbodyComponent * rigidComp, glm::vec3 & localVec, float radius);

	// CollisionComponent을(를) 통해 상속됨
	virtual void updateCollisionComp() override;
	virtual bool collideTestToOther(CollisionComponent* comp) override;

	void		setLocalPosVec(glm::vec3& localVec);
	void		setSphereRadius(float radius);

	const SphereOb&		getSphereObConstRef();
	const glm::vec3&	getLocalVecConstRef();

private:
	SphereOb		_sphereOb;
	glm::vec3		_localPosVec;
};
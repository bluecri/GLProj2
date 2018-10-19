#pragma once
#include "stdafx.h"
#include "DynamicCollisionSubComp.h"

class AABBOb;
class SphereOb;

class DynamicCollisionSubCompVec : public DynamicCollisionSubComp
{
public:
	DynamicCollisionSubCompVec()
		: DynamicCollisionSubComp()
	{

	}

	void updateLapWithSphere(SphereOb& spherePost);
	void updateLapWithAABBOb(AABBOb& aabbPost);
	const glm::vec3& getPrevInfo()
	{
		return _prevWorldVec;
	}
	void setPrevInfo(const glm::vec3 prev)
	{
		_prevWorldVec = prev;
	}
public:
	glm::vec3 _prevWorldVec;

	void saveCollideInfoToPrevInfo(const glm::vec3& collsionWorldVec);	// called by collisionComponent
};
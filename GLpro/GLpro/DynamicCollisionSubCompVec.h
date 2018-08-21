#pragma once
#include "stdafx.h"
#include "DynamicCollisionSubComp.h"

class AABBOb;

class DynamicCollisionSubCompVec : public DynamicCollisionSubComp
{
public:
	DynamicCollisionSubCompVec()
		: DynamicCollisionSubComp()
	{

	}

	void updateLapWithAABBOb(AABBOb& aabbPost);

public:
	glm::vec3 _prevWorldVec;
};
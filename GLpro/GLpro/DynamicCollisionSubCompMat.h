#pragma once
#include "stdafx.h"
#include "DynamicCollisionSubComp.h"

class DynamicCollisionSubCompMat : public DynamicCollisionSubComp
{
public:
	DynamicCollisionSubCompMat()
		: DynamicCollisionSubComp()
	{

	}

public:
	glm::mat4 _prevWorldMat;
};
#pragma once
#include "stdafx.h"
#include "DynamicCollisionSubComp.h"

class OBBOb;

class DynamicCollisionSubCompMat : public DynamicCollisionSubComp
{
public:
	DynamicCollisionSubCompMat()
		: DynamicCollisionSubComp()
	{
		printf_s("[LOG] : DynamicCollisionSubCompMat::DynamicCollisionSubCompMat should not use\n");
	}
	void updateLapWithOBBOb(OBBOb& obbPost);
	const glm::mat4& getPrevInfo()
	{
		return _prevWorldMat;
	}
	void setPrevInfo(const glm::mat4& prev)
	{
		_prevWorldMat = prev;
	}
public:
	glm::mat4 _prevWorldMat;
};
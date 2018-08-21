#pragma once
#include "stdafx.h"
#include "DynamicCollisionSubComp.h"
#include "LineOb.h"

class DynamicCollisionSubCompLine : public DynamicCollisionSubComp
{
public:
	DynamicCollisionSubCompLine()
		: DynamicCollisionSubComp(), _prevLineOb()
	{

	}

public:
	LineOb _prevLineOb;
};
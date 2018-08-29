#pragma once
#include "stdafx.h"
#include "DynamicCollisionSubComp.h"
#include "LineOb.h"

class DynamicCollisionSubCompLine : public DynamicCollisionSubComp
{
public:
	DynamicCollisionSubCompLine()
		: DynamicCollisionSubComp(), _prevLineOb(LINEOB_TYPE_ENUM::LINEOB_TYPE_LEN)
	{
		printf_s("[LOG] : DynamicCollisionSubCompLine::DynamicCollisionSubCompLine should not use\n");
	}

	void updateLapWithLineOb(LineOb& linePost);
	const LineOb& getPrevInfo()
	{
		return _prevLineOb;
	}
	void setPrevInfo(const LineOb& prev)
	{
		_prevLineOb = prev;
	}
public:
	LineOb _prevLineOb;
};
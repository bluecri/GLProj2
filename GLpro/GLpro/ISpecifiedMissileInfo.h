#pragma once

#include "stdafx.h"
#include "EnumMissileType.h"

class ISpecifiedMissileInfo {
public:
	virtual void init() = 0;
	virtual void init(std::string & specifiedMissileInfo) = 0;
};
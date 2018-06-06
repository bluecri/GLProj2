#pragma once

#include "stdafx.h"
#include "EnumMissileType.h"

class SpecifiedNormalMissileState;

class ISpecifiedMissileInfo {
public:
	virtual void initISpecifiedMissileInfo() = 0;
	virtual void initISpecifiedMissileInfo(std::string & specifiedMissileInfo) = 0;

	// double dispatch
	virtual void doubleDispatchInit(SpecifiedNormalMissileState* normaState);
};
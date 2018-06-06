#pragma once

class ISpecifiedMissileInfo;

class ISpecifiedMissileState
{
public:
	virtual int getType() = 0;
	virtual void initSpecifiedState(ISpecifiedMissileInfo* info) = 0;
};
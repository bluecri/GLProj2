#pragma once

class ISpecifiedMissileInfo;
class BuffSum;

class ISpecifiedMissileState
{
public:
	virtual int getType() = 0;
	virtual void initSpecifiedState(ISpecifiedMissileInfo* info) = 0;
	virtual void transferBuffSum(BuffSum* buffSum, ISpecifiedMissileState* originState) = 0;
};
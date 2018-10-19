#pragma once

#include "ICommonMissileInfo.h"

class CommonNormalMissileInfo : public ICommonMissileInfo
{
public:
	// ICommonMissileInfo을(를) 통해 상속됨
	virtual void initICommonMissileInfo() override;
	virtual void initICommonMissileInfo(std::string & commonMissileInfoFile) override;
};
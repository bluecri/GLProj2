#pragma once

#include "ICommonMissileInfo.h"

class CommonNormalMissileInfo : public ICommonMissileInfo
{
public:
	// ICommonMissileInfo을(를) 통해 상속됨
	virtual void init() override;
	virtual void init(std::string & commonMissileInfoFile) override;
};
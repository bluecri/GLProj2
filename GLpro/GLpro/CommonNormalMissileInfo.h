#pragma once

#include "ICommonMissileInfo.h"

class CommonNormalMissileInfo : public ICommonMissileInfo
{
public:
	// ICommonMissileInfo��(��) ���� ��ӵ�
	virtual void init() override;
	virtual void init(std::string & commonMissileInfoFile) override;
};
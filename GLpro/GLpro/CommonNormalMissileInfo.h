#pragma once

#include "ICommonMissileInfo.h"

class CommonNormalMissileInfo : public ICommonMissileInfo
{
public:
	// ICommonMissileInfo��(��) ���� ��ӵ�
	virtual void initICommonMissileInfo() override;
	virtual void initICommonMissileInfo(std::string & commonMissileInfoFile) override;
};
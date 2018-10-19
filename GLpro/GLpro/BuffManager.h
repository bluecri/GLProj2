#pragma once
#include "stdafx.h"
#include "IBuff.h"
#include "IFieldItemForBuff.h"

class BuffManager
{
public:
	using BuffListElem = std::pair<IBuff*, bool>;	// iBuff, isChained to _lastBuffPushedMap

	BuffManager();
	virtual ~BuffManager();
	int addNewBuff(ENUM_FieldItemForBuff_TYPE fieldItemForBuffType, IBuff * buff);

	// buff를 순회하며 buffSum 생성. 갱신된 경우 return true
	bool isNeedToTransferBuffSum(float deltaTime, float acc);

	BuffSum* getBuffSum();

public:
	BuffSum*					_buffSum;
	std::list<BuffListElem>		_buffList;
	float						_minBuffDuration;
	std::map<ENUM_BUFF_TYPE, std::list<BuffListElem>::iterator>	_lastBuffPushedMap;

private:
};
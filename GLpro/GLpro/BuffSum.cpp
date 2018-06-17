#include "stdafx.h"
#include "BuffSum.h"

BuffSum::BuffSum()
{
	bModify = std::vector<bool>(ENUM_BUFFSUM_MODIFY_NUM, false);

	mult = std::vector<float>(ENUM_BUFFSUM_MODIFY_NUM, 0.0f);
	acc = std::vector<float>(ENUM_BUFFSUM_MODIFY_NUM, 0.0f);

	bOnce = std::vector<bool>(ENUM_BUFFSUM_ONCE_NUM, false);
}

void BuffSum::initBuffSum()
{
	for (auto& elem : bModify)
	{
		elem = false;
	}

	for (int i = 0; i < ENUM_BUFFSUM_MODIFY_NUM; i++)
	{
		mult[i] = 0.0f;
		acc[i] = 0.0f;
	}

	for (auto& elem : bOnce)
	{
		elem = false;
	}
}

void BuffSum::partialInitBuffSum()
{
	for (int i = 0; i < ENUM_BUFFSUM_MODIFY_NUM; i++)
	{
		if (bModify[i] == true)
		{
			mult[i] = 0.0f;
			acc[i] = 0.0f;
		}
	}

	for (auto& elem : bOnce)
	{
		elem = false;
	}
}

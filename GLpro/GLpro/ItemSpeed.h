#pragma once

#include "IFieldItemForBuff.h"

class ItemSpeed : public IFieldItemForBuff
{
public:
	ItemSpeed(ENUM_FieldItemForBuff_TYPE fieldItemForBuffType, GameSession * gSession, glm::vec3& genPos);
	virtual ~ItemSpeed();
};
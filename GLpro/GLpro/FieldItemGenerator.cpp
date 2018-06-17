#include "stdafx.h"
#include "FieldItemGenerator.h"

#include "ItemSpeed.h"

void FieldItemGenerator::initFieldItemGenerator(GameSession* gSession)
{
	_bindedGameSession = gSession;
}

// itemType ; buff

IFieldItemForBuff * FieldItemGenerator::genItemForBuff(ENUM_ITEMBUFF_TYPE buffType, ENUM_FieldItemForBuff_TYPE adjustType, glm::vec3 & genPos)
{
	switch (buffType)
	{
	case ENUM_ITEMBUFF_SPEED:
		return new ItemSpeed(adjustType, _bindedGameSession, genPos);
		break;
	case ENUM_ITEMBUFF_HP:
		return new ItemSpeed(adjustType, _bindedGameSession, genPos);
		break;
	case ENUM_ITEMBUFF_INVISIBLE:
		return new ItemSpeed(adjustType, _bindedGameSession, genPos);
		break;
	case ENUM_ITEMBUFF_AMMO:
		return new ItemSpeed(adjustType, _bindedGameSession, genPos);
		break;
	case ENUM_ITEMBUFF_ARMOR:
		return new ItemSpeed(adjustType, _bindedGameSession, genPos);
		break;
	case ENUM_ITEMBUFF_OVERWHELIMNG:
		return new ItemSpeed(adjustType, _bindedGameSession, genPos);
		break;
	case ENUM_ITEMBUFF_DMGBUFF:
		return new ItemSpeed(adjustType, _bindedGameSession, genPos);
		break;
	case ENUM_ITEMBUFF_RANDTELEPORT:
		return new ItemSpeed(adjustType, _bindedGameSession, genPos);
		break;
	case ENUM_ITEMBUFF_SHOT_DELAY:
		return new ItemSpeed(adjustType, _bindedGameSession, genPos);
		break;
	case ENUM_ITEMBUFF_SHOT_SPEED:
		return new ItemSpeed(adjustType, _bindedGameSession, genPos);
		break;
	case ENUM_ITEMBUFF_OTHER_DISABLESHOT:
		return new ItemSpeed(adjustType, _bindedGameSession, genPos);
		break;
	case ENUM_ITEMBUFF_OTHER_RAND_DIRECTION:
		return new ItemSpeed(adjustType, _bindedGameSession, genPos);
		break;
	case ENUM_ITEMBUFF_OTHER_SPEEDSLOW:
		return new ItemSpeed(adjustType, _bindedGameSession, genPos);
		break;
	case ENUM_ITEMBUFF_TEAM_DMG:
		return new ItemSpeed(adjustType, _bindedGameSession, genPos);
		break;
	}

	return nullptr;
}

// itemType ; buff

IFieldItemForStuff * FieldItemGenerator::genItemForStuff(ENUM_ITEMSTUFF_TYPE buffType, ENUM_FieldItemForStuff_TYPE adjustType, glm::vec3 & genPos)
{
	return nullptr;
}

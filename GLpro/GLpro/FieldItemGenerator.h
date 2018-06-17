#pragma once
#include "stdafx.h"

#include "IFieldItemForBuff.h"
#include "IFieldItemForStuff.h"

// (Item for buff) Type
enum ENUM_ITEMBUFF_TYPE
{
	ENUM_ITEMBUFF_SPEED,
	ENUM_ITEMBUFF_HP,
	ENUM_ITEMBUFF_INVISIBLE,
	ENUM_ITEMBUFF_AMMO,
	ENUM_ITEMBUFF_ARMOR,
	ENUM_ITEMBUFF_OVERWHELIMNG,
	ENUM_ITEMBUFF_DMGBUFF,
	ENUM_ITEMBUFF_RANDTELEPORT,
	ENUM_ITEMBUFF_SHOT_DELAY,
	ENUM_ITEMBUFF_SHOT_SPEED,
	ENUM_ITEMBUFF_OTHER_DISABLESHOT,
	ENUM_ITEMBUFF_OTHER_RAND_DIRECTION,
	ENUM_ITEMBUFF_OTHER_SPEEDSLOW,
	ENUM_ITEMBUFF_TEAM_DMG,
	ENUM_ITEMBUFF_NUM
};

enum ENUM_ITEMSTUFF_TYPE
{
	ENUM_ITEMSTUFF_NUM
};

class GameSession;

class FieldItemGenerator
{
public:
	void initFieldItemGenerator(GameSession* gSession);

	// itemType ; buff
	IFieldItemForBuff* genItemForBuff(ENUM_ITEMBUFF_TYPE buffType, ENUM_FieldItemForBuff_TYPE adjustType, glm::vec3& genPos);

	// itemType : stuff
	IFieldItemForStuff* genItemForStuff(ENUM_ITEMSTUFF_TYPE buffType, ENUM_FieldItemForStuff_TYPE adjustType, glm::vec3& genPos);
public:
	GameSession* _bindedGameSession;
};
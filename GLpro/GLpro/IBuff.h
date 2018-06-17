#pragma once

enum ENUM_BUFF_TYPE
{
	ENUM_BUFF_SPEEDBUFF,
	ENUM_BUFF_HP,
	ENUM_BUFF_INVISIBLE,
	ENUM_BUFF_AMMO,
	ENUM_BUFF_ARMOR,
	ENUM_BUFF_OVERWHELIMNG,
	ENUM_BUFF_DMGBUFF,
	ENUM_BUFF_RANDTELEPORT,
	ENUM_BUFF_SHOT_DELAYBUFF,
	ENUM_BUFF_SHOT_SPEEDBUFF,
	ENUM_BUFF_OTHER_DISABLESHOT,
	ENUM_BUFF_OTHER_RAND_DIRECTION,
	ENUM_BUFF_OTHER_SPEEDSLOW,
	ENUM_BUFF_TEAM_DMGBUFF
};

class Player;
class Entity;
class BuffSum;

class IBuff
{
public:
	IBuff(ENUM_BUFF_TYPE buffType, float lifeTime);
	ENUM_BUFF_TYPE		getBuffType();
	bool				lifeEnd(float deltaTime, float acc);

	virtual IBuff*		getClone() = 0;		// get clone buff
	virtual void		adjustBuffSum(BuffSum* buffSum) = 0;
	virtual float		getPowerful() = 0;

public:
	ENUM_BUFF_TYPE		_buffType;
	Entity*				_whoGenBuff;		// if nullptr -> get from item in map
	float				_lifeTime;
};

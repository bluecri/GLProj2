#pragma once

enum ENUM_COLLISION_TYPE
{
	ENUM_COLLISION_PLAYER,
	ENUM_COLLISION_ENEMY,
	ENUM_COLLISION_PLAYER_MISSILE,
	ENUM_COLLISION_ENEMY_MISSILE
};

class CollisionInfo
{
public:
	CollisionInfo(ENUM_COLLISION_TYPE type);

	virtual ~CollisionInfo();

	ENUM_COLLISION_TYPE getType();

private:
	ENUM_COLLISION_TYPE _type;
};

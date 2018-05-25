#include "stdafx.h"
#include "CollisionInfo.h"

CollisionInfo::CollisionInfo(ENUM_COLLISION_TYPE type) : _type(type)
{
}

CollisionInfo::~CollisionInfo() {}

ENUM_COLLISION_TYPE CollisionInfo::getType() {
	return _type;
}

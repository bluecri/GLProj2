#include "AABBOb.h"

AABBOb::AABBOb()
{
	_center = glm::vec3();
	_halfAxisSize = glm::vec3();
	_bDirty = true;
}

void AABBOb::updateAABBOb(glm::vec3 & center, glm::vec3 & halfAxisSize)
{
	_bDirty = true;
	_center = center;
	_halfAxisSize = halfAxisSize;
}

void AABBOb::updateAABBObAxis(glm::vec3 & halfAxisSize)
{
	_bDirty = true;
	_halfAxisSize = halfAxisSize;
}

void AABBOb::updateAABBObAxis(float& halfAxisSize)
{
	_bDirty = true;
	_center[0] = halfAxisSize;
	_center[1] = halfAxisSize;
	_center[2] = halfAxisSize;
}

void AABBOb::updateAABBObCenter(glm::vec3 & center)
{
	_bDirty = true;
	_center = center;
}

void AABBOb::updateAABBObCenter(glm::vec4 & center)
{
	_bDirty = true;
	_center[0] = center[0];
	_center[1] = center[1];
	_center[2] = center[2];
}


const glm::vec3& AABBOb::getCenterConstRef() const
{
	return _center;
}

const glm::vec3 & AABBOb::getAxisConstRef() const
{
	return _halfAxisSize;
}

bool AABBOb::getDirty()
{
	return _bDirty;
}

void AABBOb::setDirty(bool bDirty)
{
	_bDirty = bDirty;
}

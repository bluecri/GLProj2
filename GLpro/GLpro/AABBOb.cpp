#include "AABBOb.h"

AABBOb::AABBOb()
{
	_center = glm::vec3();
	_halfAxisSize = glm::vec3();
}

void AABBOb::updateAABBOb(glm::vec3 & center, glm::vec3 & halfAxisSize)
{
	_center = center;
	_halfAxisSize = halfAxisSize;
}

void AABBOb::updateAABBObAxis(glm::vec3 & halfAxisSize)
{
	_halfAxisSize = halfAxisSize;
}

void AABBOb::updateAABBObAxis(float& halfAxisSize)
{
	_center[0] = halfAxisSize;
	_center[1] = halfAxisSize;
	_center[2] = halfAxisSize;
}

void AABBOb::updateAABBObCenter(glm::vec3 & center)
{
	_center = center;
}

void AABBOb::updateAABBObCenter(glm::vec4 & center)
{
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
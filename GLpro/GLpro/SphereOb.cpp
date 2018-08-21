#include "SphereOb.h"

SphereOb::SphereOb()
{
	_center = glm::vec3();
	_radius = 1.0f;
}

void SphereOb::updateSphereOb(glm::vec3 & center)
{
	_center = center;
}

void SphereOb::updateSphereOb(float & radius)
{
	_radius = radius;
}

const glm::vec3 & SphereOb::getCenterConstRef()
{
	return _center;
}

glm::vec3 SphereOb::getCenter()
{
	return _center;
}

float SphereOb::getRadius()
{
	return _radius;
}

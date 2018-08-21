#pragma once

#include "stdafx.h"

class SphereOb
{
public:
	SphereOb();
	void		updateSphereOb(glm::vec3& center);
	void		updateSphereOb(float& radius);
	
	const glm::vec3&	getCenterConstRef();
	glm::vec3	getCenter();
	float		getRadius();

private:
	glm::vec3	_center;
	float		_radius;
};
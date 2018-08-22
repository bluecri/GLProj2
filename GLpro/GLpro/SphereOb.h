#pragma once

#include "stdafx.h"

class SphereOb
{
public:
	SphereOb();
	void		updateSphereOb(glm::vec3& center);
	void		updateSphereOb(float& radius);
	
	const glm::vec3&	getCenterConstRef() const;
	glm::vec3	getCenter() const;
	float		getRadius() const;

private:
	glm::vec3	_center;
	float		_radius;
};
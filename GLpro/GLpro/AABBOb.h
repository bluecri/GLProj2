#pragma once

#include "stdafx.h"

class AABBOb
{
public:
	AABBOb();

	void updateAABBOb(glm::vec3& center, glm::vec3& halfAxisSize);
	void updateAABBObAxis(glm::vec3& halfAxisSize);
	void updateAABBObAxis(float& halfAxisSize);
	void updateAABBObCenter(glm::vec3& center);
	void updateAABBObCenter(glm::vec4& center);

	glm::vec3& getCenter();
	glm::vec3& getAxis();

private:
	glm::vec3 _center;
	glm::vec3 _halfAxisSize;
};
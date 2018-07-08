#pragma once

#include "stdafx.h"

class AABBOb
{
public:
	AABBOb();

	void	updateAABBOb(glm::vec3& center, glm::vec3& halfAxisSize);
	void	updateAABBObAxis(glm::vec3& halfAxisSize);
	void	updateAABBObAxis(float& halfAxisSize);
	void	updateAABBObCenter(glm::vec3& center);
	void	updateAABBObCenter(glm::vec4& center);

	const	glm::vec3& getCenterConstRef() const;
	const	glm::vec3& getAxisConstRef() const;

private:
	glm::vec3 _center;
	glm::vec3 _halfAxisSize;
};
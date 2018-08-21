#pragma once

#include "stdafx.h"

class AABBOb
{
public:
	AABBOb();

	void	updateAABBOb(glm::vec3& center, glm::vec3& halfAxisSize);
	void	updateAABBObAxis(glm::vec3& halfAxisSize);
	void	updateAABBObAxis(float& halfAxisSize);
	void	updateAABBObCenter(glm::mat4& center);
	void	updateAABBObCenter(glm::vec3& center);
	void	updateAABBObCenter(glm::vec4& center);
	bool	checkINFLineIntersection(glm::vec3& lineStartPos, glm::vec3& lineVec, float& resultDist);
	bool	checkLineIntersection(glm::vec3& lineStartPos, glm::vec3& lineVec, float len, float& resultDist);
	bool	checkPointInAABB(glm::vec3& pos);

	const	glm::vec3&	getCenterConstRef() const;
	glm::vec3			getCenter();
	const	glm::vec3&	getAxisConstRef() const;
	glm::vec3			getAxis();

private:
	glm::vec3	_worldVec;
	glm::vec3	_halfAxisSize;
};
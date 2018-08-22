#pragma once

#include "stdafx.h"

class AABBOb
{
public:
	AABBOb();

	void	updateAABBOb(const glm::vec3& center, const glm::vec3& halfAxisSize);
	void	updateAABBObAxis(const glm::vec3& halfAxisSize);
	void	updateAABBObAxis(float halfAxisSize);
	void	updateAABBObCenter(const glm::mat4& center);
	void	updateAABBObCenter(const glm::vec3& center);
	void	updateAABBObCenter(const glm::vec4& center);
	//bool	checkINFLineIntersection(glm::vec3& lineStartPos, glm::vec3& lineVec, float& resultDist);
	//bool	checkLineIntersection(glm::vec3& lineStartPos, glm::vec3& lineVec, float len, float& resultDist);
	bool	checkPointInAABB(glm::vec3& pos);

	const	glm::vec3&	getCenterConstRef() const;
	glm::vec3			getCenter() const;
	const	glm::vec3&	getAxisConstRef() const;
	glm::vec3			getAxis() const;

private:
	glm::vec3	_worldVec;
	glm::vec3	_halfAxisSize;
};
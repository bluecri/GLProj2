#pragma once

#include "stdafx.h"

class OBBOb
{
public:
	OBBOb();
	OBBOb(glm::mat4& worldMat, glm::vec3& halfAxisSize);
	OBBOb(glm::vec3& pos, glm::vec3& halfAxisSize);

	void	updateOBBOb(const glm::mat4& worldMat, const glm::vec3& halfAxisSize);
	void	updateOBBObAxis(const glm::vec3& halfAxisSize);
	void	updateOBBObMat(const glm::mat4& worldMat);
	void	updateOBBObMatOnlyRotate(const glm::mat4& rotMat);

	const	glm::mat4& getMatConstRef() const;
	const	glm::vec3& getAxisConstRef() const;

private:
	glm::mat4	_worldMat;		// center + rotate
	glm::vec3	_halfAxisSize;
};
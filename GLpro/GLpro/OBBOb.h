#pragma once

#include "stdafx.h"

class OBBOb
{
public:
	OBBOb();
	OBBOb(glm::mat4& worldMat, glm::vec3& halfAxisSize);
	OBBOb(glm::vec3& pos, glm::vec3& halfAxisSize);

	void	updateOBBOb(glm::mat4& worldMat, glm::vec3& halfAxisSize);
	void	updateOBBObAxis(glm::vec3& halfAxisSize);
	void	updateOBBObMat(glm::mat4& worldMat);
	void	updateOBBObMatOnlyRotate(glm::mat4& rotMat);

	const	glm::vec3& getMatConstRef();
	const	glm::vec3& getAxisConstRef();

private:
	glm::mat4	_worldMat;		// center + rotate
	glm::vec3	_halfAxisSize;
};
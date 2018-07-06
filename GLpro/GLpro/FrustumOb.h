#pragma once

#include "stdafx.h"

class FrustumOb
{
public:
	FrustumOb();
	void updateFrustumOb(glm::mat4& persMat, glm::mat4& viewMat);

	std::vector<glm::vec4>& getFrustum();

private:
	std::vector<glm::vec4>	_frustum;
};
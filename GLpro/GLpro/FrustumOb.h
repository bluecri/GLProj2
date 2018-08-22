#pragma once

#include "stdafx.h"

class FrustumOb
{
public:
	FrustumOb();
	void updateFrustumOb(glm::mat4& persMat, glm::mat4& viewMat);

	const std::vector<glm::vec4>& getFrustumConstRef() const;

private:
	std::vector<glm::vec4>	_frustum;
};
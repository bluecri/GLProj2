#pragma once

#include "stdafx.h"

class Light
{
public:
	Light(glm::vec4 color, bool bLitOn = true);

	glm::vec4		getColor();
	bool			isLightOn();
	void			setLightOn(bool b);

protected:
	bool			_bLightOn;
	glm::vec4		_color;
};
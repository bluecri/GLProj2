#include "light.h"

Light::Light(glm::vec4 color, bool bLitOn) : _color(color), _bLightOn(bLitOn) {}

glm::vec4 Light::getColor() {
	return _color;
}

bool Light::isLightOn()
{
	return _bLightOn;
}

void Light::setLightOn(bool b)
{
	_bLightOn = b;
}

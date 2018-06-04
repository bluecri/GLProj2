#pragma once
#include "stdafx.h"
#include "TextBox.h"

class AimTextUIObj : public TextBox
{
public:
	AimTextUIObj(int textSize);

	void setAimPositionWithQuat(const glm::quat& curQuat, const glm::quat& targetQuat);
	void setAimColor(glm::vec3& color);

public:
	static glm::vec3 _outAimColor;
	static glm::vec3 _inAimColor;

private:
	int _textSizeCompensation;
};
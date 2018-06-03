#pragma once
#include "stdafx.h"
#include "TextBox.h"

class AimTextUIObj : public TextBox
{
public:
	AimTextUIObj();

	void setAimPositionWithQuat(const glm::quat& curQuat, const glm::quat& targetQuat);
};
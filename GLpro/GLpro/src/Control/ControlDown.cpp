#include "./ControlDown.h"
#include "../../stdafx.h"

namespace CONTROL
{
	ControlDown::ControlDown(int glfw_key_num, bool bKey)
		: Control(bKey)
	{
		_glfwKeyNum = glfw_key_num;
	}

	bool ControlDown::checkKeyInput(GLFWwindow* pWindow)
	{
		if (_bKey)
		{
			if (glfwGetKey(pWindow, _glfwKeyNum) == GLFW_PRESS) {
				setBTrigger(true);
				return true;
			}
			return false;
		}
		
		if (glfwGetMouseButton(pWindow, _glfwKeyNum) == GLFW_PRESS) {
			setBTrigger(true);
			return true;
		}
		return false;
	}

	bool ControlDown::isKeyON()
	{
		return isBTriggerOn();
	}
}


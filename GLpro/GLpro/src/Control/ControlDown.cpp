#include "./ControlDown.h"
#include "../../stdafx.h"

namespace CONTROL
{
	ControlDown::ControlDown(int glfw_key_num, bool bKey)
		: Control(bKey)
	{
		_glfwKeyNum = glfw_key_num;
	}

	// do not use checkKeyInput return value
	bool ControlDown::checkKeyInput(GLFWwindow* pWindow)
	{
		if (_bKey)
		{
			if (glfwGetKey(pWindow, _glfwKeyNum) == GLFW_PRESS) {
				setBTrigger(true);
				return true;
			}
			setBTrigger(false);
			return false;
		}
		
		if (glfwGetMouseButton(pWindow, _glfwKeyNum) == GLFW_PRESS) {
			setBTrigger(true);
			return true;
		}
		setBTrigger(false);
		return false;
	}

	bool ControlDown::isKeyON(bool reset)
	{
		return isBTriggerOn();
	}
}


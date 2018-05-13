#include "./ControlDown.h"

namespace CONTROL
{
	ControlDown::ControlDown(int glfw_key_num)
	{
		_glfwKeyNum = glfw_key_num;
	}

	bool ControlDown::checkKeyInput(GLFWwindow* pWindow)
	{
		if (glfwGetKey(pWindow, _glfwKeyNum) == GLFW_PRESS) {
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


#include "./ControlDown.h"

namespace CONTROL
{
	ControlDown::ControlDown(int glfw_key_num)
	{
		_glfwKeyNum = glfw_key_num;
	}

	void ControlDown::checkKeyInput(GLFWwindow* pWindow)
	{
		if (glfwGetKey(pWindow, _glfwKeyNum) == GLFW_PRESS) {
			setBTrigger(true);
		}
	}

	bool ControlDown::isKeyON()
	{
		return isBTriggerOn();
	}
}


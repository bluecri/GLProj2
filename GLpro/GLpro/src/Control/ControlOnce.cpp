#include "ControlOnce.h"

namespace CONTROL 
{
	ControlOnce::ControlOnce(int glfw_key_num)
	{
		_glfwKeyNum = glfw_key_num;
		_bOnceEnd = false;
	}

	void ControlOnce::checkKeyInput(GLFWwindow * pWindow)
	{
		if (glfwGetKey(pWindow, _glfwKeyNum) == GLFW_PRESS) {
			setBTrigger(true);
		}
		else {
			_bOnceEnd = false;
			setBTrigger(false);
		}
	}

	bool ControlOnce::isKeyON()
	{
		if (isBTriggerOn() && !_bOnceEnd) {
			_bOnceEnd = true;
			return true;
		}
		else {
			return false;
		}
	}

}

#include "ControlOnce.h"

namespace CONTROL 
{
	ControlOnce::ControlOnce(int glfw_key_num, bool bKey)
		: Control(bKey)
	{
		_glfwKeyNum = glfw_key_num;
		_bOnceEnd = false;	
	}

	bool ControlOnce::checkKeyInput(GLFWwindow * pWindow)
	{
		if (_bKey)
		{
			if (glfwGetKey(pWindow, _glfwKeyNum) == GLFW_PRESS) {
				setBTrigger(true);
				return true;
			}
			else {
				_bOnceEnd = false;
				setBTrigger(false);
				return false;
			}
		}
		else
		{
			// mouse key
			if (glfwGetMouseButton(pWindow, _glfwKeyNum) == GLFW_PRESS) {
				setBTrigger(true);
				return true;
			}
			else {
				_bOnceEnd = false;
				setBTrigger(false);
				return false;
			}
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

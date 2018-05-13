#ifndef __CONTROLDOWN_H__
#define __CONTROLDOWN_H__

#include "./Control.h"

namespace CONTROL
{
	class ControlDown : public Control
	{
		public:
			ControlDown(int glfw_key_num);
			virtual ~ControlDown() {};

			virtual bool checkKeyInput(GLFWwindow* pWindow);
			virtual bool isKeyON();

		private:
			int _glfwKeyNum;
	};
}

#endif
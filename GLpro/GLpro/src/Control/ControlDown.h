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

			void checkKeyInput(GLFWwindow* pWindow);
			bool isKeyON();

		private:
			int _glfwKeyNum;
	};
}

#endif
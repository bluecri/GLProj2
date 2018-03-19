#ifndef __CONTROLONCE_H__
#define __CONTROLONCE_H__

#include "./Control.h"

namespace CONTROL
{
	class ControlOnce : public Control
	{
	public:
		ControlOnce(int glfw_key_num);
		virtual ~ControlOnce() {};

		void checkKeyInput(GLFWwindow* pWindow);
		bool isKeyON();

	private:
		int _glfwKeyNum;
		bool _bOnceEnd;
	};
}

#endif
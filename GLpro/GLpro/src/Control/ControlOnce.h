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

		virtual bool checkKeyInput(GLFWwindow* pWindow);
		virtual bool isKeyON();

	private:
		int _glfwKeyNum;
		bool _bOnceEnd;
	};
}

#endif
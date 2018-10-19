#ifndef __CONTROLONCE_H__
#define __CONTROLONCE_H__

#include "./Control.h"

namespace CONTROL
{
	// 키를 지속적으로 누르고 있는 경우에도 한번의 input으로 처리한다.
	// 다시 keyOn(true)을 받으려면 누름을 멈추고 다시 눌러야함.
	class ControlOnce : public Control
	{
	public:
		ControlOnce(int glfw_key_num, bool bKey);
		virtual ~ControlOnce() {};

		virtual bool checkKeyInput(GLFWwindow* pWindow);
		virtual bool isKeyON(bool reset = true);

	private:
		bool _bOnceEnd;
	};
}

#endif
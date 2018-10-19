#ifndef __CONTROLONCE_H__
#define __CONTROLONCE_H__

#include "./Control.h"

namespace CONTROL
{
	// Ű�� ���������� ������ �ִ� ��쿡�� �ѹ��� input���� ó���Ѵ�.
	// �ٽ� keyOn(true)�� �������� ������ ���߰� �ٽ� ��������.
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
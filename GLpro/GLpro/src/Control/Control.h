#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <vector>

namespace CONTROL
{
	

	class Control 
	{
		public:
			Control();
			virtual ~Control() {};

			virtual bool checkKeyInput(GLFWwindow* pWindow) = 0;
			virtual bool isKeyON() = 0;

		protected:
			bool isBTriggerOn();
			void doOn();
			void doOff();
			void setBTrigger(bool b);

			int _glfwKeyNum;

		private:
			bool _bTriggered;
	};
}
#endif
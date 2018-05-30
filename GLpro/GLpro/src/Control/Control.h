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
			Control(bool bKey);
			virtual ~Control() {};

			virtual bool checkKeyInput(GLFWwindow* pWindow) = 0;
			virtual bool isKeyON() = 0;

		protected:
			bool isBTriggerOn();
			bool isBKey();
			void doOn();
			void doOff();
			void setBTrigger(bool b);

			int _glfwKeyNum;
			bool _bKey;

		private:
			bool _bTriggered;
	};
}
#endif
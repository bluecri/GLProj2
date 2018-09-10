#ifndef __CONTROL_H__
#define __CONTROL_H__

typedef struct GLFWwindow GLFWwindow;

namespace CONTROL
{
	class Control 
	{
		public:
			Control(bool bKey);
			virtual ~Control() {};

			virtual bool checkKeyInput(GLFWwindow* pWindow) = 0;
			virtual bool isKeyON(bool reset = true) = 0;

		protected:
			bool isBTriggerOn();
			bool isBKey();
			void doOn();
			void doOff();
			void setBTrigger(bool b);

			int _glfwKeyNum;
			bool _bKey;		// is not mouse key input

		private:
			bool _bTriggered;
	};
}
#endif
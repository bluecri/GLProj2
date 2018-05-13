#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <vector>

namespace CONTROL
{
	enum ENUM_BEHAVIOR {
		MOVE_LEFT = 0,
		MOVE_RIGHT,
		MOVE_UP,
		MOVE_DOWN,
		CLICK_L_ONCE,
		CLICK_R_ONCE,
		CLICK_L_DOWN,
		CLICK_R_DOWN,
		ENUM_BEHAVIOR_SIZE
	};

	static long long int s_bitKeyState = 0;	//global key state
	static bool controlCheck(ENUM_BEHAVIOR en);	//use this to check key

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


		private:
			bool _bTriggered;
	};
}
#endif
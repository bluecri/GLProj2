#ifndef __INPUTMANAGER_H__
#define __INPUTMANAGER_H__

#include "../../stdafx.h"

#include "./Control.h"
#include "./ControlDown.h"
#include "./ControlOnce.h"

#include <algorithm>

enum ENUM_INPUT_MODE
{
	ENUM_INPUT_CHAT,
	ENUM_INPUT_GAME
};

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


namespace CONTROL 
{
	class InputManager
	{
		public:
			InputManager();
			virtual ~InputManager() {};

			void keyUpdate();		// Do Key State Update
			void changeInputMode(ENUM_INPUT_MODE mode);

			static bool controlCheckGlobal(ENUM_BEHAVIOR en);	// check key input global
			static bool controlCheck(long long inKey, ENUM_BEHAVIOR en);

	public:
		std::string _inputString;

	private:
		void init();

	private:
			ENUM_INPUT_MODE _mode;
			std::vector<Control*> updateControlVec;	// Target to update controls
			static long long int _bitKeyState;	//global key state

	};

}

void chatModeCallBack(GLFWwindow* window, int inKey, int scancode, int action, int modifier);
void gameModeCallBack(GLFWwindow* window, int inKey, int scancode, int action, int modifier);

extern CONTROL::InputManager* GInputManager;
#endif
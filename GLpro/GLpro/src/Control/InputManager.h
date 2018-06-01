#ifndef __INPUTMANAGER_H__
#define __INPUTMANAGER_H__
#include <vector>

typedef struct GLFWwindow GLFWwindow;
namespace CONTROL {	class Control; }


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
	KEY_1_DOWN,
	KEY_2_DOWN,
	KEY_3_DOWN,
	KEY_4_DOWN,
	KEY_5_DOWN,
	KEY_6_DOWN,
	KEY_7_DOWN,
	KEY_8_DOWN,
	KEY_9_DOWN,
	KEY_0_DOWN,
	KEY_SPACE_DOWN,
	TAB_DOWN,
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
	
		float getMouseXPos();
		float getMouseYPos();

	private:
		void init();

	public:
		std::string _inputString;
		double _mouseXPos, _mouseYPos;

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
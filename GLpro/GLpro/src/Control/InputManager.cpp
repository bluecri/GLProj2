#include "./InputManager.h"

#include "../window.h"
#include "../../Scene.h"

long long int CONTROL::InputManager::_bitKeyState = 0;	//global key state
/*
*	inputmode == key : key(동시 input)값이 GScene으로 넘어감.
*	inputMode == string : string값이 GScene으로 넘어가며 string 초기화. (미처리시 제거됨)
*/
namespace CONTROL {
	InputManager::InputManager() 
	{
		_mode = ENUM_INPUT_GAME;
	}

	void InputManager::keyUpdate()
	{
		switch (_mode)
		{
		case ENUM_INPUT_MODE::ENUM_INPUT_CHAT :
			GScene->keyInput(_inputString);
			_inputString.clear();
			break;
		case ENUM_INPUT_MODE::ENUM_INPUT_GAME :
			int idx = 0;
			std::for_each(updateControlVec.begin(), updateControlVec.end(), [&](Control* elem) {
				if (elem->checkKeyInput(GWindow->_pWindow)) {
					_bitKeyState |= (1ULL << idx);
					++idx;
				}
				else {
					_bitKeyState &= ~(1ULL << idx);
					++idx;
				}
			});

			GScene->keyInput(_bitKeyState);
			break;
		}
		
	}

	// Do Key State Update

	void InputManager::changeInputMode(ENUM_INPUT_MODE mode)
	{
		_mode = mode;
		if (_mode == ENUM_INPUT_MODE::ENUM_INPUT_CHAT)
		{
			// this == GInputManager
			glfwSetKeyCallback(GWindow->_pWindow, chatModeCallBack);
		}
		else if (_mode == ENUM_INPUT_MODE::ENUM_INPUT_GAME)
		{
			glfwSetKeyCallback(GWindow->_pWindow, gameModeCallBack);
		}
	}

	bool InputManager::controlCheck(ENUM_BEHAVIOR en) {
		if (_bitKeyState & (1ULL << en)) {
			return true;
		}
		return false;
	}

	void InputManager::init()
	{
		// Init key or mouse down check
		updateControlVec[ENUM_BEHAVIOR::MOVE_LEFT] = new ControlDown(GLFW_KEY_A);
		updateControlVec[ENUM_BEHAVIOR::MOVE_RIGHT] = new ControlDown(GLFW_KEY_D);
		updateControlVec[ENUM_BEHAVIOR::MOVE_UP] = new ControlDown(GLFW_KEY_W);
		updateControlVec[ENUM_BEHAVIOR::MOVE_DOWN] = new ControlDown(GLFW_KEY_S);
		updateControlVec[ENUM_BEHAVIOR::CLICK_L_DOWN] = new ControlDown(GLFW_MOUSE_BUTTON_LEFT);
		updateControlVec[ENUM_BEHAVIOR::CLICK_R_DOWN] = new ControlDown(GLFW_MOUSE_BUTTON_RIGHT);

		// Init key or mouse down ONCE check
		updateControlVec[ENUM_BEHAVIOR::CLICK_L_ONCE] = new ControlOnce(GLFW_MOUSE_BUTTON_LEFT);
		updateControlVec[ENUM_BEHAVIOR::CLICK_R_ONCE] = new ControlOnce(GLFW_MOUSE_BUTTON_RIGHT);
		
	}
}

CONTROL::InputManager* GInputManager = nullptr;

void chatModeCallBack(GLFWwindow * window, int inKey, int scancode, int action, int modifier)
{
	// input string
	if (inKey == GLFW_KEY_BACKSPACE && GInputManager->_inputString.length() != 0)
	{
		GInputManager->_inputString.pop_back();
		return;
	}
	GInputManager->_inputString.push_back(static_cast<char>(inKey));
}

void gameModeCallBack(GLFWwindow * window, int inKey, int scancode, int action, int modifier)
{
	// none
}

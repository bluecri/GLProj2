#include "./InputManager.h"
#include "../../stdafx.h"

#include "../window.h"
#include "../../Scene.h"

#include "./Control.h"
#include "./ControlDown.h"
#include "./ControlOnce.h"

#include "../../Option.h"

long long int CONTROL::InputManager::_bitKeyState = 0;	//global key state
/*
*	inputmode == key : key(동시 input)값이 GScene으로 넘어감.
*	inputMode == string : string값이 GScene으로 넘어가며 string 초기화. (미처리시 제거됨)
*/
namespace CONTROL {
	InputManager::InputManager() 
	{
		_mode = ENUM_INPUT_GAME;
		init();
	}

	void InputManager::keyUpdate()
	{
		// mouse point update
		glfwGetCursorPos(GWindow->_pWindow, &_mouseXPos, &_mouseYPos);

		switch (_mode)
		{
		case ENUM_INPUT_MODE::ENUM_INPUT_CHAT :
			GScene->keyInput(_inputString);
			_inputString.clear();
			break;

		case ENUM_INPUT_MODE::ENUM_INPUT_GAME :
			int idx = 0;
			std::for_each(updateControlVec.begin(), updateControlVec.end(), [&](Control* elem) {
				elem->checkKeyInput(GWindow->_pWindow);
				if (elem->isKeyON()) {
					_bitKeyState |= (1ULL << idx);
					++idx;
				}
				else {
					_bitKeyState &= ~(1ULL << idx);
					++idx;
				}
			});

			GScene->keyInput(_bitKeyState);

			// for debug text print
			GOption->_debugInfoPrintProgress(_bitKeyState);

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

	bool InputManager::controlCheckGlobal(ENUM_BEHAVIOR en) {
		if (_bitKeyState & (1ULL << en)) {
			return true;
		}
		return false;
	}

	bool InputManager::controlCheck(long long inKey, ENUM_BEHAVIOR en)
	{
		if (_bitKeyState & (1ULL << en)) {
			return true;
		}
		return false;
	}

	float InputManager::getMouseXPos() {
		return static_cast<float>(_mouseXPos);
	}

	float InputManager::getMouseYPos() {
		return static_cast<float>(_mouseYPos);
	}

	void InputManager::init()
	{
		// Init key or mouse down check
		for (int i = 0; i < ENUM_BEHAVIOR::ENUM_BEHAVIOR_SIZE; i++)
		{
			updateControlVec.push_back(nullptr);
		}
		updateControlVec[ENUM_BEHAVIOR::MOVE_LEFT] = new ControlDown(GLFW_KEY_A, true);
		updateControlVec[ENUM_BEHAVIOR::MOVE_RIGHT] = new ControlDown(GLFW_KEY_D, true);
		updateControlVec[ENUM_BEHAVIOR::MOVE_UP] = new ControlDown(GLFW_KEY_W, true);
		updateControlVec[ENUM_BEHAVIOR::MOVE_DOWN] = new ControlDown(GLFW_KEY_S, true);
		updateControlVec[ENUM_BEHAVIOR::CLICK_L_DOWN] = new ControlDown(GLFW_MOUSE_BUTTON_LEFT, false);
		updateControlVec[ENUM_BEHAVIOR::CLICK_R_DOWN] = new ControlDown(GLFW_MOUSE_BUTTON_RIGHT, false);

		updateControlVec[ENUM_BEHAVIOR::KEY_1_DOWN] = new ControlDown(GLFW_KEY_1, true);
		updateControlVec[ENUM_BEHAVIOR::KEY_2_DOWN] = new ControlDown(GLFW_KEY_2, true);
		updateControlVec[ENUM_BEHAVIOR::KEY_3_DOWN] = new ControlDown(GLFW_KEY_3, true);
		updateControlVec[ENUM_BEHAVIOR::KEY_4_DOWN] = new ControlDown(GLFW_KEY_4, true);
		updateControlVec[ENUM_BEHAVIOR::KEY_5_DOWN] = new ControlDown(GLFW_KEY_5, true);
		updateControlVec[ENUM_BEHAVIOR::KEY_6_DOWN] = new ControlDown(GLFW_KEY_6, true);
		updateControlVec[ENUM_BEHAVIOR::KEY_7_DOWN] = new ControlDown(GLFW_KEY_7, true);
		updateControlVec[ENUM_BEHAVIOR::KEY_8_DOWN] = new ControlDown(GLFW_KEY_8, true);
		updateControlVec[ENUM_BEHAVIOR::KEY_9_DOWN] = new ControlDown(GLFW_KEY_9, true);
		updateControlVec[ENUM_BEHAVIOR::KEY_0_DOWN] = new ControlDown(GLFW_KEY_0, true);
		updateControlVec[ENUM_BEHAVIOR::KEY_SPACE_DOWN] = new ControlDown(GLFW_KEY_SPACE, true);
		updateControlVec[ENUM_BEHAVIOR::TAB_DOWN] = new ControlDown(GLFW_KEY_TAB, true);

		updateControlVec[ENUM_BEHAVIOR::KEY_F1_ONCE] = new ControlOnce(GLFW_KEY_F1, true);
		updateControlVec[ENUM_BEHAVIOR::KEY_F2_ONCE] = new ControlOnce(GLFW_KEY_F2, true);
		updateControlVec[ENUM_BEHAVIOR::KEY_F3_ONCE] = new ControlOnce(GLFW_KEY_F3, true);
		updateControlVec[ENUM_BEHAVIOR::KEY_F4_ONCE] = new ControlOnce(GLFW_KEY_F4, true);
		updateControlVec[ENUM_BEHAVIOR::KEY_F5_ONCE] = new ControlOnce(GLFW_KEY_F5, true);
		updateControlVec[ENUM_BEHAVIOR::KEY_F6_ONCE] = new ControlOnce(GLFW_KEY_F6, true);
		updateControlVec[ENUM_BEHAVIOR::KEY_F7_ONCE] = new ControlOnce(GLFW_KEY_F7, true);
		updateControlVec[ENUM_BEHAVIOR::KEY_F8_ONCE] = new ControlOnce(GLFW_KEY_F8, true);
		updateControlVec[ENUM_BEHAVIOR::KEY_F9_ONCE] = new ControlOnce(GLFW_KEY_F9, true);
		updateControlVec[ENUM_BEHAVIOR::KEY_F10_ONCE] = new ControlOnce(GLFW_KEY_F10, true);
		updateControlVec[ENUM_BEHAVIOR::KEY_F11_ONCE] = new ControlOnce(GLFW_KEY_F11, true);
		updateControlVec[ENUM_BEHAVIOR::KEY_F12_ONCE] = new ControlOnce(GLFW_KEY_F12, true);

		updateControlVec[ENUM_BEHAVIOR::KEY_MIDDLE_PLUS_ONCE] = new ControlOnce(GLFW_KEY_EQUAL, true);
		updateControlVec[ENUM_BEHAVIOR::KEY_MIDDLE_MINUS_ONCE] = new ControlOnce(GLFW_KEY_MINUS, true);


		// Init key or mouse down ONCE check
		updateControlVec[ENUM_BEHAVIOR::CLICK_L_ONCE] = new ControlOnce(GLFW_MOUSE_BUTTON_LEFT, false);
		updateControlVec[ENUM_BEHAVIOR::CLICK_R_ONCE] = new ControlOnce(GLFW_MOUSE_BUTTON_RIGHT, false);
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

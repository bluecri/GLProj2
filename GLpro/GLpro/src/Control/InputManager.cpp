#include "./InputManager.h"

namespace CONTROL {
	void InputManager::keyUpdate(GLFWwindow* window)
	{
		int idx = 0;
		std::for_each(updateControlVec.begin(), updateControlVec.end(), [&](Control* elem) {
			if (elem->checkKeyInput(window)) {
				s_bitKeyState |= (1 << idx);
				++idx;
			}
			else {
				s_bitKeyState &= ~(1 << idx);
				++idx;
			}
		});
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

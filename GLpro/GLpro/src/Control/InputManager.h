#ifndef __INPUTMANAGER_H__
#define __INPUTMANAGER_H__

#include "./Control.h"
#include "./ControlDown.h"
#include "./ControlOnce.h"

#include <algorithm>

class FObj;

namespace CONTROL 
{
	class InputManager
	{
		public:
			InputManager() {};
			virtual ~InputManager() {};

			void keyUpdate(GLFWwindow* window);		// Do Key State Update
		private:
			std::vector<Control*> updateControlVec = std::vector<Control*>(ENUM_BEHAVIOR_SIZE, nullptr);	// Target to update controls
			void init();
	};

}
#endif
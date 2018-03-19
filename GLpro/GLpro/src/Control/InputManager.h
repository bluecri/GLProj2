#ifndef __INPUTMANAGER_H__
#define __INPUTMANAGER_H__

#include "./Control.h"

namespace CONTROL 
{
	class InputManager
	{
		public:
			InputManager() {};
			virtual ~InputManager() {};
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
		private:
			std::vector<Control*> updateControlVec = std::vector<Control*>(ENUM_BEHAVIOR_SIZE, nullptr);	// Target to update controls
			void init();
	};

}
#endif
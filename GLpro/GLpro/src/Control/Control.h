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
			Control();
			virtual ~Control() {};

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
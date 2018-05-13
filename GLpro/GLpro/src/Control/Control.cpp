#include "Control.h"

namespace CONTROL {

	Control::Control()
	{
		_bTriggered = false;
	}

	static bool controlCheck(ENUM_BEHAVIOR en) {
		if (s_bitKeyState & (1 << en)) {
			return true;
		}
		return false;
	}

	bool Control::isBTriggerOn()
	{
		return _bTriggered;
	}

	void Control::doOn()
	{
		_bTriggered = true;
	}

	void Control::doOff()
	{
		_bTriggered = false;
	}

	void Control::setBTrigger(bool b)
	{
		_bTriggered = b;
	}
}


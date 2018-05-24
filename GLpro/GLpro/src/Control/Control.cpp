#include "Control.h"

namespace CONTROL {

	Control::Control()
	{
		_bTriggered = false;
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


#include "Control.h"

namespace CONTROL {

	Control::Control(bool bKey)
	{
		_bTriggered = false;
		_bKey = bKey;
	}

	bool Control::isBTriggerOn()
	{
		return _bTriggered;
	}

	bool Control::isBKey() {
		return _bKey;
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


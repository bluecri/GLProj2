#include "Control.h"

CONTROL::Control::Control()
{
	_bTriggered = false;
}

bool CONTROL::Control::isBTriggerOn()
{
	return _bTriggered;
}

void CONTROL::Control::doOn()
{
	_bTriggered = true;
}

void CONTROL::Control::doOff()
{
	_bTriggered = false;
}

void CONTROL::Control::setBTrigger(bool b)
{
	_bTriggered = b;
}

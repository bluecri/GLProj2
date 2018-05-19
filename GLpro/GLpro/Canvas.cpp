#include "stdafx.h"
#include "Canvas.h"
#include "Box.h"

// set focusBox that was clicked.

inline void Canvas::transferKeyInputToFocusBox(int inputKey)
{
	if (_focusBox == nullptr)
		return;

	_focusBox->eventKeyInput(inputKey);

}

inline void Canvas::transferMouseClickToBox(int x, int y)
{
	for (auto elem : _boxMap)
	{
		if (elem.second->IsInBox(x, y))
		{
			_focusBox = elem.second->getBoxWithCoord(x, y);
			_focusBox->eventMouseClick();
			return;
		}
	}
}

void Canvas::addBox(Box * inbox)
{
	_boxMap.insert(std::make_pair(inbox->_name, inbox));
}

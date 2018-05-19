#include "stdafx.h"
#include "Canvas.h"
#include "Box.h"

inline void Canvas::setFocusBox(int x, int y)
{
	for (auto elem : _boxList)
	{
		if (elem->IsInBox(x, y))
		{
			_focusBox = elem;
			return;
		}
	}
	return;
}

inline void Canvas::transferKeyInputToFocusBox(int inputKey)
{
	if (_focusBox == nullptr)
		return;

	_focusBox->eventKeyInput(inputKey);

}

void Canvas::addBox(Box * box)
{
	_boxList.push_back(box);
}

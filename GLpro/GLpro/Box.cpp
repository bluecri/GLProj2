#include "stdafx.h"
#include "Box.h"

std::vector<Box*> Box::preMadeBoxesVec;


Box::Box(int width, int height, int widthLT, int heightLT)
	: UIEntity(), _width(width), _height(height), _widthLT(widthLT), _heightLT(heightLT), _keyFocusOn(false)
{
	_evMouseOnCallBack = nullptr;
	_evMouseClickCallBack = nullptr;
	_evMouseOutCallBack = nullptr;
	_evKeyInputCallBack = nullptr;
}

Box::Box(const Box & copy)
	: UIEntity(), _width(copy._width), _height(copy._height), _widthLT(copy._widthLT), _heightLT(copy._heightLT), _keyFocusOn(false)
{
}

void Box::eventMouseOn()
{
	if (_evMouseOnCallBack)	_evMouseOnCallBack();
}

void Box::eventMouseClick()
{
	if (_evMouseClickCallBack)	_evMouseClickCallBack();
}

void Box::eventMouseOut()
{
	if (_evMouseOutCallBack)	_evMouseOutCallBack();
}

void Box::eventKeyInput(long long inputKey)
{
	if (_evKeyInputCallBack)	_evKeyInputCallBack(inputKey);
}

void Box::eventKeyInput(std::string & inputStr)
{
	if (_evKeyStrInputCallBack)	_evKeyStrInputCallBack(inputStr);
}

void Box::bindEvMouseOnCallback(std::function<void()>& evCallBack)
{
	_evMouseOnCallBack = evCallBack;
}

void Box::bindEvMouseClick(std::function<void()>& evCallBack)
{
	_evMouseClickCallBack = evCallBack;
}

void Box::bindEvMouseOut(std::function<void()>& evCallBack)
{
	_evMouseOutCallBack = evCallBack;
}

Box * Box::getBoxWithCoord(int x, int y)
{
	for (auto elem : _childUIEntityList)
	{
		Box* childBox = static_cast<Box*>(elem);
		if (childBox->IsInBox(x, y))
		{
			return childBox->getBoxWithCoord(x, y);
		}
	}

	return this;
}

// width, height update function not exit)

bool Box::IsInBox(int x, int y)
{
	if (_widthLT < x && x < _width + _widthLT
		&& _heightLT < y && y < _height + _heightLT)
	{
		return true;
	}
	return false;
}

void Box::SetKeyFocus(bool & b) { _keyFocusOn = b; }

bool Box::GetKeyFocus() { return _keyFocusOn; }

// only for empty box

void Box::modifyEmptySize(int width, int height)
{
	_width = width;
	_height = height;
}

void Box::moveLTPosition(int widthLT, int heightLT)
{
	_widthLT = widthLT;
	_heightLT = heightLT;
}

void Box::initPreMade()
{
	Box* tempBox = new Box(10, 10, 10, 10);
	tempBox->setBRender(false);
	preMadeBoxesVec.push_back(tempBox);
}

void Box::setBRender(bool bRender)
{
	// do nothing
}

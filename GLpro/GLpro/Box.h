#pragma once

#include <functional>
#include "UIEntity.h"

class Box : public UIEntity
{
public:
	Box(int width, int height, int widthLT, int heightLT)
		: UIEntity(), _width(width), _height(height), _widthLT(widthLT), _heightLT(heightLT), _keyFocusOn(false)
	{
		_evMouseOnCallBack = nullptr;
		_evMouseClickCallBack = nullptr;
		_evMouseOutCallBack = nullptr;
		_evKeyInputCallBack = nullptr;
	}

	Box(const Box& copy)
		: UIEntity(), _width(copy._width), _height(copy._height), _widthLT(copy._widthLT), _heightLT(copy._heightLT), _keyFocusOn(false)
	{
	}

	virtual void eventMouseOn()
	{
		if(_evMouseOnCallBack)	_evMouseOnCallBack();
	}
	virtual void eventMouseClick()
	{
		if (_evMouseClickCallBack)	_evMouseClickCallBack();
	}
	virtual void eventMouseOut()
	{
		if (_evMouseOutCallBack)	_evMouseOutCallBack();
	}
	virtual void eventKeyInput(int inputKey)
	{
		if (_evKeyInputCallBack)	_evKeyInputCallBack(inputKey);
	}

	virtual void bindEvMouseOnCallback(std::function<void()>& evCallBack)
	{
		_evMouseOnCallBack = evCallBack;
	}
	virtual void bindEvMouseClick(std::function<void()>& evCallBack)
	{
		_evMouseClickCallBack = evCallBack;
	}
	virtual void bindEvMouseOut(std::function<void()>& evCallBack)
	{
		_evMouseOutCallBack = evCallBack;
	}
	virtual void bindEvMouseOut(std::function<void()>& evCallBack)
	{
		_evKeyInputCallBack = evCallBack;
	}

	virtual Box* getBoxWithCoord(int x, int y)
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
	bool IsInBox(int x, int y)
	{
		if (_widthLT < x && x < _width + _widthLT
			&& _heightLT < y && y < _height + _heightLT)
		{
			return true;
		}
		return false;
	}

	void SetKeyFocus(bool& b) {	_keyFocusOn = b;}
	bool GetKeyFocus() { return _keyFocusOn; }

	// only for empty box
	virtual void modifyEmptySize(int width, int height)
	{
		_width = width;
		_height = height;
		_widthLT = widthLT;
		_heightLT = heightLT;
	}

	virtual void moveLTPosition(int widthLT, int heightLT)
	{
		_widthLT = widthLT;
		_heightLT = heightLT;
	}

public:
	static void initPreMade()
	{
	}

	enum ENUM_PREMADE_BOX
	{
		PREMADE_BOX_EMPTY
	};

	static std::vector<Box*> preMadeBoxesVec;

protected:
	// UIEntity을(를) 통해 상속됨
	virtual void setBRender(bool bRender) override;

	bool _keyFocusOn;
	// TextFObj에도 존재함.(data 중복) textFObj를 정의하는편이 좋은가? ==> empty box 때문에 일단 이렇게 구현해놓음
	int _width, _height;
	int _widthLT, _heightLT;

	std::function<void()> _evMouseOnCallBack;
	std::function<void()> _evMouseClickCallBack;
	std::function<void()> _evMouseOutCallBack;

	std::function<void(int)> _evKeyInputCallBack;
	
};
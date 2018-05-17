#pragma once

#include <functional>

class Box
{
public:
	Box(int width, int height, int widthLT, int heightLT)
		: _width(width), _height(height), _widthLT(widthLT), _heightLT(heightLT), _keyFocusOn(false)
	{

	}
	virtual void eventMouseOn()
	{
		_evMouseOnCallBack();
	}
	virtual void eventMouseClick()
	{
		_evMouseClickCallBack();
	}
	virtual void eventMouseOut()
	{
		_evMouseOutCallBack();
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

	void SetKeyFocus(bool& b) {	_keyFocusOn = b;}
	bool GetKeyFocus() { return _keyFocusOn; }
	bool IsInBox(int width, int height) 
	{
		if (_widthLT < width && width < _width + _widthLT
			&& _heightLT < _height && _height < _width + _heightLT)
		{
			return true;
		}
		return false;
	}
	virtual void render(float deltaTime) = 0;

private:
	bool _keyFocusOn;
	int _width, _height;
	int _widthLT, _heightLT;
	std::function<void()> _evMouseOnCallBack;
	std::function<void()> _evMouseClickCallBack;
	std::function<void()> _evMouseOutCallBack;
};
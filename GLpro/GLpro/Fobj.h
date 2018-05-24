#pragma once

class FObj
{
public:
	FObj(bool bRender = true) : _bRender(bRender)
	{
	}

	void setBRender(bool bRender)
	{
		_bRender = bRender;
	}

	bool isRender()
	{
		return _bRender;
	}

private:
	bool _bRender;
};
#include "stdafx.h"
#include "Fobj.h"

FObj::FObj(bool bRender) : _bRender(bRender)
{
	_bDeleted = false;
}

void FObj::setBRender(bool bRender)
{
	_bRender = bRender;
}

bool FObj::isRender()
{
	return _bRender;
}

void FObj::setBDeleted()
{
	_bDeleted = true;
}

bool FObj::isBDeleted()
{
	return _bDeleted;
}

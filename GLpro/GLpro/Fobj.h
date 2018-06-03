#pragma once

class FObj
{
public:
	FObj(bool bRender = true);
	void setBRender(bool bRender);
	bool isRender();

	void setBDeleted();
	bool isBDeleted();
private:
	bool _bRender;
	bool _bDeleted;
};
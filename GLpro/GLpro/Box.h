#pragma once

#include <functional>
#include "UIEntity.h"

enum ENUM_PREMADE_BOX
{
	PREMADE_BOX_EMPTY
};

class Box : public UIEntity
{
public:
	Box(int width, int height, int widthLT, int heightLT);

	Box(const Box& copy);

	virtual void eventMouseOn();
	virtual void eventMouseClick();
	virtual void eventMouseOut();
	virtual void eventKeyInput(long long inputKey);
	virtual void eventKeyInput(std::string& inputStr);

	virtual void bindEvMouseOnCallback(std::function<void()>& evCallBack);
	virtual void bindEvMouseClick(std::function<void()>& evCallBack);
	virtual void bindEvMouseOut(std::function<void()>& evCallBack);

	Box* getBoxWithCoord(int x, int y);

	// width, height update function not exit)
	bool IsInBox(int x, int y);

	void SetKeyFocus(bool& b);
	bool GetKeyFocus();

	// only for empty box
	virtual void modifyEmptySize(int width, int height);

	virtual void moveLTPosition(int widthLT, int heightLT);

public:
	static void initPreMade();
	static std::vector<Box*> preMadeBoxesVec;

protected:
	// UIEntity��(��) ���� ��ӵ�
	virtual void setBRender(bool bRender) override;

	bool _keyFocusOn;
	// TextFObj���� ������.(data �ߺ�) textFObj�� �����ϴ����� ������? ==> empty box ������ �ϴ� �̷��� �����س���
	int _width, _height;
	int _widthLT, _heightLT;

	std::function<void()> _evMouseOnCallBack;
	std::function<void()> _evMouseClickCallBack;
	std::function<void()> _evMouseOutCallBack;

	std::function<void(long long)> _evKeyInputCallBack;
	std::function<void(std::string&)> _evKeyStrInputCallBack;

};


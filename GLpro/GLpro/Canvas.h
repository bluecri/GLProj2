#pragma once

#include "stdafx.h"
class Box;

enum ENUM_PREMADE_CANVAS
{
	PREMADE_CANVAS_LOGIN,
	PREMADE_CANVAS_CHANNEL,
	PREMADE_CANVAS_LOBBY,
	PREMADE_CANVAS_ROOM,
	PREMADE_CANVAS_INGAMEMENU,
	PREMADE_CANVAS_INGAME
};

/* 같은 height의 box depth 미구현. 같은 box height(tree)의 box들은 겹치지 않는 것으로 한다. */
class Canvas
{
public:
	void transferKeyInputToFocusBox(int inputKey);
	// set focusBox that was clicked.
	void transferMouseClickToBox(int x, int y)
	{
		for (auto elem : _boxList)
		{
			if (elem->IsInBox(x, y))
			{
				_focusBox = elem->getBoxWithCoord(x, y);
				_focusBox->eventMouseClick();
				return;
			}
		}
	}
	void addBox(Box* box);
	void setBRender(bool bRender)
	{
		for (auto elem : _boxList)
		{
			elem->setAllBRender(bRender);
		}
	}

	static void initPreMade()
	{
		Canvas* loginCanvas = new Canvas();
		preMadeCanvasVec.push_back(loginCanvas);

		Canvas* channelCanvas = new Canvas();
		preMadeCanvasVec.push_back(channelCanvas);

		Canvas* lobbyCanvas = new Canvas();
		preMadeCanvasVec.push_back(lobbyCanvas);

		Canvas* roomCanvas = new Canvas();
		preMadeCanvasVec.push_back(roomCanvas);

		Canvas* ingameMenuCanvas = new Canvas();
		preMadeCanvasVec.push_back(ingameMenuCanvas);

		Canvas* gameCanvas = new Canvas();
		preMadeCanvasVec.push_back(gameCanvas);

		// need textbox, imagebox... clone.?
		loginCanvas->
	}

public:
	std::list<Box*> _boxList;
	Box * _focusBox = nullptr;

	static std::vector<Canvas*> preMadeCanvasVec;
	
};
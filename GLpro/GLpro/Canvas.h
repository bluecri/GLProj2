#pragma once

#include "stdafx.h"
class Box;
class TextBox;
class ImageBox;

#define LOBBY_CNT 10
#define ROOM_ROW_CNT 10
#define ROOM_COL_CNT 2
#define PLAYER_ROW_CNT 20
#define PLAYER_COL_CNT 8
#define MENU_ROW_CNT 6
#define MENU_COL_CNT 2

enum ENUM_PREMADE_CANVAS
{
	PREMADE_CANVAS_LOGIN,
	PREMADE_CANVAS_ACCOUNTCREATE,
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
	void transferKeyInputToFocusBox(long long inputKey);
	void transferKeyInputToFocusBox(std::string & inputStr);

	// set focusBox that was clicked.
	void transferMouseClickToBox(int x, int y);
	void addBox(Box* box);
	void setBRender(bool bRender);

	static void initPreMade();

public:
	std::map<std::string, Box*> _boxMap;
	Box * _focusBox = nullptr;

	static std::vector<Canvas*> preMadeCanvasVec;

};

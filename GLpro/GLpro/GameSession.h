#pragma once

#include "Box.h"
#include "Canvas.h"

class Player;
// _targetGameSession

/*

Canvas* ingameMenuCanvas = new Canvas();
preMadeCanvasVec.push_back(ingameMenuCanvas);

Canvas* gameCanvas = new Canvas();
preMadeCanvasVec.push_back(gameCanvas);
*/
class GameSession
{
public:
	GameSession()
	{
		_menuCanvas = Canvas::preMadeCanvasVec[PREMADE_CANVAS_INGAMEMENU];
	}

	void pauseGame()
	{
		_menuCanvas->setBRender(true);
		_bMenuOn = true;
	}
	void resumeGame()
	{
		_menuCanvas->setBRender(false);
		_bMenuOn = false;
	}

	void transferKeyInputToFocusBox(int inputKey)
	{
		if (_bMenuOn)
		{
			return;
		}

		// process
	}
	// set focusBox that was clicked.
	void transferMouseClickToBox(int x, int y)
	{
		if (_bMenuOn)
		{
			return;
		}

		// process
	}
	
	void updateGameLogic(float deltaTime)
	{
		// main game logic loop
	}

public:
	Canvas* _menuCanvas;
	Player* _player;
	bool _bMenuOn;
};
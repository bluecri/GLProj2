#pragma once
#include "stdafx.h"

class Player;
class GameSession;
class Canvas;
class Entity;
class SkyboxGObject;

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
	GameSession();

	void pauseGame();
	void resumeGame();

	void transferKeyInput(long long inputKey);
	void transferKeyInput(std::string& inputStr);
//	void transferMouseClickToBox(int x, int y);		// set focusBox that was clicked.

	void initGameSession();
	void setAllEntityRRender(bool bRender);
	void removeEntityProcess();
	void update(float deltaTime, float acc);
	float getGSessionStartTimeStamp()
	{
		return _gSessionStartTimeStamp;
	}

	static void preMade();

public:
	static std::vector<GameSession*> preMadeGameSession;
	// draw game->gameUI->menu
	Canvas* _menuCanvas;
	Canvas* _gameUICanvas;
	Player* _player;

	std::map<int, Entity*> _allEntityMap;
	SkyboxGObject* _spaceSkybox;

	bool _bMenuOn;
	bool _bMouseOn;

private:
	float	_gSessionStartTimeStamp;
};
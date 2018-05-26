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

	// register new entity in game session
	void registerEntityToGameSession(Entity* entity);

	static void preMade();

public:
	static std::vector<GameSession*> preMadeGameSession;
	Canvas* _menuCanvas;
	Player* _player;

	std::map<int, Entity*> _allEntityMap;
	SkyboxGObject* _spaceSkybox;

	bool _bMenuOn;
};
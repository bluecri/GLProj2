#pragma once

class GameSession;
class Canvas;

class Scene
{
public:
	void changeCanvas(Canvas* newTargetCanvas);

	void startGame();
	void endGame();

	void networkPacketProcess();

	void keyInput(long long inputKey);
	void keyInput(std::string& inputStr);

	void update(float deltaTime, float acc);


	// pre elems

	//ttest : to be private
public:		
	Canvas * _targetCanvas;		// user Canvas::preMadeCanvasVec
	GameSession * _targetGameSession;
};

extern Scene* GScene;
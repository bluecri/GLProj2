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

	void mouseInput(int x, int y);
	void keyInput(long long inputKey);
	void keyInput(std::string& inputStr);


	// pre elems
private:
	Canvas * _targetCanvas;		// user Canvas::preMadeCanvasVec
	GameSession * _targetGameSession;
};

extern Scene* GScene;
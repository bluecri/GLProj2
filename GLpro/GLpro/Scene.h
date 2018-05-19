#pragma once

//class Canvas;
#include"Canvas.h"
#include"GameSession.h"

class GameSession;

class Scene
{
public:
	void changeCanvas(Canvas* newTargetCanvas)
	{
		_targetCanvas->setBRender(false);
		newTargetCanvas->setBRender(true);
		_targetCanvas = newTargetCanvas;
	}

	void startGame()
	{
		changeCanvas(nullptr);

		//_targetGameSession start
	}
	
	void endGame()
	{
		changeCanvas(Canvas::preMadeCanvasVec[PREMADE_CANVAS_ROOM]);
	}

	void networkPacketProcess()
	{
		// targetCanvas에 따라 패킷 처리
		
		/*
		switch (packetType)
		{
			// ui packet
			if (_targetCanvas != nullptr)
			{
				packetType == _targetCanvas type 인 경우에만 전달
			}

			// game packet
			if (_targetGameSession != nullptr)
			{
				
			}
		}
		*/
	}

	void mouseInput(int x, int y)
	{
		if (_targetGameSession != nullptr)
		{
			_targetGameSession->transferMouseClickToBox(x, y);
			return;
		}
		_targetCanvas->transferMouseClickToBox(x, y);
	}
	void keyInput(int inputKey)
	{
		if (_targetGameSession != nullptr)
		{
			_targetGameSession->transferKeyInputToFocusBox(inputKey);
			return;
		}
		_targetCanvas->transferKeyInputToFocusBox(inputKey);
	}

// pre elems
private:
	Canvas * _targetCanvas;		// user Canvas::preMadeCanvasVec
	GameSession * _targetGameSession;
};

Scene* GScene = nullptr;
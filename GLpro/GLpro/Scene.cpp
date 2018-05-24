#include "stdafx.h"
#include "Scene.h"
#include "GameSession.h"
#include "Canvas.h"

inline void Scene::changeCanvas(Canvas * newTargetCanvas)
{
	_targetCanvas->setBRender(false);
	newTargetCanvas->setBRender(true);
	_targetCanvas = newTargetCanvas;
}

inline void Scene::startGame()
{
	changeCanvas(nullptr);

	//_targetGameSession start
}

inline void Scene::endGame()
{
	changeCanvas(Canvas::preMadeCanvasVec[PREMADE_CANVAS_ROOM]);
}

inline void Scene::networkPacketProcess()
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

inline void Scene::mouseInput(int x, int y)
{
	if (_targetGameSession != nullptr)
	{
		_targetGameSession->transferMouseClickToBox(x, y);
		return;
	}
	_targetCanvas->transferMouseClickToBox(x, y);
}

void Scene::keyInput(long long inputKey)
{
	if (_targetGameSession != nullptr)
	{
		_targetGameSession->transferKeyInputToFocusBox(inputKey);
		return;
	}
	_targetCanvas->transferKeyInputToFocusBox(inputKey);
}

void Scene::keyInput(std::string & inputStr)
{
	if (_targetGameSession != nullptr)
	{
		_targetGameSession->transferKeyInputToFocusBox(inputStr);
		return;
	}
	_targetCanvas->transferKeyInputToFocusBox(inputStr);
}

Scene* GScene = nullptr;
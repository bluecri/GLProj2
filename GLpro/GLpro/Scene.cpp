#include "stdafx.h"
#include "Scene.h"
#include "GameSession.h"
#include "Canvas.h"

void Scene::changeCanvas(Canvas * newTargetCanvas)
{
	_targetCanvas->setBRender(false);
	newTargetCanvas->setBRender(true);
	_targetCanvas = newTargetCanvas;
}

void Scene::startGame()
{
	changeCanvas(nullptr);

	//_targetGameSession start
}

void Scene::endGame()
{
	changeCanvas(Canvas::preMadeCanvasVec[PREMADE_CANVAS_ROOM]);
}

void Scene::networkPacketProcess()
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

void Scene::keyInput(long long inputKey)
{
	if (_targetGameSession != nullptr)
	{
		_targetGameSession->transferKeyInput(inputKey);
		return;
	}
	if (_targetCanvas == nullptr)
	{
		printf_s("_targetCanvas is nullptr : Scene::keyInput(long long inputKey) \n");
		return;
	}
	_targetCanvas->transferKeyInputToFocusBox(inputKey);
}

void Scene::keyInput(std::string & inputStr)
{
	if (_targetGameSession != nullptr)
	{
		_targetGameSession->transferKeyInput(inputStr);
		return;
	}
	if (_targetCanvas == nullptr)
	{
		printf_s("_targetCanvas is nullptr : Scene::keyInput(std::string & inputStr) \n");
		return;
	}
	_targetCanvas->transferKeyInputToFocusBox(inputStr);
}

void Scene::update(float deltaTime, float acc)
{
	if (_targetGameSession != nullptr)
	{
		_targetGameSession->update(deltaTime, acc);
		return;
	}
	if (_targetCanvas == nullptr)
	{
		printf_s("_targetCanvas is nullptr : Scene::keyInput(std::string & inputStr) \n");
		return;
	}
	_targetCanvas->update(deltaTime, acc);
}

Scene* GScene = nullptr;
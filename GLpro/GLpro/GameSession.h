#pragma once

#include "Box.h"
#include "Canvas.h"
#include "src/Entity.h"
#include "Player.h"
#include "src/Camera/CameraManager.h"
#include "src/Camera/Camera.h"
#include "src/Resource/Model.h"
#include "src/Resource/ModelManager.h"
#include "src/Resource/Texture.h"
#include "src/Resource/TextureManager.h"

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
	void initGameSession()
	{
		// load plane, other plane...

	}
	void updateGameLogic(float deltaTime)
	{
		// main game logic loop

	}

	// register new entity in game session
	void registerEntityToGameSession(Entity* entity)
	{
		entity->_gameSession = this;
		_entityMap.insert(std::make_pair(entity->getID(), entity));
	}

	static void preMade()
	{
		int TEST_ENEMY_NUM = 20;

		GameSession* premadeSession = new GameSession();
		// player, camera, skybox, ->_entityMap

		// plane resource
		RESOURCE::Model* planeModel = GModelManager->getModelWithFileName("obj/SpaceShip.obj", true);
		RESOURCE::Texture* planeTexture = GTextureManager->getTextureWithFileName("texture/uvmap.DDS", "dds");
		SHADER::ShaderMain* shaderMain = GShaderManager->m_addShader<SHADER::ShaderMain>(ENUM_SHADER_TYPE::SHADER_TYPE_MAIN, "data/Shader/ShadowMapping.vertexshader", "data/Shader/ShadowMapping.fragmentshader");

		Player* newPlayer = new Player(planeModel, planeTexture, shaderMain);
		CAMERA::Camera* mainCam = GCameraManager->GetMainCamera();
		//todo : camera pos, lookat modify
		newPlayer->attachChildEntity(mainCam);
		premadeSession->registerEntityToGameSession(newPlayer);		// not register cam
		for (int i = 0; i < TEST_ENEMY_NUM; i++)
		{
			Player* newEnemy = new Enemy(planeModel, planeTexture, shaderMain);
			premadeSession->registerEntityToGameSession(newEnemy);
		}

		skybox,
		preMadeGameSession.push_back(premadeSession);
	}

public:
	static std::vector<GameSession*> preMadeGameSession;
	Canvas* _menuCanvas;
	Player* _player;
	std::map<int, Entity*> _entityMap;
	bool _bMenuOn;
};
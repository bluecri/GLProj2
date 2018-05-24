#include "GameSession.h"

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
#include "src/Resource/TextureManager.h"
#include "src/Shader/ShaderManager.h"
#include "src/Shader/ShaderMain.h"

#include "src/RenderTarget/Skybox/SkyboxFObj.h"
#include "src/RenderTarget/Normal/NormalFObj.h"
#include "SkyboxGObject.h"

std::vector<GameSession*> GameSession::preMadeGameSession;


inline GameSession::GameSession()
{
	_menuCanvas = Canvas::preMadeCanvasVec[PREMADE_CANVAS_INGAMEMENU];
}

inline void GameSession::pauseGame()
{
	_menuCanvas->setBRender(true);
	_bMenuOn = true;
}

inline void GameSession::resumeGame()
{
	_menuCanvas->setBRender(false);
	_bMenuOn = false;
}

inline void GameSession::transferKeyInputToFocusBox(long long inputKey)
{
	if (_bMenuOn)
	{
		return;
	}

	// todo : transfer to game process
}

inline void GameSession::transferKeyInputToFocusBox(std::string & inputStr)
{
	if (_bMenuOn)
	{
		// todo : transfer to menu string
	}

	// todo : transfer to game process
}

// set focusBox that was clicked.

inline void GameSession::transferMouseClickToBox(int x, int y)
{
	if (_bMenuOn)
	{
		return;
	}

	// process
}

inline void GameSession::initGameSession()
{
	// load plane, other plane...

}

inline void GameSession::updateGameLogic(float deltaTime)
{
	// main game logic loop

}

// register new entity in game session

inline void GameSession::registerEntityToGameSession(Entity * entity)
{
	entity->_gameSession = this;
	_allEntityMap.insert(std::make_pair(entity->getID(), entity));
}

inline void GameSession::preMade()
{
	int TEST_ENEMY_NUM = 20;

	GameSession* premadeSession = new GameSession();
	// player, camera, skybox, ->_entityMap

	// plane resource
	RESOURCE::Model* planeModel = GModelManager->getModelWithFileName("data/Model/SpaceShip.obj", true);
	RESOURCE::Texture* planeTexture = GTextureManager->getTextureWithFileName("data/Texture/uvmap.DDS", "dds");
	SHADER::ShaderMain* shaderMain = GShaderManager->m_addShader<SHADER::ShaderMain>(ENUM_SHADER_TYPE::SHADER_TYPE_MAIN, "data/Shader/ShadowMapping.vertexshader", "data/Shader/ShadowMapping.fragmentshader");

	// player & camera attach
	Player * newPlayer = new Player(planeModel, planeTexture, shaderMain);
	CAMERA::Camera** mainCam = GCameraManager->GetMainCamera();
	newPlayer->_normalFObj->setBRender(false);
	newPlayer->attachChildEntity(*mainCam);
	//todo player collide compo ( sound 는 player 안에서 ), ...
	//todo : camera pos, lookat local modify

	premadeSession->registerEntityToGameSession(*mainCam);
	premadeSession->registerEntityToGameSession(newPlayer);
	premadeSession->_player = newPlayer;

	// enemy
	for (int i = 0; i < TEST_ENEMY_NUM; i++)
	{
		//Player* newEnemy = new Enemy(planeModel, planeTexture, shaderMain);
		//premadeSession->registerEntityToGameSession(newEnemy);
		//newEnemy->_normalFObj->setBRender(false);
	}

	// space skybox
	SHADER::ShaderSkybox* skyboxShader = GShaderManager->m_addShader<SHADER::ShaderSkybox>(ENUM_SHADER_TYPE::SHADER_TYPE_SKYBOX, "data/shader/SkyboxMapping.vertexshader", "data/shader/SkyboxMapping.fragmentshader");
	RENDER_TARGET::SKYBOX::SkyboxFObj* skyboxFObj = new RENDER_TARGET::SKYBOX::SkyboxFObj("data/Texture/sky_bot.dds", "data/Texture/sky_top.dds", "data/Texture/sky_back.dds", "data/Texture/sky_front.dds", "data/Texture/sky_right.dds", "data/Texture/sky_left.dds", 40.0f);
	skyboxFObj->setBRender(false);

	SkyboxGObject * spaceSkybox = new SkyboxGObject(skyboxShader, skyboxFObj);
	premadeSession->_player->attachChildEntity(spaceSkybox);
	premadeSession->_spaceSkybox = spaceSkybox;
	premadeSession->registerEntityToGameSession(spaceSkybox);

	preMadeGameSession.push_back(premadeSession);		// premade session
}

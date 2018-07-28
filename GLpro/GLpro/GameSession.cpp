#include "GameSession.h"

#include "stdafx.h"

#include "Box.h"
#include "Canvas.h"
#include "Player.h"
#include "Enemy.h"
#include "src/Camera/CameraManager.h"
#include "src/Camera/Camera.h"
#include "src/Resource/Model.h"
#include "src/Resource/ModelManager.h"
#include "src/Resource/Texture.h"
#include "src/Resource/TextureManager.h"
#include "src/Shader/ShaderManager.h"
#include "src/Shader/ShaderMain.h"
#include "src/Control/InputManager.h"

#include "src/RenderTarget/Skybox/SkyboxFObj.h"
#include "src/RenderTarget/Normal/NormalFObj.h"
#include "SkyboxGObject.h"
#include "Fobj.h"

#include "Canvas.h"
#include "RigidbodyComponent.h"
#include "src/window.h"

#include "NormalMissileGenerator.h"
#include "MissileGeneratorStorage.h"

#include "LightManager.h"
#include "SpotLight.h"
#include "LightGameObject.h"

std::vector<GameSession*> GameSession::preMadeGameSession;


GameSession::GameSession()
{
	_menuCanvas = Canvas::preMadeCanvasVec[PREMADE_CANVAS_INGAMEMENU];
	_gSessionStartTimeStamp = glfwGetTime();
	_bMouseOn = false;
}

void GameSession::pauseGame()
{
	// todo : stop all entities's component(collide, rigid update...)
	_menuCanvas->setBRender(true);
	_bMenuOn = true;
}

void GameSession::resumeGame()
{
	_menuCanvas->setBRender(false);
	_bMenuOn = false;
}

void GameSession::transferKeyInput(long long inputKey)
{
	if (_bMenuOn)
	{
		_menuCanvas->transferKeyInputToFocusBox(inputKey);
		return;
	}

	// transfer key to player
	if(_player != nullptr)
		_player->inputProgress(inputKey);
	
}

void GameSession::transferKeyInput(std::string & inputStr)
{
	if (_bMenuOn)
	{
		// transfer to menu string
		_menuCanvas->transferKeyInputToFocusBox(inputStr);
		return;
	}

	// todo : string input 처리 ( not in player )
}

void GameSession::initGameSession()
{
	// load plane, other plane...

}

void GameSession::setAllEntityRRender(bool bRender) {
	for (auto elem : _allEntityMap)
	{
		elem.second->setBRender(bRender);
	}
}

// 지워야 할 entity 제거	doopt : bool check delete -> delete pointer list
void GameSession::removeEntityProcess() {
	for (auto it = _allEntityMap.begin(); it != _allEntityMap.end(); )
	{
		if ((*it).second->isBeDeleted())
		{
			// delete all child of entity too

			delete (*it).second;
			it = _allEntityMap.erase(it);

			continue;
		}
		++it;
	}
}

void GameSession::update(float deltaTime, float acc)
{
	// main game logic loop

	if (!_bMenuOn)
	{
		// entities game loop
		for (auto elem : _allEntityMap)
		{
			elem.second->logicUpdate(deltaTime, acc);
		}

		//mouse pos to center
		if (!_bMouseOn)
		{
			GWindow->mouseToCenter();
		}

		// skybox update
		_spaceSkybox->logicUpdate(deltaTime, acc);

		removeEntityProcess();
		return;
	}
}

float GameSession::getGSessionStartTimeStamp()
{
	return _gSessionStartTimeStamp;
}

void GameSession::preMade()
{
	int TEST_ENEMY_NUM = 20;

	GameSession* premadeSession = new GameSession();
	// player, camera, skybox, ->_entityMap

	// plane resource
	RESOURCE::Model* planeModel = GModelManager->getModelWithFileName("data/Model/SpaceShip.obj", true);
	RESOURCE::Texture* planeTexture = GTextureManager->getTextureWithFileName("data/Texture/uvmap.DDS", "dds");
	SHADER::ShaderMain* shaderMain = GShaderManager->m_addShader<SHADER::ShaderMain>(ENUM_SHADER_TYPE::SHADER_TYPE_MAIN, "data/Shader/ShadowMapping.vertexshader", "data/Shader/ShadowMapping.fragmentshader");

	// player & camera attach
	Player * newPlayer = new Player(premadeSession, planeModel, planeTexture, shaderMain);
	//newPlayer->_rigidbodyComponent->accRotationMatrix(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	newPlayer->getRigidbodyComponent()->setScaleMatrix(glm::vec3(1.0f, 1.0f, 1.0f));
	newPlayer->getRigidbodyComponent()->setDirty();
	newPlayer->setBRender(true);
	newPlayer->initIPlane(new PlaneInfo(100, 0, 3.0f), new PlaneInfo(100, 100, 3.0f));
	newPlayer->initPlayer();
	NormalMissileGenerator * normalMissileGenerator = new NormalMissileGenerator();
	normalMissileGenerator->initNormalMissileGenerator(newPlayer, newPlayer->getMissileGeneratorStorage());
	newPlayer->addMissileGenerator(normalMissileGenerator);

	//spot light on player
	GLightManager->AddDirectinalLight(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	LightGameObject* tempSpotGameObj = GLightManager->AddSpotLightManager(glm::mat4(), glm::vec3(0.0f, 0.0f, 5.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 40.0f, 1.0f, 0.01f, 20.0f)->getLightGameObject();
	newPlayer->attachChildEntity(static_cast<Entity*>(tempSpotGameObj));

	for (int i = 0; i < 1; i++)
	{
		LightGameObject* tempSpotGameObj2 = GLightManager->AddPointLightManager(glm::vec3(0.0f, 0.0f, 5.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f)->getLightGameObject();
		//tempSpotGameObj = GLightManager->AddDeferredPointLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f)->getLightGameObject();
		newPlayer->attachChildEntity(static_cast<Entity*>(tempSpotGameObj2));
	}


	
	//tempSpotGameObj = GLightManager->AddSpotLightManager(glm::mat4(), glm::vec3(0.0f, 4.0f, 4.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f)->getLightGameObject();
	//newPlayer->attachChildEntity(static_cast<Entity*>(tempSpotGameObj));
	//tempSpotGameObj = GLightManager->AddSpotLightManager(glm::mat4(), glm::vec3(0.0f, 4.0f, 4.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f)->getLightGameObject();
	//newPlayer->attachChildEntity(static_cast<Entity*>(tempSpotGameObj));
	//tempSpotGameObj = GLightManager->AddSpotLightManager(glm::mat4(), glm::vec3(0.0f, 4.0f, 4.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f)->getLightGameObject();
	//newPlayer->attachChildEntity(static_cast<Entity*>(tempSpotGameObj));
	

	std::vector<glm::vec3> enemyVec;
	for (int i = -3; i < 3; i++)
	{
		for (int k = -3; k < 3; k++)
		{
			for (int q = -3; q < 3; q++)
			{
				enemyVec.push_back(glm::vec3(i * 10.0f, k * 5.0f, q * 10.0f) );
			}
		}
	}
	Enemy * newEnemy = nullptr;
	for (auto elem : enemyVec)
	{
		// player & camera attach
		newEnemy = new Enemy(premadeSession, planeModel, planeTexture, shaderMain);
		//newPlayer->_rigidbodyComponent->accRotationMatrix(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		newEnemy->getRigidbodyComponent()->setDirty();
		newEnemy->getRigidbodyComponent()->translateModelMatrix(elem);
		newEnemy->setBRender(true);
		newEnemy->initIPlane(new PlaneInfo(100, 0, 3.0f), new PlaneInfo(100, 100, 3.0f));
		newEnemy->initEnemy();
		normalMissileGenerator = new NormalMissileGenerator();
		normalMissileGenerator->initNormalMissileGenerator(newEnemy, newEnemy->getMissileGeneratorStorage());
		newEnemy->addMissileGenerator(normalMissileGenerator);
		
	}


	
	CAMERA::Camera** mainCam = GCameraManager->GetMainCamera();
	//(*mainCam)->_rigidbodyComponent->accRotationMatrix(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	(*mainCam)->getRigidbodyComponent()->accModelMatrix(glm::vec3(0.0f, 0.0f, -14.0f));
	(*mainCam)->getRigidbodyComponent()->setDirty();
	//newPlayer->attachChildEntity(*mainCam);
	
	//ttest
	//Player * testP = new Player(premadeSession, planeModel, planeTexture, shaderMain);
	//testP->_rigidbodyComponent->accModelMatrix(glm::vec3(0.0f, 0.0f, 15.0f));
	//testP->_rigidbodyComponent->setDirty();
	//testP->init();


	//todo player collide compo ( sound 는 player 안에서 ), ...
	//todo : camera pos, lookat local modify

	premadeSession->_player = newPlayer;

	// enemy
	for (int i = 0; i < TEST_ENEMY_NUM; i++)
	{
		//Player* newEnemy = new Enemy(planeModel, planeTexture, shaderMain);
		//newEnemy->_normalFObj->setBRender(false);
	}

	// space skybox

	//SHADER::ShaderSkybox* skyboxShader = GShaderManager->m_addShader<SHADER::ShaderSkybox>(ENUM_SHADER_TYPE::SHADER_TYPE_SKYBOX, "data/shader/SkyboxMapping.vertexshader", "data/shader/SkyboxMapping.fragmentshader");
	//RENDER_TARGET::SKYBOX::SkyboxFObj* skyboxFObj = new RENDER_TARGET::SKYBOX::SkyboxFObj("data/Texture/sky_bot.dds", "data/Texture/sky_top.dds", "data/Texture/sky_back.dds", "data/Texture/sky_front.dds", "data/Texture/sky_right.dds", "data/Texture/sky_left.dds", 40.0f);
	//skyboxFObj->setBRender(false);
	
	//SkyboxGObject * spaceSkybox = new SkyboxGObject(skyboxShader, skyboxFObj);
	SkyboxGObject * spaceSkybox = SkyboxGObject::_preMadeSpaceSkybox[0];
	spaceSkybox->setBRender(true);
	newPlayer->attachChildEntity(static_cast<Entity*>(spaceSkybox));
	premadeSession->_spaceSkybox = spaceSkybox;

	preMadeGameSession.push_back(premadeSession);		// premade session
}

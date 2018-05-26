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
#include "src/Shader/ShaderManager.h"
#include "src/Shader/ShaderMain.h"
#include "src/Control/InputManager.h"

#include "src/RenderTarget/Skybox/SkyboxFObj.h"
#include "src/RenderTarget/Normal/NormalFObj.h"
#include "SkyboxGObject.h"
#include "Fobj.h"

#include "Canvas.h"
#include "src/Transform.h"
#include "RigidbodyComponent.h"
#include "src/window.h"

std::vector<GameSession*> GameSession::preMadeGameSession;


GameSession::GameSession()
{
	_menuCanvas = Canvas::preMadeCanvasVec[PREMADE_CANVAS_INGAMEMENU];
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

			it = _allEntityMap.erase(it);
			delete (*it).second;

			continue;
		}
		++it;
	}
}

void GameSession::update(float deltaTime, float acc)
{
	if (!_bMenuOn)
	{
		return;
	}

	// main game logic loop

	// entities game loop
	for (auto elem : _allEntityMap)
	{
		elem.second->logicUpdate(deltaTime, acc);
	}

	removeEntityProcess();
}

// register new entity in game session

void GameSession::registerEntityToGameSession(Entity * entity)
{
	entity->_gameSession = this;
	_allEntityMap.insert(std::make_pair(entity->getID(), entity));
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
	Player * newPlayer = new Player(planeModel, planeTexture, shaderMain);
	//newPlayer->_rigidbodyComponent->_transform->accRotationMatrix(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	newPlayer->_rigidbodyComponent->_transform->setScaleMatrix(glm::vec3(1.0f, 1.0f, 1.0f));
	newPlayer->_rigidbodyComponent->_transform->setDirty();

	CAMERA::Camera** mainCam = GCameraManager->GetMainCamera();
	//(*mainCam)->_rigidbodyComponent->_transform->accRotationMatrix(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	(*mainCam)->_rigidbodyComponent->_transform->accModelMatrix(glm::vec3(0.0f, 0.0f, -14.0f));
	(*mainCam)->_rigidbodyComponent->_transform->setDirty();
	newPlayer->_normalFObj->setBRender(true);
	newPlayer->attachChildEntity(*mainCam);

	//ttest
	Player * testP = new Player(planeModel, planeTexture, shaderMain);
	testP->_rigidbodyComponent->_transform->accModelMatrix(glm::vec3(1.0f, 1.0f, 1.0f));
	testP->_rigidbodyComponent->_transform->setScaleMatrix(glm::vec3(1.0f, 1.0f, 1.0f));
	testP->_rigidbodyComponent->_transform->setDirty();
	newPlayer->attachChildEntity(testP);
	premadeSession->registerEntityToGameSession(testP);


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

	//SHADER::ShaderSkybox* skyboxShader = GShaderManager->m_addShader<SHADER::ShaderSkybox>(ENUM_SHADER_TYPE::SHADER_TYPE_SKYBOX, "data/shader/SkyboxMapping.vertexshader", "data/shader/SkyboxMapping.fragmentshader");
	//RENDER_TARGET::SKYBOX::SkyboxFObj* skyboxFObj = new RENDER_TARGET::SKYBOX::SkyboxFObj("data/Texture/sky_bot.dds", "data/Texture/sky_top.dds", "data/Texture/sky_back.dds", "data/Texture/sky_front.dds", "data/Texture/sky_right.dds", "data/Texture/sky_left.dds", 40.0f);
	//skyboxFObj->setBRender(false);
	
	//SkyboxGObject * spaceSkybox = new SkyboxGObject(skyboxShader, skyboxFObj);
	SkyboxGObject * spaceSkybox = SkyboxGObject::_preMadeSpaceSkybox[0];
	spaceSkybox->setBRender(true);
	premadeSession->_spaceSkybox = spaceSkybox;
	premadeSession->registerEntityToGameSession(spaceSkybox);

	preMadeGameSession.push_back(premadeSession);		// premade session
}

#include "stdafx.h"
#include "Player.h"
#include "src/Resource/Model.h"
#include "src/Resource/Texture.h"
#include "src/Render/RNormal.h"
#include "RText.h"
#include "src/RenderTarget/Normal/NormalFObj.h"
#include "TextFObj.h"
#include "src/Shader/ShaderMain.h"
#include "RenderManager.h"
#include "CollisionComponent.h"
#include "src/Control/InputManager.h"
#include "src/Transform.h"

#include "src/Camera/CameraManager.h"
#include "src/Camera/Camera.h"

#include "./src/Sound/ALManager.h"
#include "./src/Sound/ALSource.h"
#include "CollisionComponentManager.h"
#include "NormalMissile.h"
#include "src/Resource/TextureManager.h"
#include "src/Shader/ShaderManager.h"
#include "src/Shader/ShaderText.h"
#include "ParticleFObj.h"

#include "MissileGeneratorStorage.h"

#include "GameSession.h"
#include "./src/window.h"
#include "./src/Transform.h"

#include "ParticleEntity.h"

Player::Player(GameSession* gSession, RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain)
	: IPlane(ENUM_ENTITY_TYPE::ENUM_ENTITY_PLANE_PLAYER, gSession, model, texture, shadermain)
{
	// aim text
	SHADER::ShaderText* shaderText = GShaderManager->m_addShader<SHADER::ShaderText>(ENUM_SHADER_TYPE::SHADER_TYPE_TEXT, "data/Shader/TextVertexShader.vertexshader", "data/Shader/TextVertexShader.fragmentshader");
	_rText = GRendermanager->getRRender<RENDER::RText, SHADER::ShaderText>(shaderText);
	_rElemInTextAim = _rText->addToDrawList(new RENDER_TARGET::TEXT::TextFObj("data/Texture/Holstein.DDS", "dds", 1, 1, glm::vec2(300, 400), 30) , _rigidbodyComponent);

	// particle
	SHADER::ShaderParticle* shaderParticle = GShaderManager->m_addShader<SHADER::ShaderParticle>(ENUM_SHADER_TYPE::SHADER_TYPE_PARTICLE, "data/Shader/Particle.vertexshader", "data/Shader/Particle.fragmentshader");
	_backParticle = new ParticleEntity(gSession, shaderParticle);
	_backParticle->init(glm::vec3(0.0f, 0.0f, -1.0f), glm::quat(), glm::vec3(0.0f, 0.0f, -1.0f), true, 3.0f, 5);
	//_frontParticle = new ParticleEntity(gSession, shaderParticle);
	//_frontParticle->init(glm::vec3(0.0f, 0.0f, -1.0f), glm::quat(), glm::vec3(0.0f, 0.0f, -1.0f), true, 2.0f, 5);
}

void Player::inputProgress(long long inputKey)
{
	_bShotKeyDown = false;

	// tab key on progress
	if (_gameSession->_bMouseOn)
	{
		tabKeyProgress(inputKey);
		return;
	}

	// not tab key on progress
	if (GInputManager->controlCheck(inputKey, ENUM_BEHAVIOR::TAB_DOWN))
	{
		_gameSession->_bMouseOn = true;
		return;
	}

	playerMovementProgress(inputKey);

	if (GInputManager->controlCheck(inputKey, ENUM_BEHAVIOR::CLICK_L_DOWN))
	{
		// mouse click
		//GInputManager->_mouseXPos

		_bShotKeyDown = true;
	}
}

Player::~Player()
{
}

void Player::init()
{
	// basic info
	_curHp = 100;
	_maxHp = 100;
	_curArmor = 0;
	_maxArmor = 100;
	_deltaSpeed = 0.1f;

	_notDmgedTime = 3.0f;
	_bNotDmged = false;
	_curDmgedTime = 0.0f;

	_missileGeneratorStorage = new MissileGeneratorStorage(PLAYER_DEFAULT_WEAPON_MAX_NUM, this);

	_explosionSound = GALManager->getNewALSource(std::string("explosion"), _rigidbodyComponent->_transform);
	
	glm::mat4 collisionBoxMat = glm::mat4();
	collisionBoxMat[3][2] += 0.2f;	//collision box pos 보정
	glm::vec3 missileCollisionBox = glm::vec3(0.02f, 0.02f, 0.2f);
	_collisionComp = GCollisionComponentManager->GetNewOBBCollisionComp(_rigidbodyComponent, collisionBoxMat, missileCollisionBox);
}

void Player::logicUpdate(float deltaTime, float acc)
{
	collisionLogicUpdate();		// update collision event & clear collision info
	GALManager->updateALListenerWithWorldMat(_rigidbodyComponent->_transform->getWorldMatRef());	 // listener pos update
	
	if(_curHp < 0)
	{
		_explosionSound->play();
		setBRender(false);
		setCollisionTest(false);
	}

	// shot
	_missileGeneratorStorage->update(deltaTime, acc);
	if(_bShotKeyDown)
	{
		_missileGeneratorStorage->shotMissile();
	}

	// overwhelimg
	if(_bNotDmged)
	{
		_curDmgedTime += deltaTime;
		if(_curDmgedTime > _notDmgedTime)
		{
			_bNotDmged = false;		// end overwhelming time
			_curDmgedTime = 0.0f;
			// todo : make plane opacity => 1.0

		}
	}
}

void Player::collisionFunc(CollisionComponent * collisionComp)
{
	// collision event 처리 memeber 함수
	Entity* entity = collisionComp->_rigidComp->_bindedEntity;
	int entityType = entity->getType();

	// missile collision logic은 모두 missile에서.
	switch (entityType)
	{
	case ENUM_ENTITY_PLANE_PLAYER:
		break;
	case ENUM_ENTITY_MISSILE_NORMAL:
		break;
	case ENUM_ENTITY_ENEMY:
		_bNotDmged = true;
		// todo : make plane opacity => 0.5
		break;
	default:
		// none
		break;
	}
}

void Player::doJobWithBeDeleted()
{
	// sound remove
	_explosionSound->unBind();
	_explosionSound->setDoDelete();
}

bool Player::isCanGetDmg()
{
	return !_bNotDmged;

}

void Player::tabKeyProgress(long long transferKeyInput)
{
	// tab key on progress
	if (GInputManager->controlCheck(transferKeyInput, ENUM_BEHAVIOR::TAB_DOWN))
	{
		_gameSession->_bMouseOn = false;
	}
}

void Player::playerMovementProgress(long long transferKeyInput)
{
	// mouse movement process (not tab key on)
	float horizontalAngle = float(GWindow->_windowWidth / 2 - GInputManager->getMouseXPos());
	float verticalAngle = float(GWindow->_windowHeight / 2 - GInputManager->getMouseYPos());

	// camera world rotation
	CAMERA::Camera *maincam = *(GCameraManager->GetMainCamera());
	maincam->camAccQuaternionYaw(horizontalAngle);
	maincam->camAccQuaternionPitch(-verticalAngle);

	int roll = 0;

	if (GInputManager->controlCheck(transferKeyInput, ENUM_BEHAVIOR::MOVE_LEFT))
		roll -= 1;

	if (GInputManager->controlCheck(transferKeyInput, ENUM_BEHAVIOR::MOVE_RIGHT))
		roll += 1;
	
	if (roll != 0)
		maincam->camAccQuaternionRoll((float)roll);
	if (GInputManager->controlCheck(transferKeyInput, ENUM_BEHAVIOR::MOVE_UP))
		_rigidbodyComponent->_transform->speedAdd(_deltaSpeed);

	if (GInputManager->controlCheck(transferKeyInput, ENUM_BEHAVIOR::MOVE_DOWN))
		_rigidbodyComponent->_transform->speedAdd(-_deltaSpeed);

	// set cam position (follow plane)
	maincam->_rigidbodyComponent->_transform->setModelMatrix(_rigidbodyComponent->_transform->getModelVec());
	maincam->_rigidbodyComponent->_transform->translateModelMatrix(glm::vec3(0.0f, 0.0f, -14.0f));

	// plane quaternion rotation to camera rotation
	_rigidbodyComponent->_transform->accQuaternionMix(maincam->_rigidbodyComponent->_transform, _maxAngle, _angleSpeed);
}

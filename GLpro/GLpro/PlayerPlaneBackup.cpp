/*
#include "stdafx.h"
#include "Player.h"
#include "src/Resource/Model.h"
#include "src/Resource/Texture.h"
#include "src/Render/RNormal.h"
#include "src/RenderTarget/Normal/NormalFObj.h"
#include "src/Shader/ShaderMain.h"
#include "RenderManager.h"
#include "CollisionComponent.h"
#include "src/Control/InputManager.h"

#include "src/Camera/CameraManager.h"
#include "src/Camera/Camera.h"

#include "./src/Sound/ALManager.h"
#include "./src/Sound/ALSource.h"
#include "CollisionComponentManager.h"
#include "NormalMissile.h"

#include "MissileGeneratorStorage.h"

#include "GameSession.h"

Player::Player(GameSession* gSession, RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain)
	: IPlane(ENUM_ENTITY_TYPE::ENUM_ENTITY_PLANE_PLAYER, gSession, model, texture, shadermain)
{

}

void Player::inputProgress(long long inputKey)
{
	_bShotKeyDown = false;
	if (GInputManager->controlCheck(inputKey, ENUM_BEHAVIOR::MOVE_LEFT))
	{
		_rigidbodyComponent->accRotationMatrix(_angleSpeedVec[0], glm::vec3(1.0f, 0.0f, 0.0f));
	}

	if (GInputManager->controlCheck(inputKey, ENUM_BEHAVIOR::MOVE_RIGHT))
	{
		// ttest 
		CAMERA::Camera *maincam = *(GCameraManager->GetMainCamera());
		RigidbodyComponent* testComp = this->_rigidbodyComponent;
		//maincam->_rigidbodyComponent->setMove(false);

		std::cout << "cam local" << std::endl;
		maincam->_rigidbodyComponent->printLocalModel();
		maincam->_rigidbodyComponent->printLocalRotMat();

		std::cout << "transform local" << std::endl;
		testComp->printLocalModel();
		testComp->printLocalRotMat();

		std::cout << "cam wolrd" << std::endl;
		maincam->_rigidbodyComponent->printWorldMat();

		std::cout << "transform world" << std::endl;
		testComp->printWorldMat();
	}

	if (GInputManager->controlCheck(inputKey, ENUM_BEHAVIOR::MOVE_UP))
	{
		_rigidbodyComponent->speedAdd(_deltaSpeed);
	}

	if (GInputManager->controlCheck(inputKey, ENUM_BEHAVIOR::MOVE_DOWN))
	{
		_rigidbodyComponent->speedAdd(-_deltaSpeed);
	}

	if (GInputManager->controlCheck(inputKey, ENUM_BEHAVIOR::CLICK_L_DOWN))
	{
		// mouse click
		double xPos, yPos;
		glfwGetCursorPos(GWindow->_pWindow, &xPos, &yPos);
		// printf_s("[LOG] mouse click %lf %lf \n", xPos, yPos);

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

	_explosionSound = GALManager->getNewALSource(std::string("explosion"), _rigidbodyComponent);

	glm::mat4 collisionBoxMat = glm::mat4();
	collisionBoxMat[3][2] += 0.2f;	//collision box pos 보정
	glm::vec3 missileCollisionBox = glm::vec3(0.02f, 0.02f, 0.2f);
	_collisionComp = GCollisionComponentManager->GetNewCollisionComp(_rigidbodyComponent, collisionBoxMat, missileCollisionBox);
}

void Player::logicUpdate(float deltaTime, float acc)
{
	collisionLogicUpdate();		// update collision event & clear collision info
	GALManager->updateALListenerWithWorldMat(_rigidbodyComponent->getWorldMatRef());	 // listener pos update

	if (_curHp < 0)
	{
		_explosionSound->play();
		setBRender(false);
		setCollisionTest(false);
	}

	// shot
	_missileGeneratorStorage->update(deltaTime, acc);
	if (_bShotKeyDown)
	{
		_missileGeneratorStorage->shotMissile();
	}

	// overwhelimg
	if (_bNotDmged)
	{
		_curDmgedTime += deltaTime;
		if (_curDmgedTime > _notDmgedTime)
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
	_explosionSound->setDoDelete();
}

bool Player::isCanGetDmg()
{
	return !_bNotDmged;

}

*/
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
#include "src/Transform.h"

#include "src/Camera/CameraManager.h"
#include "src/Camera/Camera.h"

#include "./src/Sound/ALManager.h"
#include "./src/Sound/ALSource.h"
#include "CollisionComponentManager.h"
#include "NormalMissile.h"

#include "GameSession.h"

Player::Player(RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain)
	: IPlane(ENUM_ENTITY_TYPE::ENUM_ENTITY_PLANE_PLAYER, model, texture, shadermain)
{
	// basic info
	_shotDelay = 0.5f;
	_shotDmg = 10;
	_curHp = 100;
	_maxHp = 100;
	_curArmor = 0;
	_maxArmor = 100;

	_notDmgedTime = 3.0f;
	_bNotDmged = false;
	_curDmgedTime = 0.0f;

	_deltaSpeed = 0.1f;

	_explosionSound = GALManager->getNewALSource(std::string("explosion"), _rigidbodyComponent->_transform);
}

void Player::inputProgress(long long inputKey)
{
	_bShotKeyDown = false;
	if (GInputManager->controlCheck(inputKey, ENUM_BEHAVIOR::MOVE_LEFT))
	{
		_rigidbodyComponent->_transform->accRotationMatrix(0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
	}

	if (GInputManager->controlCheck(inputKey, ENUM_BEHAVIOR::MOVE_RIGHT))
	{
		// ttest 
		CAMERA::Camera *maincam = *(GCameraManager->GetMainCamera());
		RigidbodyComponent* testComp = this->_rigidbodyComponent;
		//maincam->_rigidbodyComponent->_transform->setMove(false);

		std::cout << "cam local" << std::endl;
		maincam->_rigidbodyComponent->_transform->printLocalModel();
		maincam->_rigidbodyComponent->_transform->printLocalRotMat();

		std::cout << "transform local" << std::endl;
		testComp->_transform->printLocalModel();
		testComp->_transform->printLocalRotMat();

		std::cout << "cam wolrd" << std::endl;
		maincam->_rigidbodyComponent->_transform->printWorldMat();

		std::cout << "transform world" << std::endl;
		testComp->_transform->printWorldMat();
	}

	if (GInputManager->controlCheck(inputKey, ENUM_BEHAVIOR::MOVE_UP))
	{
		_rigidbodyComponent->_transform->speedAdd(_deltaSpeed);
	}

	if (GInputManager->controlCheck(inputKey, ENUM_BEHAVIOR::MOVE_DOWN))
	{
		_rigidbodyComponent->_transform->speedAdd(-_deltaSpeed);
	}

	if (GInputManager->controlCheck(inputKey, ENUM_BEHAVIOR::CLICK_L_DOWN))
	{
		// mouse click
		double xPos, yPos;
		glfwGetCursorPos(GWindow->_pWindow, &xPos, &yPos);

		printf_s("[LOG] mouse click %lf %lf \n", xPos, yPos);

		_bShotKeyDown = true;
	}
}

Player::~Player()
{
}

void Player::init()
{
	_shotDelay = 0.5f;
	_shotDmg = 10;
	_curHp = 100;
	_maxHp = 100;
	_curArmor = 0;
	_maxArmor = 100;
	_deltaSpeed = 0.1f;

	glm::mat4 collisionBoxMat = glm::mat4();
	collisionBoxMat[3][2] += 0.2f;	// 보정
	glm::vec3 missileCollisionBox = glm::vec3(0.02f, 0.02f, 0.2f);
	_collisionComp = GCollisionComponentManager->GetNewCollisionComp(_rigidbodyComponent, collisionBoxMat, missileCollisionBox);
}

void Player::logicUpdate(float deltaTime, float acc)
{
	collisionLogicUpdate();		// update collision event & clear collision info
	
	if(_curHp < 0)
	{
		_explosionSound->play();
		setBRender(false);
		setCollisionTest(false);
	}

	_curShotDelay += deltaTime;

	// shot
	if(_bShotKeyDown)
	{
		if(_curShotDelay > _shotDelay)
		{
			
		}
		else
		{
			// delay sound play
		}
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

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

Player::Player(RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain)
	: IPlane(ENUM_ENTITY_TYPE::ENUM_ENTITY_PLANE_PLAYER, model, texture, shadermain)
{
	// basic info
	_shotDelay = 0.5f;
	_shotDmg = 10;
	_hp = 100;
	_maxHp = 100;
	_armor = 0;
	_maxArmor = 100;

	_deltaSpeed = 0.1f;
}

void Player::inputProgress(long long inputKey)
{

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
	}
}

Player::~Player()
{
}


void Player::logicUpdate(float deltaTime, float acc)
{
	collisionLogicUpdate();		// update collision event & clear collision info
	
	// shot
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
		break;
	default:
		// none
	}
}

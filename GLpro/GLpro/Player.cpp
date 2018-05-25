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

Player::Player(RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain) : Entity(0)
{
	_rNormal = GRendermanager->getRRender<RENDER::RNormal, SHADER::ShaderMain>(shadermain);

	_normalFObj = new RENDER_TARGET::NORMAL::NormalFObj(model, texture);
	registeredElemInRenderer = _rNormal->addDrawElem(_normalFObj, _rigidbodyComponent);


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
	_rNormal->removeDrawElem(registeredElemInRenderer);
	delete _normalFObj;
}


void Player::logicUpdate(float deltaTime, float acc)
{
	// shot

	//collisionComp-> use callback
}

void Player::setBRender(bool bRender)
{ 
	_normalFObj->setBRender(bRender);
}

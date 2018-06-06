#include "IFieldItem.h"

#include "RenderManager.h"
#include "src/RenderTarget/Normal/NormalFObj.h"
#include "src/Shader/ShaderMain.h"
#include "CollisionComponent.h"
#include "RigidbodyComponent.h"


IFieldItem::IFieldItem(int type, GameSession * gSession, RESOURCE::Model * model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain)
	:Entity(gSession, type)
{
	_rNormal = GRendermanager->getRRender<RENDER::RNormal, SHADER::ShaderMain>(shadermain);
	rendererElem = _rNormal->addDrawElem(new RENDER_TARGET::NORMAL::NormalFObj(model, texture), _rigidbodyComponent);

	// default value
	_maxAngle = 0.01f;
	_angleSpeed = 0.8f;
	_moveType = ENUM_ITEM_MOVE_STAY;
	_randomInterval = 5.0f;
	_curRandomInterval = 0.0f;
	_curLIfeTime = 30.0f;
	_buffCount = 1;
}

IFieldItem::~IFieldItem()
{
	_collisionComp->setDeleted(true);
}

void IFieldItem::IFieldItemInit(float lifeTime, int buffCount, float randomInterval, ENUM_ITEM_MOVE_TYPE moveType, float angleSpeed)
{
	_curLIfeTime = lifeTime;
	_buffCount = buffCount;
	_angleSpeed = angleSpeed;
	_moveType = moveType;
	_randomInterval = randomInterval;
}

void IFieldItem::setBRender(bool bRender)
{
	rendererElem->first->setBRender(bRender);
}

void IFieldItem::setCollisionTest(bool bCollision)
{
	if (_collisionComp != nullptr)
		_collisionComp->setCollisionTest(bCollision);
}

void IFieldItem::setAngleSpeed(float speed)
{
	_angleSpeed = speed;
}

void IFieldItem::setSpeed(float speed) {
	_rigidbodyComponent->speedSet(speed);
}

void IFieldItem::setMoveType(ENUM_ITEM_MOVE_TYPE moveType) {
	_moveType = moveType;
}

void IFieldItem::setRandomInterval(float interval)
{
	_randomInterval = interval;
}

void IFieldItem::collisionLogicUpdate()
{
	for (auto elem : _collisionComp->_collisionLogList)
	{
		collisionFunc(elem);
	}

	_collisionComp->_collisionLogList.clear();
}


bool IFieldItem::moveLogicUpdate(float deltaTime)
{
	_curLIfeTime -= deltaTime;
	if (_curLIfeTime < 0.0f)
	{
		_bDeleted = true;
		return true;
	}

	switch (_moveType)
	{
	case ENUM_ITEM_MOVE_TYPE::ENUM_ITEM_MOVE_STAY :
		break;
	case ENUM_ITEM_MOVE_TYPE::ENUM_ITEM_MOVE_STAY_ROT:
		_rigidbodyComponent->accQuaternionPitch(10.0f * deltaTime);
		break;
	case ENUM_ITEM_MOVE_TYPE::ENUM_ITEM_MOVE_RAND_LINE:
		_curRandomInterval += deltaTime;
		if (_curRandomInterval > _randomInterval)
		{
			_rigidbodyComponent->setQuaternion(glm::vec3(glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f)));
			_rigidbodyComponent->speedSet(glm::linearRand(0.0f, _rigidbodyComponent->getMaxSpeed()));
		}
		break;
	case ENUM_ITEM_MOVE_TYPE::ENUM_ITEM_MOVE_RAND_QUAT:
		_curRandomInterval += deltaTime;
		if (_curRandomInterval > _randomInterval)
		{
			// set target quat
			_rigidbodyComponent->speedSet(glm::linearRand(0.0f, _rigidbodyComponent->getMaxSpeed()));
		}
		//mix cur quat -> target quat
		break;
	}
	
	return false;
}

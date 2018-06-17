#include "IFieldItem.h"

#include "RenderManager.h"
#include "src/RenderTarget/Normal/NormalFObj.h"
#include "src/Shader/ShaderMain.h"
#include "CollisionComponent.h"
#include "RigidbodyComponent.h"


IFieldItem::IFieldItem(int type, GameSession * gSession, RESOURCE::Model * model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain)
	:IGameObject(type, gSession, model, texture, shadermain)
{
	_rNormal = GRendermanager->getRRender<RENDER::RNormal, SHADER::ShaderMain>(shadermain);
	rendererElem = _rNormal->addDrawElem(new RENDER_TARGET::NORMAL::NormalFObj(model, texture), _rigidbodyComponent);

	// default value
	_maxAngle = 0.01f;
	_angleSpeed = 0.8f;
	_moveType = ENUM_ITEM_MOVE_STAY;
	_randomInterval = 5.0f;
	_timerRandomInterval = 0.0f;
	_timerLIfeTime = 30.0f;
	_itemCount = 1;
	_maxSpeed = 3;
}

IFieldItem::~IFieldItem()
{
}

void IFieldItem::IFieldItemInit(float lifeTime, float activeTime, int buffCount, float randomInterval, ENUM_ITEM_MOVE_TYPE moveType, float angleSpeed)
{
	_lIfeTime = lifeTime;
	_timerLIfeTime = lifeTime;

	_activeTime = activeTime;
	_timeActiveTime = _activeTime;

	_itemCount = buffCount;
	_angleSpeed = angleSpeed;
	_moveType = moveType;
	_randomInterval = randomInterval;
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

bool IFieldItem::moveLogicUpdate(float deltaTime, float acc)
{
	_timeActiveTime += deltaTime;
	if (_timeActiveTime > _activeTime)
	{
		setCollisionTest(true);
	}

	_timerLIfeTime -= deltaTime;
	if (_timerLIfeTime < 0.0f)
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
		_timerRandomInterval += deltaTime;
		if (_timerRandomInterval > _randomInterval)
		{
			_rigidbodyComponent->setQuaternion(glm::vec3(glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f)));
			_rigidbodyComponent->speedSet(glm::linearRand(0.0f, _maxSpeed));
			_timerRandomInterval = 0.0f;
		}
		break;
	case ENUM_ITEM_MOVE_TYPE::ENUM_ITEM_MOVE_RAND_QUAT:
		_timerRandomInterval += deltaTime;
		if (_timerRandomInterval > _randomInterval)
		{
			// set target quat
			_rigidbodyComponent->speedSet(glm::linearRand(0.0f, _maxSpeed));
			_timerRandomInterval = 0.0f;
		}
		//mix cur quat -> target quat
		break;
	}
	
	return false;
}

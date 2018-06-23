#include "./Entity.h"
#include "../RigidbodyComponentManager.h"
#include "../RigidbodyComponent.h"

#include "../GameSession.h"

int Entity::_sMaxID = 0;

Entity::Entity(GameSession * gSession, ENUM_ENTITY_TYPE type)
{
	_ID = _sMaxID;
	_name = std::to_string(_sMaxID);
	_type = type;
	_rigidbodyComponent = GRigidbodyComponentManager->getNewRigidbodyComp(this);
	_parentEntity = nullptr;
	_gameSession = gSession;
	_bDeleted = false;
	++_sMaxID;

	if (_gameSession != nullptr)
	{
		// register new entity in game session
		_gameSession->_allEntityMap.insert(std::make_pair(_ID, this));
	}
}

Entity::Entity(std::string name, GameSession * gSession, ENUM_ENTITY_TYPE type)
{
	_ID = _sMaxID;
	_name = name;
	_type = type;
	_rigidbodyComponent = GRigidbodyComponentManager->getNewRigidbodyComp(this);
	_parentEntity = nullptr;
	_gameSession = gSession;
	_bDeleted = false;
	++_sMaxID;

	if (_gameSession != nullptr)
	{
		// register new entity in game session
		_gameSession->_allEntityMap.insert(std::make_pair(_ID, this));
	}
}

Entity::~Entity() {
	_rigidbodyComponent->setBDeleted();
	
	detachParentEntity();
}

ENUM_ENTITY_TYPE Entity::getType() {
	return _type;
}

int Entity::getID() {
	return _ID;
}

ENUM_ENTITY_TYPE Entity::getType() const{
	return _type;
}

int Entity::getID() const{
	return _ID;
}

bool Entity::isBeDeleted() {
	return _bDeleted;
}

// delete 시 모든 child entity도 delete.
void Entity::setBeDeleted()
{
	_bDeleted = true;
	for (auto elem : _childEntityList)
	{
		elem->setBeDeleted();
	}
	setAllChildBRender(false);
	setAllChildCollisionComp(false);
	//doAllJobWithBeDeleted();
}

void Entity::setName(std::string& name) {
	_name = name;
}

std::string Entity::getName() {
	return _name;
}

const std::string& Entity::getNameRef() const {
	return _name;
}

void Entity::setGameSession(GameSession * gSession)
{
	if (_gameSession != nullptr)
	{
		// 기존 gamesession에서의 gSession 해제
		_gameSession->_allEntityMap.erase(_ID);
		printf_s("setGameSession(GameSession* gSession) : change entity gamesession to another\n");
	}
	_gameSession = gSession;
	_gameSession->_allEntityMap.insert(std::make_pair(_ID, this));
}

GameSession * Entity::getGameSession()
{
	return _gameSession;
}

RigidbodyComponent * Entity::getRigidbodyComponent() {
	return _rigidbodyComponent;
}

Entity * Entity::getParentEntityPtr()
{
	return _parentEntity;
}

Entity * Entity::getChildEntityWithID(int id)
{
	for (auto elem : _childEntityList)
	{
		if (elem->getID() == id)
		{
			return elem;
		}
	}
	return nullptr;
}

Entity * Entity::getChildEntityWithName(const std::string & name)
{
	for (auto elem : _childEntityList)
	{
		if ( elem->getNameRef().compare(name) == 0 )
		{
			return elem;
		}
	}
	return nullptr;
}

Entity * Entity::detachParentEntity()
{
	Entity* ret = _parentEntity;
	if (ret == nullptr)
	{
		return nullptr;
	}
	ret->detachChildEntityWithID(_ID);
	return ret;
}

Entity * Entity::detachChildEntityWithID(int id)
{
	for (auto elem : _childEntityList)
	{
		if (elem->getID() == id)
		{
			// delete child -> parent(this)
			elem->_rigidbodyComponent->detachParentRigidbodyComponent();
			elem->_parentEntity = nullptr;

			// delete parant(this) -> child
			_childEntityList.remove(elem);
			return elem;
		}
	}

	return nullptr;
}

Entity * Entity::detachChildEntityWithName(const std::string & name)
{
	for (auto elem : _childEntityList)
	{
		if (elem->getNameRef().compare(name) == 0)
		{
			// delete child -> parent(this)
			elem->_rigidbodyComponent->detachParentRigidbodyComponent();
			elem->_parentEntity = nullptr;

			// delete parant(this) -> child
			_childEntityList.remove(elem);
			return elem;
		}
	}

	return nullptr;
}

void Entity::attachParentEntity(Entity * parentEntity)
{
	parentEntity->attachChildEntity(this);
}

void Entity::attachChildEntity(Entity * childEntity)
{
	//  check attaching child that has already parent
	if (childEntity->_parentEntity != nullptr)
		childEntity->detachParentEntity();

	_rigidbodyComponent->attachChildRigidbodyComponent(childEntity->_rigidbodyComponent);
	
	_childEntityList.push_back(childEntity);
	childEntity->_parentEntity = this;
	return;
}

void Entity::setAllChildBRender(bool bRender)
{
	setBRender(bRender);
	for (auto elem : _childEntityList)
	{
		elem->setAllChildBRender(bRender);
	}
}

void Entity::setAllChildCollisionComp(bool bCollision)
{
	setCollisionTest(bCollision);
	for (auto elem : _childEntityList)
	{
		elem->setAllChildCollisionComp(bCollision);
	}
}

void Entity::doAllJobWithBeDeleted()
{
	doJobWithBeDeleted();
	for(auto elem : _childEntityList)
	{
		elem->doAllJobWithBeDeleted();
	}
}

void Entity::destroyCallBack()
{
	for (auto elem : _destroyCallBackFuncVec)
	{
		elem();
	}
}

void Entity::registerDestroyCallBackFunc(const std::function<void(void)>& fn)
{
	_destroyCallBackFuncVec.push_back(fn);
}

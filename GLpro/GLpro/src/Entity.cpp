#include "./Entity.h"
#include "../RigidbodyComponentManager.h"
#include "../RigidbodyComponent.h"
#include "./Transform.h"

#include "../GameSession.h"

int Entity::_sMaxID = 0;

Entity::Entity(GameSession * gSession, int type)
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

Entity::Entity(std::string name, GameSession * gSession, int type)
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
	_rigidbodyComponent->_bdoDelete = true;
	detachParentEntity();
}

int Entity::getType() {
	return _type;
}

int Entity::getID() {
	return _ID;
}

int Entity::getType() const{
	return _type;
}

int Entity::getID() const{
	return _ID;
}

bool Entity::isBeDeleted() {
	return _bDeleted;
}

// delete �� ��� child entity�� delete.
void Entity::setBeDeleted()
{
	_bDeleted = true;
	for (auto elem : _childEntityList)
	{
		elem->setBeDeleted();
	}
	setAllChildBRender(false);
	setAllChildCollisionComp(false);
	doAllJobWithBeDeleted();
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
			elem->_rigidbodyComponent->_transform->detachParentTransform();
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
			elem->_rigidbodyComponent->_transform->detachParentTransform();
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

	_rigidbodyComponent->_transform->attachChildTransform(childEntity->_rigidbodyComponent->_transform);
	
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

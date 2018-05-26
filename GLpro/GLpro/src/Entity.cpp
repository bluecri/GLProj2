#include "./Entity.h"
#include "../RigidbodyComponentManager.h"
#include "../RigidbodyComponent.h"
#include "./Transform.h"

int Entity::_sMaxID = 0;

Entity::Entity(int type) {
	_ID = _sMaxID;
	_name = std::to_string(_sMaxID);
	_type = type;
	_rigidbodyComponent = GRigidbodyComponentManager->getNewRigidbodyComp(this);
	_parentEntity = nullptr; 
	_gameSession = nullptr;
	_bDeleted = false;
	++_sMaxID;
}

Entity::Entity(std::string name, int type) {
	_ID = _sMaxID;
	_name = name;
	_type = type;
	_rigidbodyComponent = GRigidbodyComponentManager->getNewRigidbodyComp(this);
	_parentEntity = nullptr;
	_gameSession = nullptr;
	_bDeleted = false;
	++_sMaxID;
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

// delete 시 모든 child entity도 delete.
void Entity::setBeDeleted()
{
	_bDeleted = true;
	for (auto elem : _childEntityList)
	{
		elem->setBeDeleted();
	}
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
	_parentEntity->attachChildEntity(this);
}

void Entity::attachChildEntity(Entity * childEntity)
{
	_rigidbodyComponent->_transform->attachChildTransform(childEntity->_rigidbodyComponent->_transform);
	//childEntity->_rigidbodyComponent->_transform->attachParentTransform(_rigidbodyComponent->_transform);
	
	_childEntityList.push_back(childEntity);
	childEntity->_parentEntity = this;
	return;
}

void Entity::setAllChildBRender(bool bRender)
{
	for (auto elem : _childEntityList)
	{
		elem->setBRender(bRender);
		elem->setAllChildBRender(bRender);
	}
}

void Entity::setBRender(bool bRender) 
{
	// do nothing
}

void Entity::setAllChildCollisionComp(bool bCollision)
{
	for (auto elem : _childEntityList)
	{
		elem->setCollisionTest(bCollision);
		elem->setAllChildCollisionComp(bCollision);
	}
}

void Entity::setCollisionTest(bool bCollision)
{
	//do nothing
}

void Entity::logicUpdate(float deltaTime, float acc) {}



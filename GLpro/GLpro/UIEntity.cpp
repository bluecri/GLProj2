#include "UIEntity.h"

int UIEntity::_sMaxID = 0;

UIEntity::UIEntity(int type) {
	_ID = _sMaxID;
	_name = std::to_string(_sMaxID);
	_type = type;
	_parentUIEntity = nullptr;
	++_sMaxID;
}

UIEntity::UIEntity(std::string name, int type) {
	_ID = _sMaxID;
	_name = name;
	_type = type;
	_parentUIEntity = nullptr;

	++_sMaxID;
}

int UIEntity::getType() {
	return _type;
}

int UIEntity::getID() {
	return _ID;
}

int UIEntity::getType() const {
	return _type;
}

int UIEntity::getID() const {
	return _ID;
}

void UIEntity::setName(std::string & name)
{
	_name = name;
}

std::string UIEntity::getName() {
	return _name;
}

const std::string& UIEntity::getNameRef() const {
	return _name;
}

UIEntity * UIEntity::getParentUIEntityPtr()
{
	return _parentUIEntity;
}

UIEntity * UIEntity::getChildUIEntityWithID(int id)
{
	for (auto elem : _childUIEntityList)
	{
		if (elem->getID() == id)
		{
			return elem;
		}
	}
	return nullptr;
}

UIEntity * UIEntity::getChildUIEntityWithName(const std::string & name)
{
	for (auto elem : _childUIEntityList)
	{
		if (elem->getNameRef().compare(name) == 0)
		{
			return elem;
		}
	}
	return nullptr;
}

UIEntity * UIEntity::detachParentUIEntity()
{
	UIEntity* ret = _parentUIEntity;
	if (ret == nullptr)
	{
		return nullptr;
	}
	ret->detachChildUIEntityWithID(_ID);
	return ret;
}

UIEntity * UIEntity::detachChildUIEntityWithID(int id)
{
	for (auto elem : _childUIEntityList)
	{
		if (elem->getID() == id)
		{
			// delete child -> parent(this)
			elem->_parentUIEntity = nullptr;

			// delete parant(this) -> child
			_childUIEntityList.remove(elem);
			return elem;
		}
	}

	return nullptr;
}

UIEntity * UIEntity::detachChildUIEntityWithName(const std::string & name)
{
	for (auto elem : _childUIEntityList)
	{
		if (elem->getNameRef().compare(name) == 0)
		{
			// delete child -> parent(this)
			elem->_parentUIEntity = nullptr;

			// delete parant(this) -> child
			_childUIEntityList.remove(elem);
			return elem;
		}
	}

	return nullptr;
}

void UIEntity::attachParentUIEntity(UIEntity * parentUIEntity)
{
	_parentUIEntity->attachChildUIEntity(this);
}

void UIEntity::attachChildUIEntity(UIEntity * childUIEntity)
{
	_childUIEntityList.push_back(childUIEntity);
	childUIEntity->_parentUIEntity = this;
	return;
}

void UIEntity::setAllBRender(bool bRender)
{
	for (auto elem : _childUIEntityList)
	{
		elem->setBRender(bRender);
		elem->setAllBRender(bRender);
	}
}


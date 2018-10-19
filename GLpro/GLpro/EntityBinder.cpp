#include "EntityBinder.h"
#include "EntityBindee.h"

//	======================== EntityBinder ========================	//

EntityBinder::EntityBinder(Entity* entity)
{
	_binderEntity = entity;
}

EntityBinder::~EntityBinder()
{
	releaseBindee();
}

EntityBindee * EntityBinder::getBindee()
{
	return _bindee;
}

Entity * EntityBinder::getBindeeEntity()
{
	if (_bindee == nullptr)
		return nullptr;
	return _bindee->_bindeeEntity;
}

void EntityBinder::bindBindee(EntityBindee* newBindee)
{
	releaseBindee();
	if (newBindee == nullptr)
		return;
	newBindee->bindBinder(this);
}

void EntityBinder::releaseBindee()
{
	EntityBindee* releaseBindee = _bindee;
	if(releaseBindee != nullptr)
		releaseBindee->releaseBinder(this);
}

#include "EntityBindee.h"
#include "EntityBinder.h"

//	======================== EntityBindee ========================	//

EntityBindee::EntityBindee(Entity* bindeeEntity)
{
	_bindeeEntity = bindeeEntity;
}

EntityBindee::~EntityBindee()
{
	relaseAllBinder();
}

void EntityBindee::bindBinder(EntityBinder * binder)
{
	binder->_bindee = this;

	_binderSet.insert(binder);
}

void EntityBindee::relaseAllBinder()
{
	for (auto it = _binderSet.begin(); it != _binderSet.end(); it++)
	{
		releaseBinder(*it);
	}

	_binderSet.clear();
}

void EntityBindee::releaseBinder(EntityBinder * binder)
{
	binder->_bindee = nullptr;
	_binderSet.erase(binder);
}

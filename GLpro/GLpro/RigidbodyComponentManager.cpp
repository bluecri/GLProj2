#include "stdafx.h"
#include "RigidbodyComponentManager.h"
#include "RigidbodyComponent.h"
#include "./src/Transform.h"

void RigidbodyComponentManager::updateRigidbodyComps(float deltaTime)
{
	for (auto it = _rigidCompList.begin(); it != _rigidCompList.end(); )
	{
		if ((*it)->_bdoDelete)
		{
			delete (*it);
			it = _rigidCompList.erase(it);
			continue;
		}

		(*it)->_transform->update(deltaTime);
		++it;
	}
}

void RigidbodyComponentManager::resetRigidbodyCompsDirty()
{
	for (auto elem : _rigidCompList)
	{
		elem->_transform->resetDirty();
	}
}

RigidbodyComponent * RigidbodyComponentManager::getNewRigidbodyComp(Entity * bindedEntity)
{
	RigidbodyComponent* ret = new RigidbodyComponent(bindedEntity);
	_rigidCompList.push_back(ret);
	return ret;
}

RigidbodyComponentManager* GRigidbodyComponentManager = nullptr;


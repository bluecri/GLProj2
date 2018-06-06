#include "stdafx.h"
#include "RigidbodyComponentManager.h"
#include "RigidbodyComponent.h"

void RigidbodyComponentManager::updateRigidbodyComps(float deltaTime)
{
	for (auto it = _rigidCompList.begin(); it != _rigidCompList.end(); )
	{
		if ((*it)->isBDeleted())
		{
			delete (*it);
			it = _rigidCompList.erase(it);
			continue;
		}

		(*it)->updateWorldMatrix(deltaTime);
		++it;
	}
}

void RigidbodyComponentManager::resetRigidbodyCompsDirty()
{
	for (auto elem : _rigidCompList)
	{
		elem->resetDirty();
	}
}

RigidbodyComponent * RigidbodyComponentManager::getNewRigidbodyComp(Entity * bindedEntity)
{
	RigidbodyComponent* ret = new RigidbodyComponent(bindedEntity);
	_rigidCompList.push_back(ret);
	return ret;
}

RigidbodyComponentManager* GRigidbodyComponentManager = nullptr;


#pragma once

#include "stdafx.h"

class Entity;
class RigidbodyComponent;

class RigidbodyComponentManager
{
public:
	void updateRigidbodyComps(float deltaTime);
	void resetAndSwapDirtyAll();
	void resetRenderDirtyAll();
	RigidbodyComponent* getNewRigidbodyComp(Entity * bindedEntity);
	
private:
	std::list<RigidbodyComponent*> _rigidCompList;
};

extern RigidbodyComponentManager* GRigidbodyComponentManager;
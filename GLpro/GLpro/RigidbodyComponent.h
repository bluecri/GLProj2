#pragma once

class Transform;
class Entity;

class RigidbodyComponent
{
public:
	RigidbodyComponent(Entity * bindedEntity);
	virtual ~RigidbodyComponent()
	{
		// todo : delete _bindedentity in gameobject
	}

public:
	Entity* _bindedEntity;
	Transform* _transform;
	bool _bdoDelete;
};

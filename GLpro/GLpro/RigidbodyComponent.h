#pragma once

class Transform;
class Entity;

class RigidbodyComponent
{
public:
	RigidbodyComponent(Entity* bindedEntity) : _bindedEntity(bindedEntity) 
	{
		_transform = new Transform(_bindedEntity->getID());
	};
	virtual ~RigidbodyComponent()
	{
		// todo : delete�� _bindedEntity ����..
	}
	Entity* _bindedEntity;
	Transform* _transform;
	int _velX, _velY, _velZ;
};
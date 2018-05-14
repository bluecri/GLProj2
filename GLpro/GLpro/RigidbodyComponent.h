#pragma once

class Entity;
class Transform;

class RigidbodyComponent
{
public:
	RigidbodyComponent(Entity* bindedEntity);
	virtual ~RigidbodyComponent()
	{
		// todo : delete´Â _bindedEntity ¿¡¼­..
	}
	Entity* _bindedEntity;
	Transform* _transform;
	int _velX, _velY, _velZ;
};
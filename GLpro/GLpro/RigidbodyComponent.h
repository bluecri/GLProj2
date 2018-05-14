#pragma once

class Entity;
class Transform;

class RigidbodyComponent
{
public:
	RigidbodyComponent(Entity* bindedEntity);
	virtual ~RigidbodyComponent()
	{
		// todo : delete�� _bindedEntity ����..
	}
	Entity* _bindedEntity;
	Transform* _transform;
	int _velX, _velY, _velZ;
};
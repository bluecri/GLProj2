#pragma once

class Entity;
class Transform;

class RigidbodyComponent
{
public:
	RigidbodyComponent(Entity* bindedEntity);
	virtual ~RigidbodyComponent()
	{
		// todo : delete는 _bindedEntity 에서..(rigidbody, transform 사용하는 component와 함께)
		// delete _transform 하고 entity에서 rigidbody delete 마지막으로
	}

public:
	Entity* _bindedEntity;
	Transform* _transform;
};
#pragma once

class Entity;
class Transform;

class RigidbodyComponent
{
public:
	RigidbodyComponent(Entity* bindedEntity);
	virtual ~RigidbodyComponent()
	{
		// todo : delete�� _bindedEntity ����..(rigidbody, transform ����ϴ� component�� �Բ�)
		// delete _transform �ϰ� entity���� rigidbody delete ����������
	}

public:
	Entity* _bindedEntity;
	Transform* _transform;
};
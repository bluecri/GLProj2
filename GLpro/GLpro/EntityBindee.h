#pragma once

#include "stdafx.h"

class Entity;
class EntityBinder;

/*
*	Binder(M) : Bindee(1)
*	Binder�� Bindee�� entity pointer�� ��ȿ�ϰ� �������� ���� class.
*
*	Binder�� remove�Ǹ� Bindee���� Binder pointer�� ���ŵǸ�
*	Bindee�� remove�Ǹ� �̸� �����ϰ��ִ� ��� Binder���� Bindee pointer�� nullptr�� ����.
*/
class EntityBindee
{
public:
	EntityBindee(Entity* bindeeEntity);
	virtual ~EntityBindee();

	void bindBinder(EntityBinder* binder);
	void relaseAllBinder();
	void releaseBinder(EntityBinder* binder);

private:
	Entity*		_bindeeEntity;
	std::set<EntityBinder*> _binderSet;

	friend class EntityBinder;
};
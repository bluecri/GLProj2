#pragma once

#include "stdafx.h"

class Entity;
class EntityBindee;

/*
*	Binder(M) : Bindee(1)
*	Binder�� Bindee�� entity pointer�� ��ȿ�ϰ� �������� ���� class.
*
*	Binder�� remove�Ǹ� Bindee���� Binder pointer�� ���ŵǸ�
*	Bindee�� remove�Ǹ� �̸� �����ϰ��ִ� ��� Binder���� Bindee pointer�� nullptr�� ����.
*/
class EntityBinder
{
public:
	EntityBinder(Entity* entity);
	virtual ~EntityBinder();

	EntityBindee*	getBindee();
	Entity*			getBindeeEntity();
	void	bindBindee(EntityBindee* newBindee);
	void	releaseBindee();

private:
	Entity*			_binderEntity;
	EntityBindee*	_bindee;

	friend class EntityBindee;
};

#pragma once

#include "stdafx.h"

class Entity;
class EntityBindee;

/*
*	Binder(M) : Bindee(1)
*	Binder가 Bindee의 entity pointer를 유효하게 가져가기 위한 class.
*
*	Binder가 remove되면 Bindee에서 Binder pointer가 제거되며
*	Bindee가 remove되면 이를 참조하고있는 모든 Binder에서 Bindee pointer가 nullptr로 변경.
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

#include "stdafx.h"
#include "RigidbodyComponent.h"
#include "src\Entity.h"
#include "src\Transform.h"

RigidbodyComponent::RigidbodyComponent(Entity * bindedEntity)
	: _bindedEntity(bindedEntity)
{
	_transform = new Transform(_bindedEntity->getID());
	_bdoDelete = false;
}
RigidbodyComponent::~RigidbodyComponent()
{
	// todo : delete transform
	delete _transform;
}
;


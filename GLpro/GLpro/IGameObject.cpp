#include "stdafx.h"
#include "IGameObject.h"

#include "RenderManager.h"
#include "src/RenderTarget/Normal/NormalFObj.h"

#include "RigidbodyComponent.h"
#include "CollisionComponent.h"

IGameObject::IGameObject(ENUM_ENTITY_TYPE type, GameSession * gSession, RESOURCE::Model * model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain)
	: Entity(gSession, type)
{
	if(shadermain != nullptr)
		_rNormal = GRendermanager->getRRender<RENDER::RNormal>(shadermain);
	
	if (model != nullptr && texture != nullptr && shadermain != nullptr)
		registeredElemInRenderer = _rNormal->addDrawElem(new RENDER_TARGET::NORMAL::NormalFObj(model, texture), _rigidbodyComponent);
}

IGameObject::~IGameObject() {
	if (_collisionComp != nullptr)
		_collisionComp->setDeleted(true);

	if (registeredElemInRenderer != nullptr)
		registeredElemInRenderer->first->setBDeleted();
}

void IGameObject::setBRender(bool bRender)
{
	if(registeredElemInRenderer != nullptr)
		registeredElemInRenderer->first->setBRender(bRender);
}

void IGameObject::setCollisionTest(bool bCollision)
{
	if (_collisionComp != nullptr)
		_collisionComp->setCollisionTest(bCollision);
}

void IGameObject::initCollisionComponent(CollisionComponent * collisionComp)
{
	_collisionComp = collisionComp;
}

void IGameObject::collisionLogicUpdate()
{
	if (_collisionComp == nullptr)
		return;

	for (auto elem : _collisionComp->_collisionLogList)
	{
		collisionFunc(elem);
	}

	_collisionComp->_collisionLogList.clear();
}
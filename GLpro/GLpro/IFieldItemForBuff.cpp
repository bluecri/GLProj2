#include "stdafx.h"
#include "IFieldItemForBuff.h"
#include "CollisionComponent.h"
#include "IBuff.h"
#include "IPlane.h"
#include "BuffManager.h"

IFieldItemForBuff::IFieldItemForBuff(ENUM_FieldItemForBuff_TYPE fieldItemBuffType, GameSession * gSession, RESOURCE::Model * model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain)
	: IFieldItem(ENUM_ENTITY_FieldItemForBuff, gSession, model, texture, shadermain)
{
	_fieldItemBuffType = fieldItemBuffType;
}

IFieldItemForBuff::~IFieldItemForBuff()
{
	if (_Ibuff != nullptr)
		delete _Ibuff;
}

ENUM_FieldItemForBuff_TYPE IFieldItemForBuff::getFieldItemForBuffType()
{
	return _fieldItemBuffType;
}

IBuff * IFieldItemForBuff::getCloneBuff() {
	if(_Ibuff != nullptr)
		return _Ibuff->getClone();
	return nullptr;
}

void IFieldItemForBuff::setBuff(IBuff * buff)
{
	_Ibuff = buff;
}

void IFieldItemForBuff::doJobWithBeDeleted()
{
}

void IFieldItemForBuff::logicUpdate(float deltaTime, float acc)
{
	collisionLogicUpdate();
	moveLogicUpdate(deltaTime, acc);

	if (_itemCount <= 0)
	{
		_bDeleted = true;
		return;
	}
}
			
void IFieldItemForBuff::collisionFunc(CollisionComponent * collisionComp)
{
	// collision event 처리 memeber 함수
	 if (_itemCount == 0)
		 return;

	Entity* entity = collisionComp->_rigidComp->getBindedEntity();
	int entityType = entity->getType();

	switch (entityType)
	{
	case ENUM_ENTITY_PLANE_PLAYER:
	case ENUM_ENTITY_ENEMY:
	{
		IPlane* plane = static_cast<IPlane*>(entity);
		int cnt = plane->getBuffManager()->addNewBuff(getFieldItemForBuffType(), _Ibuff->getClone());
		if (cnt != 0)
		{
			_itemCount -= cnt;
			_activeTime = 0.0f;		// reset active time
		}
	}
		break;
	default:
		// none
		break;
	}
}

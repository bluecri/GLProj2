#include "stdafx.h"
#include "ItemSpeedBuff.h"
#include "CollisionComponent.h"
#include "src/Transform.h"

ItemSpeedBuff::ItemSpeedBuff(GameSession * gSession, glm::vec3& genPos)
	: IFieldItem(ENUM_ENTITY_TYPE::ENUM_ENTITY_ITEM_SPEEDBUFF, ENUM_ENTITY_TYPE::ENUM_ENTITY_ITEM_SPEEDBUFF, gSession, m, t, sha)
{
	_rigidbodyComponent->_transform->setModelMatrix(genPos);
}

void ItemSpeedBuff::doJobWithBeDeleted()
{
}

void ItemSpeedBuff::logicUpdate(float deltaTime, float acc)
{
	collisionLogicUpdate();
	moveLogicUpdate(deltaTime);

}

void ItemSpeedBuff::collisionFunc(CollisionComponent * collisionComp)
{
	// collision event 처리 memeber 함수
	Entity* entity = collisionComp->_rigidComp->_bindedEntity;
	int entityType = entity->getType();

	// missile collision logic은 모두 missile에서.
	switch (entityType)
	{
	case ENUM_ENTITY_PLANE_PLAYER:
		break;
	case ENUM_ENTITY_ENEMY:
		break;
	default:
		// none
		break;
	}
}

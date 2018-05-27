#include "stdafx.h"
#include "NormalMissile.h"
#include "RigidbodyComponent.h"
#include "CollisionComponent.h"
#include "Player.h"
#include "Enemy.h"

NormalMissile::NormalMissile(Entity* fromEntity, RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain)
	: IMissile(ENUM_ENTITY_TYPE::ENUM_ENTITY_MISSILE_NORMAL, fromEntity, model, texture, shadermain)
{
}

NormalMissile::~NormalMissile() {}

void NormalMissile::logicUpdate(float deltaTime, float acc)
{
	collisionLogicUpdate();		// update collision event & clear collision info

	_curHitInterval += deltaTime;
	_curLifeTime += deltaTime;

	if (_curLifeTime > _lifeTime || _hitCount == 0)
	{
		setBeDeleted();
	}
}

void NormalMissile::collisionFunc(CollisionComponent * collisionComp)
{
	// collision event 처리 memeber 함수
	Entity* entity = collisionComp->_rigidComp->_bindedEntity;
	int entityType = entity->getType();

	if (_fromEntity == entity)
	{
		return;
	}

	if (!isDmgValid())
	{
		return;
	}


	// missile collision logic은 모두 missile에서.

	switch (entityType)
	{
	case ENUM_ENTITY_PLANE_PLAYER:
	{
		Player* player;
		player = static_cast<Player*>(entity);
		if (player->isCanGetDmg())
		{
			player->_curHp -= getDmg();
			afterDmgOther();
		}
		break;
	case ENUM_ENTITY_MISSILE_NORMAL:
		break;
	case ENUM_ENTITY_ENEMY:
		/*
		Enemy* enemy;
		enemy = static_cast<Enemy*>(entity);
		if (enemy->isCanGetDmg())
		{
		enemy->_curHp -= getDmg();
		afterDmgOther();
		}
		*/
		break;
	default:
		// none
	}
	}
}

void NormalMissile::afterDmgOther()
{
	_curHitInterval = 0.0f;
	accHitCount(-1);
}

bool NormalMissile::isDmgValid()
{
	if (_curHitInterval < _hitInterval || _curLifeTime > _lifeTime || _hitCount == 0 )
	{
		return false;
	}
	return true;
}

int NormalMissile::getDmg()
{
	return _dmg;
}

int NormalMissile::accHitCount(int acc)
{
	_hitCount += acc;			// hit count
	return _hitCount;
}


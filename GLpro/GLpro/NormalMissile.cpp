#include "stdafx.h"
#include "NormalMissile.h"
#include "RigidbodyComponent.h"
#include "CollisionComponent.h"
#include "Player.h"
#include "Enemy.h"

#include "./src/Sound/ALManager.h"
#include "./src/Sound/ALSource.h"

#include "./SpecifiedNormalMissileState.h"
#include "CollisionComponentManager.h"

#include "CommonMissileState.h"

NormalMissile::NormalMissile(Entity* fromEntity, GameSession* gSession, CommonMissileState* commonMissileState)
	: IMissile(ENUM_ENTITY_TYPE::ENUM_ENTITY_MISSILE_NORMAL, gSession, fromEntity, commonMissileState->_missileModel, commonMissileState->_missileTexture, commonMissileState->_missileShaderMain)
{
	_collisionComp = GCollisionComponentManager->GetNewOBBCollisionComp(_rigidbodyComponent, commonMissileState->collisionBoxMat, commonMissileState->missileCollisionBoxAxis);
	_collisionComp->setCollisionVelocityUpdate(false);
}

NormalMissile::~NormalMissile()
{
	// sound remove
	_startSound->unBind();
	_startSound->setDoDelete();
	_hitSound->unBind();
	_hitSound->setDoDelete();
}

void NormalMissile::init(const glm::mat4 & localMissileMat)
{
	_dmg = 10;
	_hitCount = 1;
	_hitInterval = 1.0f;
	_firstSpeed = 1.0f;
	_deltaSpeed = 0.8f;
	_lifeTime = 3.0f;

	_curLifeTime = 0.0f;
	_curHitInterval = 0.0f;

	
	_rigidbodyComponent->setLocalMatWithWorldMat(localMissileMat);
	//_rigidbodyComponent->translateModelMatrix(specifiedNormalMissileState->_missileGenAddPos);
	_rigidbodyComponent->speedAdd(_firstSpeed);

	_rigidbodyComponent->updateWorldMatrix(0.0f);		// update world matrix (not updated by component at first time..)
	
	_startSound = GALManager->getNewALSource(std::string("laser"), _rigidbodyComponent);
	_hitSound = GALManager->getNewALSource(std::string("hit"), _rigidbodyComponent);

	//_startSound->play();		// sound play after worldMat update
}

void NormalMissile::init(const glm::mat4 & localMissileMat, SpecifiedNormalMissileState * specifiedNormalMissileState)
{
	_dmg = specifiedNormalMissileState->_curDmg;
	_hitCount = specifiedNormalMissileState->_curHitCount;
	_hitInterval = specifiedNormalMissileState->_curHitInterval;
	_firstSpeed = specifiedNormalMissileState->_curFirstSpeed;
	_deltaSpeed = specifiedNormalMissileState->_curDeltaSpeed;
	_lifeTime = specifiedNormalMissileState->_curLifeTime;
	_rigidbodyComponent->setMass(specifiedNormalMissileState->_mass);
	_rigidbodyComponent->setMaxSpeed(specifiedNormalMissileState->_maxSpeed);

	_curLifeTime = 0.0f;
	_curHitInterval = 0.0f;
	
	_rigidbodyComponent->setLocalMatWithWorldMat(localMissileMat);
	_rigidbodyComponent->translateModelMatrix(specifiedNormalMissileState->_missileGenAddPos);
	_rigidbodyComponent->speedAdd(_firstSpeed);
	_rigidbodyComponent->updateWorldMatrix(0.0f);		// update world matrix (not updated by component at first time..)

	_startSound = GALManager->getNewALSource(specifiedNormalMissileState->_missileShotSoundStr, _rigidbodyComponent);
	_hitSound = GALManager->getNewALSource(specifiedNormalMissileState->_missileHitSoundStr, _rigidbodyComponent);

	//_startSound->play();		// sound play after worldMat update
}

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
	Entity* entity = collisionComp->_rigidComp->getBindedEntity();
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
		break;
	}
}

void NormalMissile::doJobWithBeDeleted()
{
	
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


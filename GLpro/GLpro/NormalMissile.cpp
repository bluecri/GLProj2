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

#include "SpecifiedNormalMissileState.h"
#include "CommonMissileState.h"

NormalMissile::NormalMissile(Entity* fromEntity, GameSession* gSession, CommonMissileState* commonMissileState)
	: IMissile(ENUM_ENTITY_TYPE::ENUM_ENTITY_MISSILE_NORMAL, gSession, fromEntity, commonMissileState->_missileModel, commonMissileState->_missileTexture, commonMissileState->_missileShaderMain)
{
	_collisionComp = GCollisionComponentManager->GetNewOBBCollisionComp(_rigidbodyComponent, commonMissileState->_collisionBoxMat, commonMissileState->_missileCollisionBoxAxis);
	_collisionComp->setCollisionVelocityUpdate(false);
}

NormalMissile::~NormalMissile()
{
	delete _missileState;
	// sound remove
	_startSound->unBind();
	_startSound->setDoDelete();
	_hitSound->unBind();
	_hitSound->setDoDelete();
}

void NormalMissile::initNormalMissile(const glm::mat4 & localMissileMat)
{
	_missileState = new SpecifiedNormalMissileState();		// copy state (use default)

	_curLifeTime = 0.0f;
	_curHitInterval = 0.0f;
	
	_rigidbodyComponent->setLocalMatWithWorldMat(localMissileMat);
	_rigidbodyComponent->translateModelMatrix(_missileState->_missileGenAddPos);
	_rigidbodyComponent->speedAdd(_missileState->_firstSpeed);

	_rigidbodyComponent->updateWorldMatrix(0.0f);		// update world matrix (not updated by component at first time..)
	
	_startSound = GALManager->getNewALSource(_missileState->_missileShotSoundStr, _rigidbodyComponent);
	_hitSound = GALManager->getNewALSource(_missileState->_missileHitSoundStr, _rigidbodyComponent);
}

void NormalMissile::initNormalMissile(const glm::mat4 & localMissileMat, SpecifiedNormalMissileState * specifiedNormalMissileState)
{
	*_missileState = *specifiedNormalMissileState;		// copy state

	_rigidbodyComponent->setMass(_missileState->_mass);

	_curLifeTime = 0.0f;
	_curHitInterval = 0.0f;
	
	_rigidbodyComponent->setLocalMatWithWorldMat(localMissileMat);
	_rigidbodyComponent->translateModelMatrix(_missileState->_missileGenAddPos);
	_rigidbodyComponent->speedAdd(_missileState->_firstSpeed);
	_rigidbodyComponent->updateWorldMatrix(0.0f);		// update world matrix (not updated by component at first time..)

	_startSound = GALManager->getNewALSource(_missileState->_missileShotSoundStr, _rigidbodyComponent);
	_hitSound = GALManager->getNewALSource(_missileState->_missileHitSoundStr, _rigidbodyComponent);
}

void NormalMissile::logicUpdate(float deltaTime, float acc)
{
	collisionLogicUpdate();		// update collision event & clear collision info

	_curHitInterval += deltaTime;
	_curLifeTime += deltaTime;

	if (_curLifeTime > _missileState->_lifeTime || _missileState->_hitCount == 0)
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
	case ENUM_ENTITY_ENEMY:
		IPlane* iPlane;
		iPlane = static_cast<IPlane*>(entity);
		if (iPlane->getNotDmgedTime() > iPlane->getCurPlaneInfo()->_notDmgedTime)
		{
			iPlane->getCurPlaneInfo()->_hp -= getDmg();
			afterDmgOther();
		}
		break;
	case ENUM_ENTITY_MISSILE_NORMAL:
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
	_missileState->_hitInterval = 0.0f;
	accHitCount(-1);
}

bool NormalMissile::isDmgValid()
{
	if (_curHitInterval < _missileState->_hitInterval || _curLifeTime > _missileState->_lifeTime || _missileState->_hitCount == 0 )
	{
		return false;
	}
	return true;
}

int NormalMissile::getDmg()
{
	return _missileState->_dmg;
}

int NormalMissile::accHitCount(int acc)
{
	_missileState->_hitCount += acc;			// hit count
	return _missileState->_hitCount;
}


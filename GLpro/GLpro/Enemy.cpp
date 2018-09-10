#include "Enemy.h"

#include "src/Resource/Model.h"
#include "src/Resource/Texture.h"
#include "src/Render/RNormal.h"
#include "RText.h"
#include "src/RenderTarget/Normal/NormalFObj.h"
#include "TextFObj.h"
#include "src/Shader/ShaderMain.h"
#include "RenderManager.h"
#include "CollisionComponent.h"
#include "src/Control/InputManager.h"

#include "src/Camera/CameraManager.h"
#include "src/Camera/Camera.h"

#include "./src/Sound/ALManager.h"
#include "./src/Sound/ALSource.h"
#include "CollisionComponentManager.h"
#include "NormalMissile.h"
#include "src/Resource/TextureManager.h"
#include "src/Shader/ShaderManager.h"
#include "src/Shader/ShaderText.h"
#include "ParticleFObj.h"

#include "MissileGeneratorStorage.h"

#include "GameSession.h"
#include "./src/window.h"

#include "ParticleEntity.h"
#include "AimTextUIObj.h"
#include "BuffManager.h"

#include "EntityBinder.h"
#include "Player.h"

Enemy::Enemy(GameSession * gSession, RESOURCE::Model * model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain)
	: IPlane(ENUM_ENTITY_TYPE::ENUM_ENTITY_ENEMY, gSession, model, texture, shadermain)
{
	// particle
	SHADER::ShaderParticle* shaderParticle = GShaderManager->m_addShader<SHADER::ShaderParticle>(ENUM_SHADER_TYPE::SHADER_TYPE_PARTICLE, "data/Shader/Particle.vertexshader", "data/Shader/Particle.fragmentshader");
	_backParticle = new ParticleEntity(gSession, "data/Texture/particle.DDS", "dds", shaderParticle);
	_backParticle->init(glm::vec3(0.0f, 0.0f, -1.0f), glm::quat(), glm::vec3(0.0f, 0.0f, -1.0f), true, 3.0f, 60);

	//_frontParticle = new ParticleEntity(gSession, "data/Texture/particle.DDS", "dds"shaderParticle);
	//_frontParticle->init(glm::vec3(0.0f, 0.0f, 2.0f), glm::quat(), glm::vec3(0.0f, 0.0f, 2.0f), true, 3.0f, 1000);

	_isChasing = false;
	_curChaseTime = 3.0f;
	_targetSpeed = 1.0f;
	_shotDistance = 8.0f;

	_rigidbodyComponent->setBTargetQuat(true);
}

Enemy::~Enemy()
{
	// sound remove
	_explosionSound->unBind();
	_explosionSound->setDoDelete();

	// chase binder delete (deleter excute release)
	delete _chaseTargetBinder;

	// release particle entity auto (particle is child of this entity)

}

void Enemy::initEnemy()
{
	// basic info
	_missileGeneratorStorage = new MissileGeneratorStorage(ENEMY_DEFAULT_WEAPON_MAX_NUM, this);

	_explosionSound = GALManager->getNewALSource(std::string("explosion"), _rigidbodyComponent);

	glm::mat4 collisionBoxMat = glm::mat4();
	collisionBoxMat[3][1] += 0.4f;	//collision box pos 보정
	collisionBoxMat[3][2] += 2.0f;	//collision box pos 보정
	glm::vec3 planeCollisionBox = glm::vec3(1.6f, 0.5f, 3.0f);
	initCollisionComponent(GCollisionComponentManager->GetNewOBBCollisionComp(_rigidbodyComponent, collisionBoxMat, planeCollisionBox));

	std::shared_ptr<RENDER::RNormal::DrawElement> rendererShredElem = getRendererSharedElem();
	rendererShredElem.get()->first->setFrustumRadius(3.5f);
	rendererShredElem.get()->first->setFrustumCompensationPos(glm::vec3(0.0f, 0.0f, 2.0f));

	// particle entity attach parnet
	_backParticle->attachParentEntity(this);
	//_frontParticle->attachParentEntity(this);

	// float param
	_idleSpeedMin = 0.05f;
	_chaseSpeedMin = 1.0f;

	_idleTimeMin	= 10.0f;
	_idleTimeMax	= 20.0f;
	_chaseTimeMin	= 5.0f;
	_chaseTimeMax	= 15.0f;

	// chase binder
	_chaseTargetBinder = new EntityBinder(this);
}

void Enemy::logicUpdate(float deltaTime, float acc)
{
	collisionLogicUpdate();		// update collision event & clear collision info
	GALManager->updateALListenerWithWorldMat(_rigidbodyComponent->getWorldMatRef());	 // listener pos update

	// generator update
	_missileGeneratorStorage->update(deltaTime, acc);

	if (_curPlaneInfo->_hp < 0)
	{
		_explosionSound->play();
		setBeDeleted();
		return;
	}
	
	if (_isChasing)
	{
		_curChaseTime -= deltaTime;
		Player* chaseTargetPlayer = static_cast<Player*>(_chaseTargetBinder->getBindeeEntity());
		if (_curChaseTime < 0.0f || chaseTargetPlayer == nullptr)	// chase end OR target removed
		{
			// set next idleTime
			_curChaseTime = glm::linearRand(_idleTimeMin, _idleTimeMax);
			_isChasing = false;
			_rigidbodyComponent->setTargetQuat(
				glm::toQuat(glm::rotate(glm::mat4(), 
					glm::linearRand(0.0f, 180.0f), 
					glm::vec3(glm::linearRand(-1.0f, 1.0f), glm::linearRand(-1.0f, 1.0f), glm::linearRand(-1.0f, 1.0f))
				))
			);
		}

		// Move speed
		setSpeedWithCurInfo(glm::linearRand(_chaseSpeedMin, _originPlaneInfo->_maxSpeed));

		// Calc Enemy Plane rotation foward to player position + alpha(assuming that player is moving)
		RigidbodyComponent* targetRB = chaseTargetPlayer->getRigidbodyComponent();
		const glm::mat4& targetRotMat = targetRB->getWorldMatRef();
		glm::vec3 distVec = targetRB->getWorldPosVec()
			+ glm::vec3(targetRotMat[2][0], targetRotMat[2][1], targetRotMat[2][2]) * glm::linearRand(0.0f, targetRB->getSpeed())
			- _rigidbodyComponent->getWorldPosVec();

		//distVec *= -1.0f;
		distVec = glm::normalize(distVec);

		//distVec을 normal로 가지는 평면에서의 random vector(up vector)
		float x, y, z;
		x = 0.0f;
		y = -1.0f;
		z = (distVec[0] * x + distVec[1] * y) / (-distVec[2]);

		glm::vec3 upVec = glm::vec3(x, y, z);
		upVec = glm::normalize(upVec);
		glm::quat targetQuat = glm::toQuat(glm::transpose(glm::lookAt(glm::vec3(0.0f), distVec, upVec)));

		_rigidbodyComponent->setTargetQuat(targetQuat);

		if (glm::distance2(targetRB->getWorldPosVec(), _rigidbodyComponent->getWorldPosVec()) < _shotDistance)
		{
			_missileGeneratorStorage->shotMissile();
		}
	}
	else
	{
		_curChaseTime -= deltaTime;
		setSpeedWithCurInfo(glm::linearRand(_idleSpeedMin, _originPlaneInfo->_maxSpeed));
		
		if (_curChaseTime < 0.0f) {
			Entity* chaseTarget = findChaseTarget();
			if (chaseTarget == nullptr)
			{
				_curChaseTime = glm::linearRand(_idleTimeMin, _idleTimeMax);
			}
			else
			{
				setChaseTarget(chaseTarget);
				// set next chaseTime
				_curChaseTime = glm::linearRand(_chaseTimeMin, _chaseTimeMax);
				_isChasing = true;
			}
			
		}
	}

	//collisionParticle end

	/*
	if (collisionParticleTime >= 0.0f) {
		damagedParticleSource->startPos = m_ddoWithCollision->modelVec;
		collisionParticleTime -= deltaTime;
		if (collisionParticleTime < 0.0f)
		{
			collisionParticleTime = 0.0f;
			damagedParticleSource->frameVsParticle = 0;
		}
	}
	*/

	// buff check
	if (_buffManager->isNeedToTransferBuffSum(deltaTime, acc))
	{
		transferBuffSum(_buffManager->getBuffSum());
	}

	// move progress
	playerMovementProgress();

	// back particle logic
	int particleNum = static_cast<int>(ENEMY_MIN_FRAME_PER_PARTICLE + (ENEMY_MAX_FRAME_PER_PARTICLE - ENEMY_MIN_FRAME_PER_PARTICLE) * (1.0f - fabsf(getSpeedPerMaxSpeedRatio())));
	_backParticle->setFrameVsParticle(particleNum);
	
}

void Enemy::collisionFunc(CollisionComponent * collisionComp)
{
	// collision event 처리 memeber 함수
	Entity* entity = collisionComp->_rigidComp->getBindedEntity();
	int entityType = entity->getType();

	switch (entityType)
	{
	case ENUM_ENTITY_PLANE_PLAYER:
		if (isCanBeDamaged())
		{
			planeDamaged(20, true);
		}
		break;
	case ENUM_ENTITY_MISSILE_NORMAL:
		break;
	case ENUM_ENTITY_ENEMY:
		break;
	default:
		// none
		break;
	}
}

void Enemy::doJobWithBeDeleted()
{
}

void Enemy::playerMovementProgress()
{
	float curSpeed = getSpeed();
	float newSpeed = curSpeed;

	// not same
	if (fabs(_targetSpeed - curSpeed) > std::numeric_limits<float>::epsilon())
	{
		if (_targetSpeed < curSpeed)
		{
			newSpeed = curSpeed - _curPlaneInfo->_deltaSpeed;
			newSpeed = std::max(newSpeed, 0.0f);
		}
		else
		{
			newSpeed = curSpeed + _curPlaneInfo->_deltaSpeed;
			newSpeed = std::min(newSpeed, _curPlaneInfo->_maxSpeed);
		}
		_rigidbodyComponent->speedSet(newSpeed);
	}
}

void Enemy::setSpeedWithCurInfo(float speed)
{
	_targetSpeed = std::min(speed, _curPlaneInfo->_maxSpeed);
}

Entity * Enemy::findChaseTarget()
{
	// todo : find chase target


	return nullptr;
}

void Enemy::setChaseTarget(Entity * target)
{
	_chaseTargetBinder->bindBindee(static_cast<Player*>(target)->getChaseBindee());
}

void Enemy::releaseChaseTarget()
{
	_chaseTargetBinder->releaseBindee();
}

EntityBinder * Enemy::getChaseBinder()
{
	return _chaseTargetBinder;
}

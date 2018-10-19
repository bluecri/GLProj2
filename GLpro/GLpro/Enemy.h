#pragma once

#include "./IPlane.h"
#include "./RText.h"
#include "./RParticle.h"

#define ENEMY_DEFAULT_WEAPON_MAX_NUM 6
#define ENEMY_MAX_FRAME_PER_PARTICLE 60
#define ENEMY_MIN_FRAME_PER_PARTICLE 5

namespace SHADER { class ShaderMain; }

class ALSource;
class MissileGeneratorStorage;
class ParticleEntity;
class EntityBinder;

class Enemy : public IPlane
{
public:
	Enemy(GameSession* gSession, RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
	virtual ~Enemy();
	void initEnemy();

	virtual void logicUpdate(float deltaTime, float acc) override;
	virtual void collisionFunc(CollisionComponent* collisionComp) override;
	virtual void doJobWithBeDeleted() override;		// done befoe setBeDeleted (resource bind remove)


private:
	void playerMovementProgress();
	void setSpeedWithCurInfo(float speed);
	void setChaseTarget(Entity* target);
	void releaseChaseTarget();
	EntityBinder*	getChaseBinder();
	Entity*	findChaseTarget();


public:
	ALSource* _explosionSound;

protected:
	ParticleEntity* _backParticle;
	ParticleEntity* _frontParticle;

private:
	bool	_isChasing;
	float	_curChaseTime;
	float	_idleSpeedMin;
	float	_chaseSpeedMin;

	float	_idleTimeMin;
	float	_idleTimeMax;
	float	_chaseTimeMin;
	float	_chaseTimeMax;

	float	_targetSpeed;
	float	_shotDistance;

	EntityBinder*	_chaseTargetBinder;
};

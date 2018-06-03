#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "./IPlane.h"
#include "./RText.h"
#include "./RParticle.h"

#define PLAYER_DEFAULT_WEAPON_MAX_NUM 6
#define PLAYER_MAX_FRAME_PER_PARTICLE 20
#define PLAYER_MIN_FRAME_PER_PARTICLE 1

namespace SHADER { class ShaderMain; }

class ALSource;
class MissileGeneratorStorage;
class ParticleEntity;
class AimTextUIObj;

class Player : public IPlane
{
public:
	Player(GameSession* gSession, RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
	virtual ~Player();
	void init();

	void inputProgress(long long transferKeyInput);
	virtual void logicUpdate(float deltaTime, float acc) override;
	virtual void collisionFunc(CollisionComponent* collisionComp) override;
	virtual void doJobWithBeDeleted() override;		// done befoe setBeDeleted (resource bind remove)

	bool isCanGetDmg();

private:
	void tabKeyProgress(long long transferKeyInput);
	void playerMovementProgress(long long transferKeyInput);

public:
	int _curHp;
	int _maxHp;
	int _curArmor;
	int _maxArmor;

	// overwhelming
	float 	_notDmgedTime;
	bool	_bNotDmged;
	float 	_curDmgedTime;

	float _deltaSpeed;

	ALSource* _explosionSound;

protected:
	RENDER::RText* _rText;
	std::shared_ptr<RENDER::RText::DrawElement> _rElemInTextAim;

	ParticleEntity* _backParticle;
	ParticleEntity* _frontParticle;

	AimTextUIObj* _aimTextUIObj;
private:
	bool _bShotKeyDown;
};

#endif
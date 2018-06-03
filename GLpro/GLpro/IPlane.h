#pragma once

#include "stdafx.h"
#include "./src/Entity.h"
#include "./src/Render/RNormal.h"

class CollisionComponent;
class GameSession;
class MissileGeneratorStorage;
class IMissileGenerator;

namespace RENDER_TARGET { namespace NORMAL { class NormalFObj; } }
namespace SHADER { class ShaderMain; }
namespace RESOURCE { class Model;  class Texture; }

class IPlane : public Entity
{
public:
	IPlane(int type, GameSession* gSession, RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
	virtual ~IPlane();

	virtual void setBRender(bool bRender) override;
	virtual void setCollisionTest(bool bCollision) override;

	bool addMissileGenerator(IMissileGenerator* accMissileGenerator);
	void accAngleSpeed(float acc);
	void setAngleSpeed(float speed);
	float getAngleSpeed();
	MissileGeneratorStorage* getMissileGeneratorStorage();

	void setSpeed(float speed);
	void setMaxSpeed(float speed);
	float getMaxSpeed();
	float getSpeed();
	float getSpeedPerMaxSpeedRatio();

public:
	RENDER::RNormal* _rNormal;		//which rbox get(which shader? use same rbox)
	//RENDER_TARGET::NORMAL::NormalFObj * _normalFObj;
	CollisionComponent * _collisionComp;

protected:
	void collisionLogicUpdate();	// collision events 처리
	virtual void collisionFunc(CollisionComponent* collisionComp) = 0;		// collisionLogicUpdate에서 사용하는 collision 처리 함수

	MissileGeneratorStorage* _missileGeneratorStorage;
	float _maxAngle;		// plane angle (quat rotation speed) = 0.01
	float _angleSpeed;		// plane angle speed (quat rotation speed)
private:
	std::shared_ptr<RENDER::RNormal::DrawElement> registeredElemInRenderer;
};
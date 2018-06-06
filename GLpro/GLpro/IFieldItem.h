#pragma once

#include "stdafx.h"
#include "./src/Entity.h"
#include "./src/Render/RNormal.h"

class CollisionComponent;
class GameSession;

namespace RESOURCE { class Model;  class Texture; }

enum ENUM_ITEM_MOVE_TYPE
{
	ENUM_ITEM_MOVE_STAY,
	ENUM_ITEM_MOVE_STAY_ROT,
	ENUM_ITEM_MOVE_RAND_LINE,
	ENUM_ITEM_MOVE_RAND_QUAT,
	ENUM_ITEM_NUM
};

class IFieldItem : public Entity
{
public:
	IFieldItem(int type, GameSession* gSession, RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
	virtual ~IFieldItem();
	void IFieldItemInit(float lifeTime, int buffCount, float randomInterval = 5.0f, ENUM_ITEM_MOVE_TYPE moveType = ENUM_ITEM_MOVE_STAY, float angleSpeed = 0.8f);

	virtual void setBRender(bool bRender) override;
	virtual void setCollisionTest(bool bCollision) override;

	void setAngleSpeed(float speed);
	void setSpeed(float speed);
	void setMoveType(ENUM_ITEM_MOVE_TYPE moveType);
	void setRandomInterval(float interval);

protected:
	void collisionLogicUpdate();	// collision events 처리
	virtual void collisionFunc(CollisionComponent* collisionComp) = 0;		// collisionLogicUpdate에서 사용하는 collision 처리 함수

	bool moveLogicUpdate(float deltaTime);	// return bDeleted

public:
	RENDER::RNormal* _rNormal;
	CollisionComponent * _collisionComp;

protected:
	float	_maxAngle;		// plane angle (quat rotation speed)
	float	_angleSpeed;		// plane angle speed (quat rotation speed)
	float	_randomInterval;
	int		_buffCount;
	float	_curLIfeTime;
	ENUM_ITEM_MOVE_TYPE _moveType;

	float _maxSpeed;

	bool _bDeleted;
	//	int _itemType;		// to preserve from overlaping identical buff	=> use entity type

private:
	std::shared_ptr<RENDER::RNormal::DrawElement> rendererElem;
	float _curRandomInterval;
};
#pragma once
#include "stdafx.h"
#include "IGameObject.h"

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

class IFieldItem : public IGameObject
{
public:
	IFieldItem(ENUM_ENTITY_TYPE type, GameSession* gSession, RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
	virtual ~IFieldItem();
	void IFieldItemInit(float lifeTime, float activeTime, int buffCount, float randomInterval = 10.0f, ENUM_ITEM_MOVE_TYPE moveType = ENUM_ITEM_MOVE_STAY, float angleSpeed = 0.8f);

	void setAngleSpeed(float speed);
	void setSpeed(float speed);
	void setMoveType(ENUM_ITEM_MOVE_TYPE moveType);
	void setRandomInterval(float interval);

protected:
	bool moveLogicUpdate(float deltaTime, float acc);	// return bDeleted

protected:
	bool	_bDeleted;

	float	_lIfeTime;			// live (0.0 ~ timerLifeTime)
	float	_timerLIfeTime;

	float	_activeTime;		// active (_timerActiveTime ~ INF)
	float	_timeActiveTime;


	float	_maxAngle;			// plane angle (quat rotation speed)
	float	_angleSpeed;		// plane angle speed (quat rotation speed)

	float	_randomInterval;	// direction change interval
	float	_timerRandomInterval;

	int		_itemCount;			// total item count
	
	ENUM_ITEM_MOVE_TYPE _moveType;

	float	_maxSpeed;

	//	int _itemType;		// to preserve from overlaping identical buff	=> use entity type

private:
	std::shared_ptr<RENDER::RNormal::DrawElement> rendererElem;
};
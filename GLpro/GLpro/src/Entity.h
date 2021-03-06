#pragma once

#include "../stdafx.h"
#include "../RigidbodyComponent.h"
/******************************************************
*  Document   :	Entity.h
*  Description: 최상위 object. 생성시 rigidbodyComponent는 반드시 같이 생성된다.
*	Entity의 부모, 자식을 붙일 때 rigidbodyComponent 부모, 자식이 연결되며,
*	component의 경우 부모, 자식 관계가 존재하지 않는다.
*******************************************************/
class GameSession;

enum ENUM_ENTITY_TYPE
{
	ENUM_ENTITY_NOFILTER = -1,
	ENUM_ENTITY_NONE = 0,
	ENUM_ENTITY_CAMERA,
	ENUM_ENTITY_PLANE_PLAYER,
	ENUM_ENTITY_ENEMY,
	ENUM_ENTITY_MISSILE_NORMAL,
	ENUM_ENTITY_PARTICLE,
	ENUM_ENTITY_LIGHT,

	ENUM_ENTITY_FieldItemForBuff,

	ENUM_ENTITY_ITEM_SPEEDBUFF,
	ENUM_ENTITY_ITEM_HP,
	ENUM_ENTITY_ITEM_INVISIBLE,
	ENUM_ENTITY_ITEM_AMMO,
	ENUM_ENTITY_ITEM_ARMOR,
	ENUM_ENTITY_ITEM_OVERWHELIMNG,
	ENUM_ENTITY_ITEM_DMGBUFF,
	ENUM_ENTITY_ITEM_RANDTELEPORT,
	ENUM_ENTITY_ITEM_SHOT_DELAYBUFF,
	ENUM_ENTITY_ITEM_SHOT_SPEEDBUFF,
	ENUM_ENTITY_ITEM_OTHER_DISABLESHOT,
	ENUM_ENTITY_ITEM_OTHER_RAND_DIRECTION,
	ENUM_ENTITY_ITEM_OTHER_SPEEDSLOW,
	ENUM_ENTITY_ITEM_TEAM_DMGBUFF,


	ENUM_ENTITY_NUM
};

class Entity 
{
public:
	Entity(GameSession* gSession = nullptr, ENUM_ENTITY_TYPE type = ENUM_ENTITY_NONE, int categoryBit = 1);
	Entity(std::string name, GameSession* gSession = nullptr, ENUM_ENTITY_TYPE type = ENUM_ENTITY_NONE, int categoryBit = 1);
	virtual	~Entity();

	ENUM_ENTITY_TYPE	getType();
	int					getID();
	ENUM_ENTITY_TYPE	getType() const;
	int					getID() const;
	bool				isBeDeleted();
	void				setBeDeleted();

	void				setName(std::string& name);
	std::string			getName();
	const std::string&	getNameRef() const;

	void				setEntityCategoryBit(int bit);
	bool				testEntityCategoryBit(int testBit);
	int					getEntityCategoryBit();

	void			setGameSession(GameSession* gSession);
	GameSession*	getGameSession();
	RigidbodyComponent*	getRigidbodyComponent();

	Entity*			getParentEntityPtr();
	Entity*			getChildEntityWithID(int id);
	Entity*			getChildEntityWithName(const std::string & name);

	Entity*			detachParentEntity();
	Entity*			detachChildEntityWithID(int id);
	Entity*			detachChildEntityWithName(const std::string& name);

	void			attachParentEntity(Entity* parentEntity);
	void			attachChildEntity(Entity* childEntity);

	virtual void	setAllChildBRender(bool bRender);
	virtual void	setBRender(bool bRender) = 0;		// many class use diverse renderer

	virtual void	setAllChildCollisionComp(bool bCollision);
	virtual void	setCollisionTest(bool bCollision) = 0;

	virtual void	doAllJobWithBeDeleted();
	virtual void	doJobWithBeDeleted() = 0;		// done befoe setBeDeleted (resource bind remove)

	virtual void	logicUpdate(float deltaTime, float acc) = 0;

	void			destroyCallBack();
	void			registerDestroyCallBackFunc(const std::function<void(void)>& fn);

public:
	static int			 _sMaxID;		//object 고유 ID(0 ~ ). 자동 생성.

protected:
	Entity*				_parentEntity;
	std::list<Entity*>	_childEntityList;

	GameSession*		_gameSession;		//registered gamesession
	RigidbodyComponent* _rigidbodyComponent;

private:
	int					_ID;
	int					_entityCategoryBit;
	ENUM_ENTITY_TYPE	_type;
	std::string			_name;
	bool				_bDeleted;

	std::vector<std::function<void(void)>>	_destroyCallBackFuncVec;
};
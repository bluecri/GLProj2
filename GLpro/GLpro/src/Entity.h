#pragma once

#include "../stdafx.h"
#include "../RigidbodyComponent.h"
/******************************************************
*  Document   :	Entity.h
*  Description: �ֻ��� object. ������ rigidbodyComponent�� �ݵ�� ���� �����ȴ�.
*	Entity�� �θ�, �ڽ��� ���� �� rigidbodyComponent �� transform�� �θ�, �ڽ��� ����Ǹ�,
*	component�� ��� �θ�, �ڽ� ���谡 �������� �ʴ´�.
*******************************************************/
class GameSession;
class CollisionInfo;

enum ENUM_ENTITY_TYPE
{
	ENUM_ENTITY,
	ENUM_ENTITY_PLANE_PLAYER,
	ENUM_ENTITY_ENEMY,
	ENUM_ENTITY_MISSILE_NORMAL,
	ENUM_ENTITY_NUM
};

class Entity 
{
public:
	Entity(int type = 0);
	Entity(std::string name, int type = 0);
	virtual ~Entity();

	int getType();
	int getID();
	int getType() const;
	int getID() const;
	bool isBeDeleted();
	void setBeDeleted();

	void setName(std::string& name);
	std::string getName();
	const std::string& getNameRef() const;

	Entity* getParentEntityPtr();
	Entity* getChildEntityWithID(int id);
	Entity* getChildEntityWithName(const std::string & name);

	Entity* detachParentEntity();
	Entity* detachChildEntityWithID(int id);
	Entity* detachChildEntityWithName(const std::string& name);

	void attachParentEntity(Entity* parentEntity);
	void attachChildEntity(Entity* childEntity);

	virtual void setAllChildBRender(bool bRender);
	virtual void setBRender(bool bRender) = 0;		// many class use diverse renderer

	virtual void setAllChildCollisionComp(bool bCollision);
	virtual void setCollisionTest(bool bCollision) = 0;

	virtual void doAllJobWithBeDeleted();
	virtual void doJobWithBeDeleted() = 0;		// done befoe setBeDeleted (resource bind remove)

	virtual void logicUpdate(float deltaTime, float acc) = 0;

public:
	static int _sMaxID;		//object ���� ID(0 ~ ). �ڵ� ����.
	std::string _name;
	GameSession* _gameSession;		//registered gamesession

	Entity* _parentEntity;
	std::list<Entity*> _childEntityList;
	RigidbodyComponent* _rigidbodyComponent;

private:
	int _type = 0;
	int _ID;
	bool _bDeleted;

};
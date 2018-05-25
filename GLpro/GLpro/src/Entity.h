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

class Entity 
{
public:
	Entity(int type = 0);
	Entity(std::string name, int type = 0);
	virtual ~Entity() {}

	int getType();
	int getID();
	int getType() const;
	int getID() const;
	void setName(std::string& name);
	std::string getName();
	const std::string& getNameRef() const;

	virtual Entity* getParentEntityPtr();
	virtual Entity* getChildEntityWithID(int id);
	virtual Entity* getChildEntityWithName(const std::string & name);

	virtual Entity* detachParentEntity();
	virtual Entity* detachChildEntityWithID(int id);
	virtual Entity* detachChildEntityWithName(const std::string& name);

	virtual void attachParentEntity(Entity* parentEntity);
	virtual void attachChildEntity(Entity* childEntity);

	virtual void setAllChildBRender(bool bRender);
	virtual void setBRender(bool bRender);

	virtual void setAllChildCollisionComp(bool bCollision);
	virtual void setCollisionComp(bool bCollision);

	virtual void logicUpdate(float deltaTime, float acc);
	virtual void collisionCallBack(CollisionInfo* collisionInfo);

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

};
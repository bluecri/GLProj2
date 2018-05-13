#include "../stdafx.h"

#include "../RigidbodyComponent.h"
/******************************************************
*  Document   :	Entity.h
*  Description: 최상위 object. 생성시 rigidbodyComponent는 반드시 같이 생성된다.
*	Entity의 부모, 자식을 붙일 때 rigidbodyComponent 안 transform의 부모, 자식이 연결되며,
*	component의 경우 부모, 자식 관계가 존재하지 않는다.
*******************************************************/
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

public:
	static int _sMaxID;		//object 고유 ID(0 ~ ). 자동 생성.
	std::string _name;

	Entity* _parentEntity;
	std::list<Entity*> _childEntityList;
	RigidbodyComponent* _rigidbodyComponent;

private:
	int _type = 0;
	int _ID;

};
#pragma once
#include "stdafx.h"

/******************************************************
*  Document   :	UIEntity.h
*  Description: 최상위 UI object. UIEntity와 같지만 rigidbodyComponent가 존재하지 않음.
*	UIEntity의 부모, 자식을 붙일 때 부모, 자식이 연결된다.
*******************************************************/
class UIEntity
{
public:
	UIEntity(int type = 0);
	UIEntity(std::string name, int type = 0);
	virtual ~UIEntity() {}

	int getType();
	int getID();
	int getType() const;
	int getID() const;
	void setName(std::string& name);
	std::string getName();
	const std::string& getNameRef() const;

	virtual UIEntity* getParentUIEntityPtr();
	virtual UIEntity* getChildUIEntityWithID(int id);
	virtual UIEntity* getChildUIEntityWithName(const std::string & name);

	virtual UIEntity* detachParentUIEntity();
	virtual UIEntity* detachChildUIEntityWithID(int id);
	virtual UIEntity* detachChildUIEntityWithName(const std::string& name);

	virtual void attachParentUIEntity(UIEntity* parentUIEntity);
	virtual void attachChildUIEntity(UIEntity* childUIEntity);

	virtual void setAllBRender(bool bRender);
	virtual void setBRender(bool bRender) = 0;		//must implement

public:
	static int _sMaxID;		//object 고유 ID(0 ~ ). 자동 생성.
	std::string _name;

	UIEntity* _parentUIEntity;
	std::list<UIEntity*> _childUIEntityList;

private:
	int _type = 0;
	int _ID;
};
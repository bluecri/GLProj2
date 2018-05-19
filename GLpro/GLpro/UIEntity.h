#pragma once
#include "stdafx.h"

/******************************************************
*  Document   :	UIEntity.h
*  Description: �ֻ��� UI object. UIEntity�� ������ rigidbodyComponent�� �������� ����.
*	UIEntity�� �θ�, �ڽ��� ���� �� �θ�, �ڽ��� ����ȴ�.
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
	static int _sMaxID;		//object ���� ID(0 ~ ). �ڵ� ����.
	std::string _name;

	UIEntity* _parentUIEntity;
	std::list<UIEntity*> _childUIEntityList;

private:
	int _type = 0;
	int _ID;
};
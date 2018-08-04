#pragma once
#include "stdafx.h"

class CollisionComponent;
class OctreeElem;

enum OCT_POS
{
	OCT_LTF,	// left top front
	OCT_RTF,
	OCT_LBF,
	OCT_RBF,
	OCT_LTB,
	OCT_RTB,
	OCT_LBB,
	OCT_RBB,	// right bottom back
	OCT_NUM
};

class OctreeForCollision
{
public:
	OctreeForCollision(int height, int halfAxisSize, glm::vec3 center);
	void initOctreeElem(OctreeElem& elem);
	void newlyInsertComponent(CollisionComponent* comp);

	// 충돌 가능한 모든 CollisionComponent들을 potentialList에 등록
	void getCollisionPotentialList(std::list<CollisionComponent*>& potentialList, CollisionComponent* comp);
	void getAllCollisionPotentialList(std::list<CollisionComponent*>& potentialList, int idx);

	// 등록해놓은 모든 CollisionComponent refresh.
	void clearPotentialCompPropa();

	// self update
	void doOctreeUpdate();

private:
	void insertComponent(CollisionComponent* comp);
	// insert 가능한 child box index return
	int getFitChildBoxIndex(OctreeElem& octreeElem, CollisionComponent * comp);

	//bool IsInBoxFitTest(CollisionComponent* comp);
	//int IsInBoxTestAll(CollisionComponent* comp);
	void removeCopmInOctreeElem(CollisionComponent* comp);


	bool IsUseThisOctreeElem(OctreeElem& elem);
	bool IsUseChild(OctreeElem& elem);
public:
	std::vector<OctreeElem>		_octreeElemVec;
	std::list<CollisionComponent*> _usingComponents;

	int  _maxCountOfObjects;
};
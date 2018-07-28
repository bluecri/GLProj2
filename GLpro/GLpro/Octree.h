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
	void insert(CollisionComponent* comp);

	// �浹 ������ ��� CollisionComponent���� potentialList�� ���
	void getCollisionPotentialList(std::list<CollisionComponent*>& potentialList, CollisionComponent* comp);
	void getAllCollisionPotentialList(std::list<CollisionComponent*>& potentialList, int idx);

	// ����س��� ��� CollisionComponent refresh.
	void clearPotentialCompPropa();

private:
	// insert ������ child box index return
	int getFitChildBoxIndex(OctreeElem& octreeElem, CollisionComponent * comp);

	//bool IsInBoxFitTest(CollisionComponent* comp);
	//int IsInBoxTestAll(CollisionComponent* comp);

public:
	//int _height;
	//int _halfAxisSize;
	//glm::vec3 _center;
	std::vector<OctreeElem>		_octreeElemVec;
	std::list<CollisionComponent*> _potentialComponents;

	//bool _bUsed;				// Whether this octree & children is used or not
	//bool _bUseChildren;	
	int  _maxCountOfObjects;
};
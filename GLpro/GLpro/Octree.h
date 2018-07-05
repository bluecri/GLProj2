#pragma once
#include "stdafx.h"

class CollisionComponent;

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
	void insert(CollisionComponent* comp);

	// �浹 ������ ��� CollisionComponent���� potentialList�� ���
	void getCollisionPotentialList(std::list<CollisionComponent*>& potentialList, CollisionComponent* comp);

	// ����س��� ��� CollisionComponent refresh.
	void clearPotentialCompPropa();

private:
	// insert ������ child box index return
	int getFitChildBoxIndex(CollisionComponent* comp);

	bool IsInBoxTest(CollisionComponent* comp);

public:
	int _height;
	int _halfAxisSize;
	glm::vec3 _center;
	OctreeForCollision* _childTree[OCT_POS::OCT_NUM];
	std::list<CollisionComponent*> _potentialComponents;

	bool _bUsed;				// Whether this octree & children is used or not
	bool _bUseChildren;	
	int  _maxCountOfObjects;
};
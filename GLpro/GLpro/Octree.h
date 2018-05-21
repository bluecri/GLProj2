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

class Octree
{
public:
	Octree(int height, int halfAxisSize, glm::vec3 center)
	{
		_center = center;
		_halfAxisSize = halfAxisSize;
		_height = height;

		if (height <= 0)
		{
			for (int i = 0; i < OCT_NUM; i++)
			{
				_childTree[i] = nullptr;
			}
		}
		else
		{
			for (int FB = 0; FB < 2; FB++) {
				for (int TB = 0; TB < 2; TB++) {
					for (int LR = 0; LR < 2; LR++) {
						{
							glm::vec3 childCenter = _center;
							int childIdx = LR + TB * 2 + FB * 4;
							childCenter.x += ((LR * 2) - 1) * _halfAxisSize / 2;	// (-1, 1) * half_half size
							childCenter.y += ((TB * 2) - 1) * _halfAxisSize / 2;
							childCenter.z += ((FB * 2) - 1) * _halfAxisSize / 2;
							_childTree[childIdx] = new Octree(height - 1, halfAxisSize / 2, childCenter);
						}
					}
				}
			}
		}
	}

	void insert(CollisionComponent* comp)
	{
		int targetIdx = -1;
		if (-1 == (targetIdx = getFitChildBoxIndex(comp)))
		{
			_potentialComponents.push_back(comp);
			return;
		}

		_childTree[targetIdx]->insert(comp);

		return;
	}

	// �浹 ������ ��� CollisionComponent���� potentialList�� ���
	void getCollisionPotentialList(std::list<CollisionComponent*>& potentialList, CollisionComponent* comp)
	{
		for (auto elem : _potentialComponents)
		{
			potentialList.push_back(elem);
		}

		if (_height == 0)
		{
			return;
		}

		int targetIdx = -1;
		// doopt : comp�� childBox Index�� �̸� �����س��� ����ϴ� ��� ����.
		if (-1 == (targetIdx = getFitChildBoxIndex(comp)))
		{
			return;
		}

		_childTree[targetIdx]->getCollisionPotentialList(potentialList, comp);

		return;
	}

	// ����س��� ��� CollisionComponent refresh. ( doopt : ���� octree���� check�� ����ġ�� �ٽ� root���� �����ϴ� �ٸ� ��� ����)
	void clearPotentialCompPropa()
	{
		_potentialComponents.clear();
		if (_height == 0)
		{
			return;
		}

		for (int i = 0; i < OCT_POS::OCT_NUM; i++)
		{
			_childTree[i]->clearPotentialCompPropa();
		}
	}
private:
	// insert ������ child box index return
	int getFitChildBoxIndex(CollisionComponent* comp)
	{
		if (_height == 0)
		{
			return -1;
		}
		for (int i = 0; i < OCT_POS::OCT_NUM; i++)
		{
			if (_childTree[i]->IsInBoxTest(comp))
			{
				return i;
			}
		}
		return -1;	// ��ġ�ϴ� ���� child box�� �������� ����.
	}

	bool IsInBoxTest(CollisionComponent* comp)
	{
		/* AABB test
		for (int i = 0; i < 3; i++)
		{
			if (fabs(_center[i] - comp->_center[i]) >(_halfAxisSize[i] + comp->_halfAxisSize[i]))
				return false;
		}
		*/
		// in box text
		for (int i = 0; i < 3; i++)
		{
			// out condition ( center diff + half > HALF )
			if (fabs(_center[i] - comp->_center[i] + comp->_halfAxisSize[i]) > _halfAxisSize)
				return false;
		}

		return true;
	}

public:
	int _height;
	int _halfAxisSize;
	glm::vec3 _center;
	Octree* _childTree[OCT_POS::OCT_NUM];
	std::list<CollisionComponent*> _potentialComponents;
};
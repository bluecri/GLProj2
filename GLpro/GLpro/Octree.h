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

	// 충돌 가능한 모든 CollisionComponent들을 potentialList에 등록
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
		// doopt : comp에 childBox Index를 미리 저장해놓고 사용하는 방법 있음.
		if (-1 == (targetIdx = getFitChildBoxIndex(comp)))
		{
			return;
		}

		_childTree[targetIdx]->getCollisionPotentialList(potentialList, comp);

		return;
	}

	// 등록해놓은 모든 CollisionComponent refresh. ( doopt : 기존 octree에서 check후 불일치시 다시 root부터 시작하는 다른 방법 있음)
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
	// insert 가능한 child box index return
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
		return -1;	// 일치하는 하위 child box가 존재하지 않음.
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
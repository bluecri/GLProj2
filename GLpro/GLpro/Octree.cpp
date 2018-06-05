#include "stdafx.h"
#include "Octree.h"

#include "CollisionComponent.h"

OctreeForCollision::OctreeForCollision(int height, int halfAxisSize, glm::vec3 center)
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
						_childTree[childIdx] = new OctreeForCollision(height - 1, _halfAxisSize / 2, childCenter);
					}
				}
			}
		}
	}
}

void OctreeForCollision::insert(CollisionComponent * comp)
{
	int targetIdx = -1;
	// doopt : _potentialComponents 개수가 적으면 stop. 일정 수가 넘어가면 recursion.
	if (-1 == (targetIdx = getFitChildBoxIndex(comp)))
	{
		_potentialComponents.push_back(comp);
		return;
	}

	_childTree[targetIdx]->insert(comp);

	return;
}

// 충돌 가능한 모든 CollisionComponent들을 potentialList에 등록

void OctreeForCollision::getCollisionPotentialList(std::list<CollisionComponent*>& potentialList, CollisionComponent * comp)
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
	// doopt : comp에 childBox Index를 미리 저장해놓고 같은 node로 collision 확인.
	// object가 octree node에서 변경이 드문 경우 하고 불일치시에 처음부터 collision check 하는 방법
	if (-1 == (targetIdx = getFitChildBoxIndex(comp)))
	{
		return;
	}

	_childTree[targetIdx]->getCollisionPotentialList(potentialList, comp);

	return;
}

// 등록해놓은 모든 CollisionComponent refresh.

void OctreeForCollision::clearPotentialCompPropa()
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

// insert 가능한 child box index return

int OctreeForCollision::getFitChildBoxIndex(CollisionComponent * comp)
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

bool OctreeForCollision::IsInBoxTest(CollisionComponent * comp)
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

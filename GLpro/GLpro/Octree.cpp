#include "stdafx.h"
#include "Octree.h"

#include "CollisionComponent.h"

OctreeForCollision::OctreeForCollision(int height, int halfAxisSize, glm::vec3 center)
{
	_center = center;
	_halfAxisSize = halfAxisSize;
	_height = height;
	_bUsed = false;
	_bUseChildren = false;

	_maxCountOfObjects = 8;

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
	_bUsed = true;
	int targetIdx = -1;


	if (_bUseChildren)
	{
		if (-1 == (targetIdx = getFitChildBoxIndex(comp)))
		{
			_potentialComponents.push_back(comp);
			return;
		}

		_childTree[targetIdx]->insert(comp);
		return;
	}
	else
	{
		_potentialComponents.push_back(comp);

		//_potentialComponents 개수가 적으면 stop. 일정 수가 넘어가면 children으로.
		if (_maxCountOfObjects < _potentialComponents.size() && _height != 0)
		{
			_bUseChildren = true;

			for (auto it = _potentialComponents.begin(); it != _potentialComponents.end();	)
			{
				if (-1 != (targetIdx = getFitChildBoxIndex(*it) ))
				{
					// push to child from this(parent)
					_childTree[targetIdx]->insert(*it);
					it = _potentialComponents.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
	}
	

	return;
}

// 충돌 가능한 모든 CollisionComponent들을 potentialList에 등록
void OctreeForCollision::getCollisionPotentialList(std::list<CollisionComponent*>& potentialList, CollisionComponent * comp)
{
	if (_bUsed == false)
		return;

	for (auto elem : _potentialComponents)
	{
		potentialList.push_back(elem);
	}

	if (_height == 0)
	{
		return;
	}

	if (_bUseChildren == false)
		return;

	int targetIdx = -1;

	// doopt : comp에 childBox Index를 미리 저장해놓고 같은 node로 collision 확인.
	// object가 octree node에서 변경이 드문 경우 하고 불일치시에 처음부터 collision check 하는 방법
	if (-1 == (targetIdx = getFitChildBoxIndex(comp)))
	{
		for (int i = 0; i < OCT_NUM; i++)
			_childTree[targetIdx]->getCollisionPotentialList(potentialList, comp);
	}

	return;
}

void OctreeForCollision::getAllCollisionPotentialList(std::list<CollisionComponent*>& potentialList)
{
	if (_bUsed == false)
		return;

	for (auto elem : _potentialComponents)
	{
		potentialList.push_back(elem);
	}

	if (_bUseChildren)
	{
		for (int i = 0; i < OCT_NUM; i++)
		{
			_childTree[i]->getAllCollisionPotentialList(potentialList);
		}
	}

	return;
}



// 등록해놓은 모든 CollisionComponent refresh.
void OctreeForCollision::clearPotentialCompPropa()
{
	if (_bUsed == false)
		return;

	_potentialComponents.clear();
	_bUsed = false;
	_maxCountOfObjects = 0;

	if (_height == 0)
	{
		return;
	}

	if (_bUseChildren)
	{
		for (int i = 0; i < OCT_POS::OCT_NUM; i++)
		{
			_childTree[i]->clearPotentialCompPropa();
		}
	}
	_bUseChildren = false;
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
		if (_childTree[i]->IsInBoxFitTest(comp))
		{
			return i;
		}
	}
	return -1;	// 일치하는 하위 child box가 존재하지 않음.
}

// component가 완전히 inbox인지 check. (intersect == outside)
bool OctreeForCollision::IsInBoxFitTest(CollisionComponent * comp)
{
	/* AABB test
	for (int i = 0; i < 3; i++)
	{
	if (fabs(_center[i] - comp->_center[i]) >(_halfAxisSize[i] + comp->_halfAxisSize[i]))
	return false;
	}
	*/

	const AABBOb& aabbOb = comp->_aabbObForOctree;
	const glm::vec3& aabbCemter = aabbOb.getCenterConstRef();
	const glm::vec3& aabbAxis = aabbOb.getAxisConstRef();


	// in box text
	for (int i = 0; i < 3; i++)
	{
		// out condition ( center diff + half > HALF )
		if (fabs(_center[i] - aabbCemter[i]) > fabs (_halfAxisSize - aabbAxis[i]))
			return false;
	}

	return true;
}

// component - box check. (inside, intersect, outside)
int OctreeForCollision::IsInBoxTestAll(CollisionComponent * comp)
{
	int ret = 1;		// inside

	const AABBOb& aabbOb = comp->_aabbObForOctree;
	const glm::vec3& aabbCemter = aabbOb.getCenterConstRef();
	const glm::vec3& aabbAxis = aabbOb.getAxisConstRef();

	// in box text
	for (int i = 0; i < 3; i++)
	{
		// out condition ( center diff > HALF + half )
		if (fabs(_center[i] - aabbCemter[i]) > _halfAxisSize + aabbAxis[i])
			return -1;	// outside

		// intersection condition ( center diff <= HALF - half )
		if (fabs(_center[i] - aabbCemter[i]) > fabs(_halfAxisSize - aabbAxis[i]))
			ret =  0;	// outside
	}

	return ret;
}

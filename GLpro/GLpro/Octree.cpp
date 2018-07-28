#include "stdafx.h"
#include "Octree.h"
#include "OctreeElem.h"

#include "CollisionComponent.h"

OctreeForCollision::OctreeForCollision(int height, int halfAxisSize, glm::vec3 center)
{
	/*
	_center = center;
	_halfAxisSize = halfAxisSize;
	_height = height;
	_bUsed = false;
	_bUseChildren = false;
	*/

	_maxCountOfObjects = 8;

	_octreeElemVec.reserve(pow(8, height) + 1);
	_octreeElemVec = std::vector<OctreeElem>(pow(8, height) + 1, OctreeElem());

	int index = 0;
	_octreeElemVec[index]._height = height;
	_octreeElemVec[index]._halfAxisSize = halfAxisSize;
	_octreeElemVec[index]._center = center;
	_octreeElemVec[index]._index = index;

	initOctreeElem(_octreeElemVec[index]);

}

void OctreeForCollision::initOctreeElem(OctreeElem& elem)
{
	if (elem._height <= 0)
	{
		return;
	}

	for (int FB = 0; FB < 2; FB++) {
		for (int TB = 0; TB < 2; TB++) {
			for (int LR = 0; LR < 2; LR++) {
				{
					glm::vec3 childCenter = elem._center;
					int childIdx = elem._index * 8 + ( LR + TB * 2 + FB * 4 + 1 );
					if (childIdx >= _octreeElemVec.size())
						return;

					childCenter.x += ((LR * 2) - 1) * elem._halfAxisSize / 2;	// (-1, 1) * half_half size
					childCenter.y += ((TB * 2) - 1) * elem._halfAxisSize / 2;
					childCenter.z += ((FB * 2) - 1) * elem._halfAxisSize / 2;

					_octreeElemVec[childIdx]._height = elem._height - 1;
					_octreeElemVec[childIdx]._halfAxisSize = elem._halfAxisSize / 2;
					_octreeElemVec[childIdx]._center = childCenter;
					_octreeElemVec[childIdx]._index = childIdx;
					initOctreeElem(_octreeElemVec[childIdx]);
				}
			}
		}
	}

}


void OctreeForCollision::insert(CollisionComponent * comp)
{
	std::stack<OctreeElem> elemStk;
	//std::stack<OctreeElem&> elemStk;
	std::stack<CollisionComponent*> compStk;

	int index = 0;
	elemStk.push(_octreeElemVec[index]);
	compStk.push(comp);
	
	while (!elemStk.empty())
	{
		OctreeElem curElem = elemStk.top();
		CollisionComponent* curComp = compStk.top();
		elemStk.pop();
		compStk.pop();

		curElem._bUsed = true;
		index = curElem._index;
		int targetIdx = -1;

		if (curElem._bUseChildren)
		{
			if (-1 == (targetIdx = getFitChildBoxIndex(curElem, comp)))
			{
				curElem._potentialComponents.push_back(curComp);
				curComp->_octreeElemIndex = index;
				return;
			}

			index = index * 8 + targetIdx;
			elemStk.push(_octreeElemVec[index]);
			compStk.push(curComp);
		}
		else
		{
			curElem._potentialComponents.push_back(comp);
			curComp->_octreeElemIndex = index;

			//_potentialComponents 개수가 적으면 stop. 일정 수가 넘어가면 children으로.
			if (_maxCountOfObjects < curElem._potentialComponents.size() && curElem._height != 0)
			{
				curElem._bUseChildren = true;

				for (auto it = curElem._potentialComponents.begin(); it != curElem._potentialComponents.end(); )
				{
					if (-1 != (targetIdx = getFitChildBoxIndex(curElem, *it)))
					{
						// push to child from this(parent)
						index = index * 8 + targetIdx;
						elemStk.push(_octreeElemVec[index]);
						compStk.push((*it));

						it = _potentialComponents.erase(it);
					}
					else
					{
						++it;
					}
				}
			}
		}
	}

	return;
}

// 충돌 가능한 모든 CollisionComponent들을 potentialList에 등록
void OctreeForCollision::getCollisionPotentialList(std::list<CollisionComponent*>& potentialList, CollisionComponent * comp)
{
	/*
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
			_childTree[i]->getAllCollisionPotentialList(potentialList);
	}
	else
	{
		_childTree[targetIdx]->getCollisionPotentialList(potentialList, comp);
	}
	*/

	// get all parent & self
	int index = comp->_octreeElemIndex;
	while (index != 0)
	{
		for (auto elem : _octreeElemVec[index]._potentialComponents)
		{
			potentialList.push_back(elem);
		}

		index = (index - 1) / 8;	// parent index
	}

	// index == 0 (root)
	for (auto elem : _octreeElemVec[0]._potentialComponents)
	{
		potentialList.push_back(elem);
	}

	// get all children
	index = comp->_octreeElemIndex;

	getAllCollisionPotentialList(potentialList, index);

	return;
}

void OctreeForCollision::getAllCollisionPotentialList(std::list<CollisionComponent*>& potentialList, int idx)
{
	if (false == _octreeElemVec[idx]._bUseChildren)
	{
		return;
	}

	for (int nextIdx = 1; nextIdx <= 8; nextIdx++)
	{
		if (_octreeElemVec[idx + nextIdx]._bUsed)
		{
			for (auto elem : _octreeElemVec[idx + nextIdx]._potentialComponents)
			{
				potentialList.push_back(elem);
				getAllCollisionPotentialList(potentialList, idx + nextIdx);
			}
		}
	}

	/*
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
	*/
}



// 등록해놓은 모든 CollisionComponent refresh.
void OctreeForCollision::clearPotentialCompPropa()
{
	std::stack<int> idxStk;
	idxStk.push(0);

	while (!idxStk.empty())
	{
		int idx = idxStk.top();
		idxStk.pop();


		if (_octreeElemVec[idx]._bUsed == false)
			return;

		_octreeElemVec[idx]._potentialComponents.clear();
		_octreeElemVec[idx]._bUsed = false;
		_maxCountOfObjects = 0;

		if (_octreeElemVec[idx]._height == 0)
		{
			return;
		}

		if (_octreeElemVec[idx]._bUseChildren)
		{
			for (int i = 1; i <= OCT_POS::OCT_NUM; i++)
			{
				idxStk.push(idx * 8 + i);
			}
		}
		_octreeElemVec[idx]._bUseChildren = false;
	}
}

// insert 가능한 child box index return
int OctreeForCollision::getFitChildBoxIndex(OctreeElem& octreeElem, CollisionComponent * comp)
{
	if (octreeElem._height == 0)
	{
		return -1;
	}
	for (int i = 0; i < OCT_POS::OCT_NUM; i++)
	{
		int index = octreeElem._index * 8 + i + 1;
		if (_octreeElemVec[index].IsInBoxFitTest(comp))
		{
			return i + 1;
		}
	}
	return -1;	// 일치하는 하위 child box가 존재하지 않음.
}

// component가 완전히 inbox인지 check. (intersect == outside)
/*
bool OctreeForCollision::IsInBoxFitTest(CollisionComponent * comp)
{
	//AABB test
	for (int i = 0; i < 3; i++)
	{
	if (fabs(_center[i] - comp->_center[i]) >(_halfAxisSize[i] + comp->_halfAxisSize[i]))
	return false;
	}


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
*/

// component - box check. (inside, intersect, outside)
/*
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
*/
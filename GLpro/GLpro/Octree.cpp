#include "stdafx.h"
#include "Octree.h"
#include "OctreeElem.h"

#include "CollisionComponent.h"
#include "CollisionComponentManager.h"

OctreeForCollision::OctreeForCollision(int height, int halfAxisSize, glm::vec3 center)
{
	/*
	_center = center;
	_halfAxisSize = halfAxisSize;
	_height = height;
	_bUsed = false;
	_bUseChildren = false;
	*/

	_octreeElemVec.reserve(pow(8, height) + 1);
	_octreeElemVec = std::vector<OctreeElem>(pow(8, height) + 1, OctreeElem());

	int index = 0;
	_octreeElemVec[index]._height = height;
	_octreeElemVec[index]._aabbOb.updateAABBObCenter(center);
	_octreeElemVec[index]._aabbOb.updateAABBObAxis(halfAxisSize);
	
	_octreeElemVec[index]._index = index;
	_octreeElemVec[index]._potentialThreshold = 8;
	
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
					int childIdx = elem._index * 8 + ( LR + TB * 2 + FB * 4 + 1 );
					if (childIdx >= _octreeElemVec.size())
						return;

					glm::vec3& halfAxisSize = elem._aabbOb.getAxisConstRef();
					float childHalfAxisSize = halfAxisSize[0] / 2;

					glm::vec3 childCenter = elem._center;
					childCenter.x += ((LR * 2) - 1) * childHalfAxisSize;	// (-1, 1) * half_half size
					childCenter.y += ((TB * 2) - 1) * childHalfAxisSize;
					childCenter.z += ((FB * 2) - 1) * childHalfAxisSize;

					_octreeElemVec[childIdx]._height = elem._height - 1;
					_octreeElemVec[childIdx]._aabbOb.updateAABBObCenter(childCenter);
					_octreeElemVec[childIdx]._aabbOb.updateAABBObAxis(childHalfAxisSize);
					_octreeElemVec[childIdx]._index = childIdx;
					_octreeElemVec[childIdx]._potentialAllCount = 0;
					_octreeElemVec[childIdx]._useChildBit = 0;
					_octreeElemVec[childIdx]._potentialThreshold = elem._potentialThreshold + elem._height + 1;

					initOctreeElem(_octreeElemVec[childIdx]);
				}
			}
		}
	}
}


void OctreeForCollision::newlyInsertComponent(CollisionComponent * comp)
{
	if (comp->isDynamicComp())
	{
		comp->updateDynamicLap();
		_usingDynamicComponents.push_back(comp);
		insertComponent(comp);
	}
	else
	{
		_usingStaticComponents.push_back(comp);
		insertComponent(comp);
	}

	return;
}

// 충돌 가능한 모든 CollisionComponent들을 potentialList에 등록
void OctreeForCollision::getCollisionPotentialList(std::list<CollisionComponent*>& potentialList, CollisionComponent * comp)
{
	// get all parent & self
	int index = comp->getOctreeElemIndex();
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
	index = comp->getOctreeElemIndex();

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

void OctreeForCollision::resetAllCollisionCompDirty()
{
	for (auto elem : _usingStaticComponents)
	{
		elem->resetCollisionComponentDirty();
	}

	for (auto elem : _usingDynamicComponents)
	{
		elem->resetCollisionComponentDirty();
	}
}

void OctreeForCollision::doOctreeUpdate()
{
	// static
	for (auto it = _usingStaticComponents.begin(); it != _usingStaticComponents.end();)
	{
		if ((*it)->_bDeleted)
		{
			// remove from octree
			removeCopmInOctreeElem(*it);

			// delete from usingComponent and DeAlloc
			delete *it;
			it = _usingStaticComponents.erase(it);
			continue;
		}

		if (!((*it)->_bCollisionTest))
		{
			// move from usingComponent to sleepCompContainer
			GCollisionComponentManager->pushToSleepComponentContainer(*it);

			// remove from octree
			removeCopmInOctreeElem(*it);

			// remove from usingComponent
			it = _usingStaticComponents.erase(it);
			continue;
		}

		(*it)->_bAlreadyVelocityUpdated = false;	// init for velocity update
		(*it)->updateCollisionComp();				// update local collision box -> world collision box

		// check aabb is modified
		if ((*it)->isCollisionComponentDirty())
		{
			return;
		}

		// check is in same block
		if (_octreeElemVec[(*it)->getOctreeElemIndex()].IsInBoxFitTest((*it)))
		{
			// if using children && not leaf node, down 1 height
			/*	opt
			if(use this elem children)
			check IsInBoxTest of this direct child box
			*/
			return;
			
		}
		else
		{
			// If not, Reinsert to root
			removeCopmInOctreeElem(*it);
			insertComponent(*it);
		}
	}

	// dynamic
	for (auto it = _usingDynamicComponents.begin(); it != _usingDynamicComponents.end();)
	{
		if ((*it)->_bDeleted)
		{
			// remove from octree
			removeCopmInOctreeElem(*it);

			// delete from usingComponent and DeAlloc
			delete *it;
			it = _usingDynamicComponents.erase(it);
			continue;
		}

		if (!((*it)->_bCollisionTest))
		{
			// move from usingComponent to sleepCompContainer
			GCollisionComponentManager->pushToSleepComponentContainer(*it);

			// remove from octree
			removeCopmInOctreeElem(*it);

			// remove from usingComponent
			it = _usingStaticComponents.erase(it);
			continue;
		}

		(*it)->_bAlreadyVelocityUpdated = false;	// init for velocity update
		(*it)->updateCollisionComp();				// update local collision box -> world collision box
		(*it)->updateDynamicLap();					// Dynamic lap update
													// check aabb is modified
		if ((*it)->isCollisionComponentDirty())
		{
			return;
		}

		// check is in same block
		if (_octreeElemVec[(*it)->getOctreeElemIndex()].IsInBoxFitTest((*it)))
		{
			// if using children && not leaf node, down 1 height
			/*	opt
			if(use this elem children)
			check IsInBoxTest of this direct child box
			*/
			return;
		}
		else
		{
			// If not, Reinsert to root
			removeCopmInOctreeElem(*it);
			insertComponent(*it);
		}
	}
}

void OctreeForCollision::insertComponent(CollisionComponent * comp)
{
	int index = 0;
	OctreeElem& curElem = _octreeElemVec[index];

	while (true)
	{
		curElem._bUsed = true;
		int targetChildIdx = -1;

		if (curElem._bUseChildren)
		{
			if (-1 == (targetChildIdx = getFitChildBoxIndex(curElem, comp)))
			{
				// no match with children
				curElem._potentialComponents.push_back(comp);
				comp->setOctreeElemIdx(index);
				return;
			}

			index = index * 8 + targetChildIdx;
			curElem = _octreeElemVec[index];
		}
		else
		{
			curElem._potentialComponents.push_back(comp);
			comp->setOctreeElemIdx(index);

			//_potentialComponents 개수가 적으면 stop. 일정 수가 넘어가면 children으로.
			if (curElem._potentialThreshold < curElem._potentialComponents.vecPSize() && curElem._height != 0)
			{
				curElem._bUseChildren = true;

				for (auto it = curElem._potentialComponents.begin(); it != curElem._potentialComponents.end(); )
				{
					if (-1 != (targetChildIdx = getFitChildBoxIndex(curElem, *it)))
					{
						// push to child from this(parent)
						CollisionComponent* moveComp = (*it);
						it = curElem._potentialComponents.erase(it);

						int childIndex = index * 8 + targetChildIdx;
						OctreeElem& childOctreeElem = _octreeElemVec[childIndex];

						childOctreeElem._bUsed = true;
						childOctreeElem._potentialComponents.push_back(moveComp);
						(moveComp)->setOctreeElemIdx(childIndex);
					}
					else
					{
						++it;
					}
				}
			}

			return;
		}
	}

	return;
}

int OctreeForCollision::getFitOctreeElemIndex(CollisionComponent * comp)
{
	int index = 0;
	OctreeElem& curElem = _octreeElemVec[index];

	while (true)
	{
		curElem._bUsed = true;
		int targetChildIdx = -1;

		if (curElem._bUseChildren)
		{
			if (-1 == (targetChildIdx = getFitChildBoxIndex(curElem, comp)))
			{
				// no match with children
				curElem._potentialComponents.push_back(comp);
				comp->setOctreeElemIdx(index);
				return;
			}

			index = index * 8 + targetChildIdx;
			curElem = _octreeElemVec[index];
		}
		else
		{
			curElem._potentialComponents.push_back(comp);
			comp->setOctreeElemIdx(index);

			//_potentialComponents 개수가 적으면 stop. 일정 수가 넘어가면 children으로.
			if (curElem._potentialThreshold < curElem._potentialComponents.vecPSize() && curElem._height != 0)
			{
				curElem._bUseChildren = true;

				for (auto it = curElem._potentialComponents.begin(); it != curElem._potentialComponents.end(); )
				{
					if (-1 != (targetChildIdx = getFitChildBoxIndex(curElem, *it)))
					{
						// push to child from this(parent)
						CollisionComponent* moveComp = (*it);
						it = curElem._potentialComponents.erase(it);

						int childIndex = index * 8 + targetChildIdx;
						OctreeElem& childOctreeElem = _octreeElemVec[childIndex];

						childOctreeElem._bUsed = true;
						childOctreeElem._potentialComponents.push_back(moveComp);
						(moveComp)->setOctreeElemIdx(childIndex);
					}
					else
					{
						++it;
					}
				}
			}

			return;
		}
	}

	return;
}

// insert 가능한 child box index return
int OctreeForCollision::getFitChildBoxIndex(OctreeElem& octreeElem, CollisionComponent * comp)
{
	if (octreeElem._height == 0)
	{
		return -1;
	}

	int spaceIndex = octreeElem.getSpaceOfMatchedCenter(comp);
	int index = octreeElem._index * 8 + spaceIndex + 1;

	if (_octreeElemVec[index].IsInBoxFitTest(comp))
	{
		return spaceIndex + 1;
	}

	/*
	for (int i = 0; i < OCT_POS::OCT_NUM; i++)
	{
		int index = octreeElem._index * 8 + i + 1;
		if (_octreeElemVec[index].IsInBoxFitTest(comp))
		{
			return i + 1;
		}
	}
	*/

	return -1;	// 일치하는 하위 child box가 존재하지 않음.
}

//bool IsInBoxFitTest(CollisionComponent* comp);
//int IsInBoxTestAll(CollisionComponent* comp);

void OctreeForCollision::removeCopmInOctreeElem(CollisionComponent * comp)
{
	int index = comp->getOctreeElemIndex();
	OctreeElem& octreeElem = _octreeElemVec[index];
	octreeElem._potentialComponents.erase(comp);

	// reduce count all parent

}

bool OctreeForCollision::IsUseThisOctreeElem(OctreeElem & elem)
{
	// parent _useChildBit check
	int idx = elem._index;
	int mod = elem._index % 8;

	if (idx == 0)
	{
		return true;
	}

	idx = (idx - 1) / 8;	// parent index

	if (_octreeElemVec[idx]._useChildBit & (1 << mod))
	{
		return true;
	}

	return false;
}


bool OctreeForCollision::IsUseChild(OctreeElem & elem)
{
	return elem._useChildBit != 0;
}

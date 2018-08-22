#include "stdafx.h"
#include "Octree.h"
#include "OctreeElem.h"

#include "CollisionComponent.h"
#include "CollisionComponentManager.h"
#include "DynamicCollisionSubComp.h"

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

					const glm::vec3& halfAxisSize = elem._aabbOb.getAxisConstRef();
					float childHalfAxisSize = halfAxisSize[0] / 2;

					glm::vec3 childCenter = elem._aabbOb.getCenterConstRef();
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
		comp->getDynamicSubComp()->resetCollisionOccured();	// init for velocity update
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

// octree collision test
void OctreeForCollision::doCollisionTest()
{
	// collision test
	innerDoCollisionTest(_octreeElemVec[0], std::vector<VectorP<CollisionComponent*>&>(), std::vector<VectorP<CollisionComponent*>&>());

	// Test Dynamic-Dynamic & Dynamic-Static :: ddAndDsCOllsionTime
	ddAndDsCOllsionResolve();
}

// 충돌 가능한 모든 CollisionComponent들을 potentialList에 등록
void OctreeForCollision::getCollisionPotentialList(std::list<CollisionComponent*>& potentialList, CollisionComponent * comp)
{
	// get all parent & self
	int index = comp->getOctreeElemIndex();
	while (index != 0)
	{
		for (auto elem : _octreeElemVec[index]._potentialStaticComponents)
		{
			potentialList.push_back(elem);
		}

		for (auto elem : _octreeElemVec[index]._potentialDynamicComponents)
		{
			potentialList.push_back(elem);
		}

		index = (index - 1) / 8;	// parent index
	}

	// index == 0 (root)
	for (auto elem : _octreeElemVec[0]._potentialStaticComponents)
	{
		potentialList.push_back(elem);
	}

	for (auto elem : _octreeElemVec[0]._potentialDynamicComponents)
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
			for (auto elem : _octreeElemVec[idx + nextIdx]._potentialStaticComponents)
			{
				potentialList.push_back(elem);
				getAllCollisionPotentialList(potentialList, idx + nextIdx);
			}

			for (auto elem : _octreeElemVec[idx + nextIdx]._potentialDynamicComponents)
			{
				potentialList.push_back(elem);
				getAllCollisionPotentialList(potentialList, idx + nextIdx);
			}
		}
	}
}

void OctreeForCollision::innerDoCollisionTest(OctreeElem& octreeElem, std::vector<VectorP<CollisionComponent*>&> staticVecAcc, std::vector<VectorP<CollisionComponent*>&> dynamicVecAcc)
{
	if (!octreeElem._bUsed)
		return;

	// accDynamic vs curDynamic
	for (auto& accVector : dynamicVecAcc)
	{
		dynamicDynamicCollisionVecTest(accVector, octreeElem._potentialDynamicComponents);
	}
	// accStatic vs curDynamic
	for (auto& accVector : staticVecAcc)
	{
		staticDynamicCollisionVecTest(accVector, octreeElem._potentialDynamicComponents);
	}

	// accDynamic vs curStatic
	for (auto& accVector : dynamicVecAcc)
	{
		staticDynamicCollisionVecTest(octreeElem._potentialStaticComponents, accVector);
	}
	// accStatic vs curStatic
	for (auto& accVector : staticVecAcc)
	{
		staticStaticCollisionVecTest(accVector, octreeElem._potentialStaticComponents);
	}

	// curDynamic vs curStatic
	staticDynamicCollisionVecTest(octreeElem._potentialStaticComponents, octreeElem._potentialStaticComponents);

	// curStatic vs curStatic
	staticSelfCollisionVecTest(octreeElem._potentialStaticComponents);

	// curDynamic vs curDynamic
	dynamicSelfCollisionVecTest(octreeElem._potentialDynamicComponents);

	if (!octreeElem._bUseChildren)
	{
		return;
	}

	staticVecAcc.push_back(octreeElem._potentialStaticComponents);
	dynamicVecAcc.push_back(octreeElem._potentialDynamicComponents);

	int index = octreeElem._index;
	for (int i = 0; i < OCT_NUM; i++)
	{
		// propagete copied compVec's vector
		innerDoCollisionTest(_octreeElemVec[index * 8 + i + 1], staticVecAcc, dynamicVecAcc);
	}

	return;
}

void OctreeForCollision::staticStaticCollisionVecTest(VectorP<CollisionComponent*>& staticCompVec1, VectorP<CollisionComponent*>& staticCompVec2)
{
	for (auto elem1 : staticCompVec1)
	{
		for (auto elem2 : staticCompVec2)
		{
			staticStaticCollisionTest(elem1, elem2);
		}
	}
}

void OctreeForCollision::staticDynamicCollisionVecTest(VectorP<CollisionComponent*>& staticCompVec, VectorP<CollisionComponent*>& dynamicCompVec)
{
	for (auto staticElem : staticCompVec)
	{
		for (auto dynamicElem : dynamicCompVec)
		{
			staticDynamicCollisionTest(staticElem, dynamicElem);
		}
	}
}

void OctreeForCollision::dynamicDynamicCollisionVecTest(VectorP<CollisionComponent*>& dycompVec1, VectorP<CollisionComponent*>& dycompVec2)
{
	for (auto elem1 : dycompVec1)
	{
		for (auto elem2 : dycompVec2)
		{
			dynamicDynamicCollisionTest(elem1, elem2);
		}
	}
}

void OctreeForCollision::staticSelfCollisionVecTest(VectorP<CollisionComponent*>& staticCompVec)
{
	std::vector<CollisionComponent*>& compVecRef = staticCompVec.getElemVecRef();
	
	for (int i = 0; i < compVecRef.size() - 1; i++)
	{
		for (int k = i + 1; k < compVecRef.size(); k++)
		{
			staticStaticCollisionTest(compVecRef[i], compVecRef[k]);
		}
	}
}

void OctreeForCollision::dynamicSelfCollisionVecTest(VectorP<CollisionComponent*>& dynamicCompVec)
{
	std::vector<CollisionComponent*>& compVecRef = dynamicCompVec.getElemVecRef();

	for (int i = 0; i < compVecRef.size() - 1; i++)
	{
		for (int k = i + 1; k < compVecRef.size(); k++)
		{
			dynamicDynamicCollisionTest(compVecRef[i], compVecRef[k]);
		}
	}
}

void OctreeForCollision::staticStaticCollisionTest(CollisionComponent * staticComp1, CollisionComponent * staticComp2)
{
	// self test
	//if (elem == potentialComp)
	//continue;

	// category test
	bool categoryTest1 = staticComp1->testCollisionCategoryBit(staticComp2);
	bool categoryTest2 = staticComp2->testCollisionCategoryBit(staticComp1);
	if (!categoryTest1 || !categoryTest1)
		return;

	// aabb lap test
	if (false == staticComp1->lapStaticTestToOtherStatic(staticComp2))
		return;

	// actual shape test
	if (false == staticComp1->collideStaticTestToOtherStatic(staticComp2))
		return;
	
	// save collision comp list to comp for logic
	staticComp1->_collisionLogList.push_back(staticComp2);
	staticComp2->_collisionLogList.push_back(staticComp1);

	// velocity test
	bool isTrigger1 = staticComp1->isTrigger();
	bool isTrigger2 = staticComp2->isTrigger();
	if (isTrigger1 || isTrigger2)
		return;

	// modify velocity (탄성 충돌)
	/*
	elem->_bAlreadyVelocityUpdated = true;
	potentialComp->_bAlreadyVelocityUpdated = true;

	glm::vec3& v1 = elem->_rigidComp->getVelocityRef();
	glm::vec3& v2 = potentialComp->_rigidComp->getVelocityRef();
	float m1 = elem->_rigidComp->getMass();
	float m2 = potentialComp->_rigidComp->getMass();

	glm::vec3 v1Ret = v1 + m2*(v2 - v1) * 2.0f / (m1 + m2);
	glm::vec3 v2Ret = v2 + m2*(v1 - v2) * 2.0f / (m1 + m2);
	*/

	staticComp1->resolveStaticStaticCollide(staticComp2);
	
	return;
}

void OctreeForCollision::staticDynamicCollisionTest(CollisionComponent * staticComp, CollisionComponent * dynamicComp)
{
	// category test
	bool categoryTest1 = staticComp->testCollisionCategoryBit(dynamicComp);
	bool categoryTest2 = dynamicComp->testCollisionCategoryBit(staticComp);
	if (!categoryTest1 || !categoryTest1)
		return;

	// aabb lap test
	if (false == staticComp->lapStaticTestToOtherDynamic(dynamicComp))
		return;

	// actual shape test
	float collideTime;
	if (false == staticComp->collideStaticTestToOtherDynamic(dynamicComp, collideTime))
		return;

	ddAndDsCOllsionTime.push_back(std::make_pair(collideTime, std::make_pair(dynamicComp, staticComp)));

	return;
}

void OctreeForCollision::dynamicDynamicCollisionTest(CollisionComponent * dycomp1, CollisionComponent * dycomp2)
{
	// category test
	bool categoryTest1 = dycomp1->testCollisionCategoryBit(dycomp2);
	bool categoryTest2 = dycomp2->testCollisionCategoryBit(dycomp1);
	if (!categoryTest1 || !categoryTest1)
		return;

	// aabb lap test
	if (false == dycomp1->lapDynamicTestToOtherDynamic(dycomp2))
		return;

	// actual shape test
	float collideTime;
	if (false == dycomp1->collideDynamicTestToOtherDynamic(dycomp2, collideTime))
		return;

	ddAndDsCOllsionTime.push_back(std::make_pair(collideTime, std::make_pair(dycomp1, dycomp2)));
	return;
}

void OctreeForCollision::ddAndDsCOllsionResolve()
{
	// Test Dynamic-Dynamic & Dynamic-Static :: ddAndDsCOllsionTime
	std::sort(ddAndDsCOllsionTime.begin(), ddAndDsCOllsionTime.end(), std::less<float>());

	for (auto elem : ddAndDsCOllsionTime)
	{
		float collideTime = elem.first;
		CollisionComponent* dynamicComp = elem.second.first;
		CollisionComponent* otherComp = elem.second.second;
		
		// check velocity already modified by prevous collision
		bool alreadyDCollisionOccured1 = dynamicComp->getDynamicSubComp()->getCollisionOccured();
		bool alreadyDCollisionOccured2 = (otherComp->isDynamicComp() ?  otherComp->getDynamicSubComp()->getCollisionOccured() : false);
		if (alreadyDCollisionOccured1 || alreadyDCollisionOccured2)
			continue;

		// save collision comp list to comp for logic
		dynamicComp->_collisionLogList.push_back(otherComp);
		otherComp->_collisionLogList.push_back(dynamicComp);

		// velocity test
		bool isTrigger1 = dynamicComp->isTrigger();
		bool isTrigger2 = otherComp->isTrigger();
		if (isTrigger1 || isTrigger2)
		{
			continue;
		}

		// set dynamic occured
		dynamicComp->getDynamicSubComp()->setCollisoinOccured();
		otherComp->isDynamicComp() ? otherComp->getDynamicSubComp()->setCollisoinOccured() : false;

		// modify velocity
		if (otherComp->isDynamicComp())
		{
			dynamicComp->resolveDynamicDynamicCollide(collideTime, otherComp);
		}
		else
		{
			dynamicComp->resolveDynamicStaticCollide(collideTime, otherComp);
		}
	}

	//todo : if no dynamic time collision, copy result(post) to prev
	for (auto elem : _usingDynamicComponents)
	{
		if (elem->getDynamicSubComp()->getCollisionOccured())
		{
			elem->getDynamicSubComp()->saveNonCollideInfoToPrevInfo();
		}
	}

	ddAndDsCOllsionTime.clear();
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

		_octreeElemVec[idx]._potentialStaticComponents.clear();
		_octreeElemVec[idx]._potentialDynamicComponents.clear();
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
			removeStaticCopmInOctreeElem(*it);

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
			removeStaticCopmInOctreeElem(*it);

			// remove from usingComponent
			it = _usingStaticComponents.erase(it);
			continue;
		}

		(*it)->updateCollisionComp();				// update local collision box -> world collision box

		// check aabb is modified
		if ((*it)->isCollisionComponentDirty())
		{
			return;
		}

		// check is in same block
		if (_octreeElemVec[(*it)->getOctreeElemIndex()].IsInBoxFitTestStaticComp((*it)))
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
			removeStaticCopmInOctreeElem(*it);
			insertStaticComponent(*it);
		}
	}

	// dynamic
	for (auto it = _usingDynamicComponents.begin(); it != _usingDynamicComponents.end();)
	{
		if ((*it)->_bDeleted)
		{
			// remove from octree
			removeDynamicCopmInOctreeElem(*it);

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
			removeDynamicCopmInOctreeElem(*it);

			// remove from usingComponent
			it = _usingDynamicComponents.erase(it);
			continue;
		}

		(*it)->updateCollisionComp();				// update local collision box -> world collision box
		(*it)->updateDynamicLap();					// Dynamic lap update
		(*it)->getDynamicSubComp()->resetCollisionOccured();	// init for velocity update
														// check aabb is modified
		if ((*it)->isCollisionComponentDirty())
		{
			return;
		}

		// check is in same block
		if (_octreeElemVec[(*it)->getOctreeElemIndex()].IsInBoxFitTestDynamicComp((*it)))
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
			removeDynamicCopmInOctreeElem(*it);
			insertDynamicComponent(*it);
		}
	}
}

void OctreeForCollision::insertComponent(CollisionComponent * comp)
{
	if (comp->isDynamicComp())
		insertDynamicComponent(comp);
	else
		insertStaticComponent(comp);
}

void OctreeForCollision::insertStaticComponent(CollisionComponent * comp)
{
	int index = 0;
	OctreeElem& curElem = _octreeElemVec[index];

	while (true)
	{
		curElem._bUsed = true;
		int targetChildIdx = -1;

		if (curElem._bUseChildren)
		{
			if (-1 == (targetChildIdx = getFitChildBoxIndexStaticComp(curElem, comp)))
			{
				// no match with children
				curElem._potentialStaticComponents.push_back(comp);
				comp->setOctreeElemIdx(index);
				return;
			}

			index = index * 8 + targetChildIdx;
			curElem = _octreeElemVec[index];
		}
		else
		{
			curElem._potentialStaticComponents.push_back(comp);
			comp->setOctreeElemIdx(index);

			//_potentialComponents 개수가 적으면 stop. 일정 수가 넘어가면 children으로.
			if (curElem._potentialThreshold < curElem._potentialStaticComponents.vecPSize() + curElem._potentialDynamicComponents.vecPSize() && curElem._height != 0)
			{
				propageteComponentsToChildren(curElem);
			}

			return;
		}
	}

	return;
}

void OctreeForCollision::insertDynamicComponent(CollisionComponent * comp)
{
	int index = 0;
	OctreeElem& curElem = _octreeElemVec[index];

	while (true)
	{
		curElem._bUsed = true;
		int targetChildIdx = -1;

		if (curElem._bUseChildren)
		{
			if (-1 == (targetChildIdx = getFitChildBoxIndexDynamicComp(curElem, comp)))
			{
				// no match with children
				curElem._potentialDynamicComponents.push_back(comp);
				comp->setOctreeElemIdx(index);
				return;
			}

			index = index * 8 + targetChildIdx;
			curElem = _octreeElemVec[index];
		}
		else
		{
			curElem._potentialDynamicComponents.push_back(comp);
			comp->setOctreeElemIdx(index);

			//_potentialComponents 개수가 적으면 stop. 일정 수가 넘어가면 children으로.
			if (curElem._potentialThreshold < curElem._potentialStaticComponents.vecPSize() + curElem._potentialDynamicComponents.vecPSize() && curElem._height != 0)
			{
				propageteComponentsToChildren(curElem);
			}

			return;
		}
	}

	return;
}

void OctreeForCollision::propageteComponentsToChildren(OctreeElem & octreeElem)
{
	int targetChildIdx;
	int index = octreeElem._index;
	octreeElem._bUseChildren = true;

	// static
	for (auto it = octreeElem._potentialStaticComponents.begin(); it != octreeElem._potentialStaticComponents.end(); )
	{
		if (-1 != (targetChildIdx = getFitChildBoxIndexStaticComp(octreeElem, *it)))
		{
			// push to child from this(parent)
			CollisionComponent* moveComp = (*it);
			it = octreeElem._potentialStaticComponents.erase(it);

			int childIndex = index * 8 + targetChildIdx;
			OctreeElem& childOctreeElem = _octreeElemVec[childIndex];

			childOctreeElem._bUsed = true;
			childOctreeElem._potentialStaticComponents.push_back(moveComp);
			(moveComp)->setOctreeElemIdx(childIndex);
		}
		else
		{
			++it;
		}
	}

	// dynamic
	for (auto it = octreeElem._potentialDynamicComponents.begin(); it != octreeElem._potentialDynamicComponents.end(); )
	{
		if (-1 != (targetChildIdx = getFitChildBoxIndexDynamicComp(octreeElem, *it)))
		{
			// push to child from this(parent)
			CollisionComponent* moveComp = (*it);
			it = octreeElem._potentialDynamicComponents.erase(it);

			int childIndex = index * 8 + targetChildIdx;
			OctreeElem& childOctreeElem = _octreeElemVec[childIndex];

			childOctreeElem._bUsed = true;
			childOctreeElem._potentialDynamicComponents.push_back(moveComp);
			(moveComp)->setOctreeElemIdx(childIndex);
		}
		else
		{
			++it;
		}
	}
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

	return -1;	// 일치하는 하위 child box가 존재하지 않음.
}

int OctreeForCollision::getFitChildBoxIndexStaticComp(OctreeElem & octreeElem, CollisionComponent * comp)
{
	if (octreeElem._height == 0)
	{
		return -1;
	}

	int spaceIndex = octreeElem.getSpaceOfMatchedCenterStaticComp(comp);
	int index = octreeElem._index * 8 + spaceIndex + 1;

	if (_octreeElemVec[index].IsInBoxFitTestStaticComp(comp))
	{
		return spaceIndex + 1;
	}

	return -1;	// 일치하는 하위 child box가 존재하지 않음.
}

int OctreeForCollision::getFitChildBoxIndexDynamicComp(OctreeElem & octreeElem, CollisionComponent * comp)
{
	if (octreeElem._height == 0)
	{
		return -1;
	}

	int spaceIndex = octreeElem.getSpaceOfMatchedCenterDynamicComp(comp);
	int index = octreeElem._index * 8 + spaceIndex + 1;

	if (_octreeElemVec[index].IsInBoxFitTestDynamicComp(comp))
	{
		return spaceIndex + 1;
	}

	return -1;	// 일치하는 하위 child box가 존재하지 않음.
}

//bool IsInBoxFitTest(CollisionComponent* comp);
//int IsInBoxTestAll(CollisionComponent* comp);

void OctreeForCollision::removeCopmInOctreeElem(CollisionComponent * comp)
{
	if (comp->isDynamicComp())
		removeDynamicCopmInOctreeElem(comp);
	else
		removeStaticCopmInOctreeElem(comp);
	// reduce count all parent
}

void OctreeForCollision::removeStaticCopmInOctreeElem(CollisionComponent * comp)
{
	int index = comp->getOctreeElemIndex();
	OctreeElem& octreeElem = _octreeElemVec[index];
	octreeElem._potentialStaticComponents.erase(comp);

	// reduce count all parent
}

void OctreeForCollision::removeDynamicCopmInOctreeElem(CollisionComponent * comp)
{
	int index = comp->getOctreeElemIndex();
	OctreeElem& octreeElem = _octreeElemVec[index];
	octreeElem._potentialDynamicComponents.erase(comp);

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

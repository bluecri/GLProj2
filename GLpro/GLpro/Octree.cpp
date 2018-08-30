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
	int powInt = 1;
	int retSize = 1;
	for (int i = 0; i < height; i++)
	{
		powInt *= 8;
		retSize += powInt;
	}

	_octreeElemVec.reserve(retSize + 1);
	_octreeElemVec = std::vector<OctreeElem>(retSize + 1, OctreeElem());

	int index = 0;
	_octreeElemVec[index]._height = height;
	_octreeElemVec[index]._aabbOb.updateAABBObCenter(center);
	_octreeElemVec[index]._aabbOb.updateAABBObAxis((float)halfAxisSize);
	
	_octreeElemVec[index].setIndex(index);
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
					int childIdx = elem.getIndex() * 8 + ( LR + TB * 2 + FB * 4 + 1 );
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
					_octreeElemVec[childIdx].setIndex(childIdx);
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
	tbb::parallel_for(tbb::blocked_range<size_t>(0, _octreeElemVec.size()), CTBB_staticCollideTest_EXXX(this));
	
	//innerDoCollisionTest(_octreeElemVec[0], std::vector<VectorP<CollisionComponent*>*>(), std::vector<VectorP<CollisionComponent*>*>());
	//this->TESTTEST(this, 0, std::vector<VectorP<CollisionComponent*>*>(), std::vector<VectorP<CollisionComponent*>*>());
	//tbb::parallel_for(tbb::blocked_range<size_t>(0, 1), CTBB_staticCollideTest(this, 0, std::vector<VectorP<CollisionComponent*>*>(), std::vector<VectorP<CollisionComponent*>*>()));
	//tbb::parallel_for(tbb::blocked_range<size_t>(0, 8), CTBB_staticCollideTest_EX(this, 0, std::vector<VectorP<CollisionComponent*>*>(), std::vector<VectorP<CollisionComponent*>*>()));
	
	// Test Dynamic-Dynamic & Dynamic-Static :: ddAndDsCOllsionTime
	ssCollsionResolve();
	//ddAndDsCOllsionResolve();
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

void OctreeForCollision::innerDoCollisionTest(OctreeElem& octreeElem, std::vector<VectorP<CollisionComponent*>*> staticVecAcc, std::vector<VectorP<CollisionComponent*>*> dynamicVecAcc)
{
	if (!octreeElem._bUsed)
		return;

	// accDynamic vs curDynamic
	for (auto accVector : dynamicVecAcc)
	{
		dynamicDynamicCollisionVecTest(*accVector, octreeElem._potentialDynamicComponents);
	}
	// accStatic vs curDynamic
	for (auto accVector : staticVecAcc)
	{
		staticDynamicCollisionVecTest(*accVector, octreeElem._potentialDynamicComponents);
	}

	// accDynamic vs curStatic
	for (auto accVector : dynamicVecAcc)
	{
		staticDynamicCollisionVecTest(octreeElem._potentialStaticComponents, *accVector);
	}
	// accStatic vs curStatic
	for (auto accVector : staticVecAcc)
	{
		staticStaticCollisionVecTest(*accVector, octreeElem._potentialStaticComponents);
	}

	// curDynamic vs curStatic
	staticDynamicCollisionVecTest(octreeElem._potentialStaticComponents, octreeElem._potentialDynamicComponents);

	// curStatic vs curStatic
	staticSelfCollisionVecTest(octreeElem._potentialStaticComponents);

	// curDynamic vs curDynamic
	dynamicSelfCollisionVecTest(octreeElem._potentialDynamicComponents);

	if (!octreeElem._bUseChildren)
	{
		return;
	}

	staticVecAcc.push_back(&(octreeElem._potentialStaticComponents));
	dynamicVecAcc.push_back(&(octreeElem._potentialDynamicComponents));

	int index = octreeElem.getIndex();
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
	int vecSize = compVecRef.size();
	for (int i = 0; i < vecSize; i++)
	{
		for (int k = i + 1; k < vecSize; k++)
		{
			staticStaticCollisionTest(compVecRef[i], compVecRef[k]);
		}
	}
}

void OctreeForCollision::dynamicSelfCollisionVecTest(VectorP<CollisionComponent*>& dynamicCompVec)
{
	std::vector<CollisionComponent*>& compVecRef = dynamicCompVec.getElemVecRef();

	int vecSize = compVecRef.size();

	for (int i = 0; i < vecSize - 1; i++)
	{
		for (int k = i + 1; k < vecSize; k++)
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
	//staticComp1->_tbbRWLock.try_lock_read();
	//staticComp2->_tbbRWLock.try_lock_read();

	// category test
	bool categoryTest1 = staticComp1->testCollisionCategoryBit(staticComp2);
	bool categoryTest2 = staticComp2->testCollisionCategoryBit(staticComp1);
	if (!categoryTest1 || !categoryTest1)
	{
		//staticComp1->_tbbRWLock.unlock();
		//staticComp2->_tbbRWLock.unlock();
		return;
	}

	// aabb lap test
	if (false == staticComp1->lapStaticTestToOtherStatic(staticComp2))
	{
		//staticComp1->_tbbRWLock.unlock();
		//staticComp2->_tbbRWLock.unlock();
		return;
	}

	// actual shape test
	if (false == staticComp1->collideStaticTestToOtherStatic(staticComp2))
	{
		//staticComp1->_tbbRWLock.unlock();
		//staticComp2->_tbbRWLock.unlock();
		return;
	}

	//staticComp1->_tbbRWLock.unlock();
	//staticComp2->_tbbRWLock.unlock();

	tbb::reader_writer_lock::scoped_lock(staticComp1->_tbbRWLock);
	tbb::reader_writer_lock::scoped_lock(staticComp2->_tbbRWLock);

	// save collision comp list to comp for logic
	staticComp1->_collisionLogList.push_back(staticComp2);
	staticComp2->_collisionLogList.push_back(staticComp1);

	// velocity test
	bool isTrigger1 = staticComp1->isTrigger();
	bool isTrigger2 = staticComp2->isTrigger();
	if (isTrigger1 || isTrigger2)
		return;

	ssCollsionCvec.push_back(std::make_pair(staticComp1, staticComp2));
	//staticComp1->resolveStaticStaticCollide(staticComp2);
	
	return;
}

void OctreeForCollision::staticDynamicCollisionTest(CollisionComponent * staticComp, CollisionComponent * dynamicComp)
{
	tbb::reader_writer_lock::scoped_lock_read(staticComp->_tbbRWLock);
	tbb::reader_writer_lock::scoped_lock_read(dynamicComp->_tbbRWLock);

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
	tbb::reader_writer_lock::scoped_lock_read(dycomp1->_tbbRWLock);
	tbb::reader_writer_lock::scoped_lock_read(dycomp2->_tbbRWLock);

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

void OctreeForCollision::ssCollsionResolve()
{
	for (auto elem : ssCollsionCvec)
	{
		elem.first->resolveStaticStaticCollide(elem.second);
	}
}


void OctreeForCollision::ddAndDsCOllsionResolve()
{
	// Test Dynamic-Dynamic & Dynamic-Static :: ddAndDsCOllsionTime
	std::sort(ddAndDsCOllsionTime.begin(), ddAndDsCOllsionTime.end());

	// check comps that should not be updated ( first time --> last time )

	// update no need lock.(only 1 update)

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

	// if no dynamic time collision, copy result(post) to prev
	for (auto elem : _usingDynamicComponents)
	{
		if (!elem->getDynamicSubComp()->getCollisionOccured())
		{
			elem->saveDynamicRetCollideInfoToPrevInfo();
		}

		elem->getDynamicSubComp()->resetCollisionOccured();	// init for velocity update
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
	tbb::parallel_do(_usingStaticComponents.begin(), _usingStaticComponents.end(), CTBB_DirtyBItInit_do());
	tbb::parallel_do(_usingDynamicComponents.begin(), _usingDynamicComponents.end(), CTBB_DirtyBItInit_do());
	
	/*
	for (auto elem : _usingStaticComponents)
	{
		elem->resetCollisionComponentDirty();
	}

	for (auto elem : _usingDynamicComponents)
	{
		elem->resetCollisionComponentDirty();
	}
	*/
}

void OctreeForCollision::doOctreeUpdate()
{
	//_pushToRootCompsCvec;
	//_pushToSelfBoxCompsCvec;

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
		++it;
	}
	
	tbb::parallel_do(_usingStaticComponents.begin(), _usingStaticComponents.end(), CTBB_UpdateStaticCollision_do(*this));

	for(auto elem : _pushToSelfBoxCompsCvec)
		trySplitStaticComp(_octreeElemVec[elem->getOctreeElemIndex()], elem);

	for (auto elem : _pushToRootCompsCvec)
	{
		removeStaticCopmInOctreeElem(elem);
		insertStaticComponent(elem, _octreeElemVec[0]);
	}

	_pushToRootCompsCvec.clear();
	_pushToSelfBoxCompsCvec.clear();

	/*
		(*it)->updateCollisionComp();				// update local collision box -> world collision box

		// check aabb is modified
		if ((*it)->isCollisionComponentDirty())
		{
			++it;
			continue;
		}

		// check is in same block
		if (_octreeElemVec[(*it)->getOctreeElemIndex()].IsInBoxFitTestStaticComp((*it)))
		{
			// if using children && not leaf node, down 1 height
			
			// opt	if(use this elem children)
			//		check IsInBoxTest of this direct child box
			trySplitStaticComp(_octreeElemVec[(*it)->getOctreeElemIndex()], (*it));
			++it;
			continue;
		}
		else
		{
			// If not, Reinsert to root
			removeStaticCopmInOctreeElem(*it);
			insertStaticComponent(*it, _octreeElemVec[0]);
			++it;
		}
		
	}
	*/

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
		++it;
	}

	tbb::parallel_do(_usingDynamicComponents.begin(), _usingDynamicComponents.end(), CTBB_UpdateDynamicCollision_do(*this));

	for (auto elem : _pushToSelfBoxCompsCvec)
		trySplitDynamicComp(_octreeElemVec[elem->getOctreeElemIndex()], elem);

	for (auto elem : _pushToRootCompsCvec)
	{
		removeDynamicCopmInOctreeElem(elem);
		insertDynamicComponent(elem, _octreeElemVec[0]);
	}

	_pushToRootCompsCvec.clear();
	_pushToSelfBoxCompsCvec.clear();

		/*
		(*it)->updateCollisionComp();				// update local collision box -> world collision box
		(*it)->updateDynamicLap();					// Dynamic lap update
														// check aabb is modified
		if ((*it)->isCollisionComponentDirty())
		{
			++it;
			continue;
		}

		// check is in same block
		if (_octreeElemVec[(*it)->getOctreeElemIndex()].IsInBoxFitTestDynamicComp((*it)))
		{
			// if using children && not leaf node, down 1 height
			trySplitDynamicComp(_octreeElemVec[(*it)->getOctreeElemIndex()], (*it));

			++it;
			continue;
		}
		else
		{
			// If not, Reinsert to root
			removeDynamicCopmInOctreeElem(*it);
			insertDynamicComponent(*it, _octreeElemVec[0]);
			++it;
		}
	}
	*/
}

void OctreeForCollision::insertComponent(CollisionComponent * comp)
{
	if (comp->isDynamicComp())
		insertDynamicComponent(comp, _octreeElemVec[0]);
	else
		insertStaticComponent(comp, _octreeElemVec[0]);
}

void OctreeForCollision::insertStaticComponent(CollisionComponent * comp, OctreeElem& octreeElem)
{
	OctreeElem* curElem = &octreeElem;
	int index = curElem->getIndex();

	while (true)
	{
		curElem->_bUsed = true;
		int targetChildIdx = -1;

		if (curElem->_bUseChildren)
		{
			if (-1 == (targetChildIdx = getFitChildBoxIndexStaticComp(*curElem, comp)))
			{
				// no match with children
				curElem->_potentialStaticComponents.push_back_pointer(comp);
				comp->setOctreeElemIdx(index);
				return;
			}

			index = index * 8 + targetChildIdx;
			curElem = &_octreeElemVec[index];
		}
		else
		{
			curElem->_potentialStaticComponents.push_back_pointer(comp);
			comp->setOctreeElemIdx(index);

			//_potentialComponents 개수가 적으면 stop. 일정 수가 넘어가면 children으로.
			if (curElem->_potentialThreshold < curElem->_potentialStaticComponents.vecPSize() + curElem->_potentialDynamicComponents.vecPSize() && curElem->_height != 0)
			{
				splitComps(*curElem);
			}

			return;
		}
	}

	return;
}

void OctreeForCollision::insertDynamicComponent(CollisionComponent * comp, OctreeElem& octreeElem)
{
	OctreeElem* curElem = &octreeElem;
	int index = curElem->getIndex();

	while (true)
	{
		curElem->_bUsed = true;
		int targetChildIdx = -1;

		if (curElem->_bUseChildren)
		{
			if (-1 == (targetChildIdx = getFitChildBoxIndexDynamicComp(*curElem, comp)))
			{
				// no match with children
				curElem->_potentialDynamicComponents.push_back_pointer(comp);
				comp->setOctreeElemIdx(index);
				return;
			}

			index = index * 8 + targetChildIdx;
			curElem = &_octreeElemVec[index];
		}
		else
		{
			curElem->_potentialDynamicComponents.push_back_pointer(comp);
			comp->setOctreeElemIdx(index);

			//_potentialComponents 개수가 적으면 stop. 일정 수가 넘어가면 children으로.
			if (curElem->_potentialThreshold < curElem->_potentialStaticComponents.vecPSize() + curElem->_potentialDynamicComponents.vecPSize() && curElem->_height != 0)
			{
				splitComps(*curElem);
			}

			return;
		}
	}

	return;
}

void OctreeForCollision::splitComps(OctreeElem & octreeElem)
{
	int targetChildIdx;
	int index = octreeElem.getIndex();
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
			moveComp->setOctreeElemIdx(-1);
			insertStaticComponent(moveComp, childOctreeElem);
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
			moveComp->setOctreeElemIdx(-1);

			insertDynamicComponent(moveComp, childOctreeElem);
		}
		else
		{
			++it;
		}
	}

	return;
}

void OctreeForCollision::trySplitStaticComp(OctreeElem & octreeElem, CollisionComponent * comp)
{
	OctreeElem* curElem = &octreeElem;
	int index = curElem->getIndex();
	int targetChildIdx = -1;

	if (curElem->_bUseChildren)
	{
		if (-1 != (targetChildIdx = getFitChildBoxIndexStaticComp(*curElem, comp)))
		{
			// remove cur
			removeStaticCopmInOctreeElem(comp);
			insertStaticComponent(comp, _octreeElemVec[index * 8 + targetChildIdx]);
		}
	}

	// stay cur box
	return;
}

void OctreeForCollision::trySplitDynamicComp(OctreeElem & octreeElem, CollisionComponent * comp)
{
	OctreeElem* curElem = &octreeElem;
	int index = curElem->getIndex();
	int targetChildIdx = -1;

	if (curElem->_bUseChildren)
	{
		if (-1 != (targetChildIdx = getFitChildBoxIndexDynamicComp(*curElem, comp)))
		{
			// remove cur
			removeDynamicCopmInOctreeElem(comp);
			insertDynamicComponent(comp, _octreeElemVec[index * 8 + targetChildIdx]);
		}
	}

	// stay cur box
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
	int index = octreeElem.getIndex() * 8 + spaceIndex + 1;

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
	int index = octreeElem.getIndex() * 8 + spaceIndex + 1;

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
	int index = octreeElem.getIndex() * 8 + spaceIndex + 1;

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
	comp->setOctreeElemIdx(-1);

	// reduce count all parent
}

void OctreeForCollision::removeDynamicCopmInOctreeElem(CollisionComponent * comp)
{
	int index = comp->getOctreeElemIndex();
	OctreeElem& octreeElem = _octreeElemVec[index];
	octreeElem._potentialDynamicComponents.erase(comp);
	comp->setOctreeElemIdx(-1);

	// reduce count all parent
}

bool OctreeForCollision::IsUseThisOctreeElem(OctreeElem & elem)
{
	// parent _useChildBit check
	int idx = elem.getIndex();
	int mod = elem.getIndex() % 8;

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

void OctreeForCollision::TESTTEST(OctreeForCollision * octreeForCollision, int startIndex, std::vector<VectorP<CollisionComponent*>*> _staticAccVec, std::vector<VectorP<CollisionComponent*>*> _dynamicAccVec)
{
	// startElem
	OctreeElem& octreeElem = octreeForCollision->_octreeElemVec[startIndex];

	if (!octreeElem._bUsed)
		return;

	// accDynamic vs curDynamic
	for (auto accVector : _dynamicAccVec)
	{
		octreeForCollision->dynamicDynamicCollisionVecTest(*accVector, octreeElem._potentialDynamicComponents);
	}
	// accStatic vs curDynamic
	for (auto accVector : _staticAccVec)
	{
		octreeForCollision->staticDynamicCollisionVecTest(*accVector, octreeElem._potentialDynamicComponents);
	}

	// accDynamic vs curStatic
	for (auto accVector : _dynamicAccVec)
	{
		octreeForCollision->staticDynamicCollisionVecTest(octreeElem._potentialStaticComponents, *accVector);
	}
	// accStatic vs curStatic
	for (auto accVector : _staticAccVec)
	{
		octreeForCollision->staticStaticCollisionVecTest(*accVector, octreeElem._potentialStaticComponents);
	}

	// curDynamic vs curStatic
	octreeForCollision->staticDynamicCollisionVecTest(octreeElem._potentialStaticComponents, octreeElem._potentialDynamicComponents);

	// curStatic vs curStatic
	octreeForCollision->staticSelfCollisionVecTest(octreeElem._potentialStaticComponents);

	// curDynamic vs curDynamic
	octreeForCollision->dynamicSelfCollisionVecTest(octreeElem._potentialDynamicComponents);

	if (!octreeElem._bUseChildren)
	{
		return;
	}

	std::vector<VectorP<CollisionComponent*>*> newStaticAccVec(_staticAccVec);
	std::vector<VectorP<CollisionComponent*>*> newDynamicAccVec(_dynamicAccVec);

	newStaticAccVec.push_back(&(octreeElem._potentialStaticComponents));
	newDynamicAccVec.push_back(&(octreeElem._potentialDynamicComponents));

	int index = octreeElem.getIndex();
	for (int i = 0; i < OCT_NUM; i++)
	{
		// propagete copied compVec's vector
		//tbb::parallel_for(tbb::blocked_range<size_t>(0, 1), CTBB_staticCollideTest(octreeForCollision, startIndex * 8 + i, newStaticAccVec, newDynamicAccVec));
		octreeForCollision->TESTTEST(octreeForCollision, startIndex * 8 + i + 1, _staticAccVec, _dynamicAccVec);
	}

	return;
}

CTBB_staticCollideTest::CTBB_staticCollideTest(OctreeForCollision* octreeForCollision, int startIndex, std::vector<VectorP<CollisionComponent*>*> staticAccVec, std::vector<VectorP<CollisionComponent*>*> dynamicAccVec)
	: _octreeForCollision(octreeForCollision), _startIndex(startIndex), _staticAccVec(staticAccVec), _dynamicAccVec(dynamicAccVec)
{
}

void CTBB_staticCollideTest::operator()(const tbb::blocked_range<size_t>& r) const
{
	// startElem
	OctreeElem& octreeElem = _octreeForCollision->_octreeElemVec[_startIndex];

	if (!octreeElem._bUsed)
		return;

	// accDynamic vs curDynamic
	for (auto accVector : _dynamicAccVec)
	{
		_octreeForCollision->dynamicDynamicCollisionVecTest(*accVector, octreeElem._potentialDynamicComponents);
	}
	// accStatic vs curDynamic
	for (auto accVector : _staticAccVec)
	{
		_octreeForCollision->staticDynamicCollisionVecTest(*accVector, octreeElem._potentialDynamicComponents);
	}

	// accDynamic vs curStatic
	for (auto accVector : _dynamicAccVec)
	{
		_octreeForCollision->staticDynamicCollisionVecTest(octreeElem._potentialStaticComponents, *accVector);
	}
	// accStatic vs curStatic
	for (auto accVector : _staticAccVec)
	{
		_octreeForCollision->staticStaticCollisionVecTest(*accVector, octreeElem._potentialStaticComponents);
	}

	// curDynamic vs curStatic
	_octreeForCollision->staticDynamicCollisionVecTest(octreeElem._potentialStaticComponents, octreeElem._potentialDynamicComponents);

	// curStatic vs curStatic
	_octreeForCollision->staticSelfCollisionVecTest(octreeElem._potentialStaticComponents);

	// curDynamic vs curDynamic
	_octreeForCollision->dynamicSelfCollisionVecTest(octreeElem._potentialDynamicComponents);

	if (!octreeElem._bUseChildren)
	{
		return;
	}

	std::vector<VectorP<CollisionComponent*>*> newStaticAccVec(_staticAccVec);
	std::vector<VectorP<CollisionComponent*>*> newDynamicAccVec(_dynamicAccVec);

	newStaticAccVec.push_back(&(octreeElem._potentialStaticComponents));
	newDynamicAccVec.push_back(&(octreeElem._potentialDynamicComponents));

	int index = octreeElem.getIndex();
	for (int i = 0; i < OCT_NUM; i++)
	{
		// propagete copied compVec's vector
		tbb::parallel_for(tbb::blocked_range<size_t>(0, 1), CTBB_staticCollideTest(_octreeForCollision, _startIndex * 8 + i + 1, newStaticAccVec, newDynamicAccVec));
	}

	return;
}

CTBB_staticCollideTest_EX::CTBB_staticCollideTest_EX(OctreeForCollision * octreeForCollision, int startIndex, std::vector<VectorP<CollisionComponent*>*> staticAccVec, std::vector<VectorP<CollisionComponent*>*> dynamicAccVec)
	: _octreeForCollision(octreeForCollision), _startIndex(startIndex), _staticAccVec(staticAccVec), _dynamicAccVec(dynamicAccVec)
{
}

void CTBB_staticCollideTest_EX::operator()(const tbb::blocked_range<size_t>& r) const
{
	// startElem
	//OctreeElem& parentElem = _octreeForCollision->_octreeElemVec[_startIndex];
	for (size_t i = r.begin(); i != r.end(); ++i)
	{
		int cIndex = _startIndex * 8 + i + 1;
		OctreeElem& octreeElem = _octreeForCollision->_octreeElemVec[cIndex];

		if (!octreeElem._bUsed)
			continue;

		// accDynamic vs curDynamic
		for (auto accVector : _dynamicAccVec)
		{
			_octreeForCollision->dynamicDynamicCollisionVecTest(*accVector, octreeElem._potentialDynamicComponents);
		}
		// accStatic vs curDynamic
		for (auto accVector : _staticAccVec)
		{
			_octreeForCollision->staticDynamicCollisionVecTest(*accVector, octreeElem._potentialDynamicComponents);
		}

		// accDynamic vs curStatic
		for (auto accVector : _dynamicAccVec)
		{
			_octreeForCollision->staticDynamicCollisionVecTest(octreeElem._potentialStaticComponents, *accVector);
		}
		// accStatic vs curStatic
		for (auto accVector : _staticAccVec)
		{
			_octreeForCollision->staticStaticCollisionVecTest(*accVector, octreeElem._potentialStaticComponents);
		}

		// curDynamic vs curStatic
		_octreeForCollision->staticDynamicCollisionVecTest(octreeElem._potentialStaticComponents, octreeElem._potentialDynamicComponents);

		// curStatic vs curStatic
		_octreeForCollision->staticSelfCollisionVecTest(octreeElem._potentialStaticComponents);

		// curDynamic vs curDynamic
		_octreeForCollision->dynamicSelfCollisionVecTest(octreeElem._potentialDynamicComponents);


		std::vector<VectorP<CollisionComponent*>*> newStaticAccVec(_staticAccVec);
		std::vector<VectorP<CollisionComponent*>*> newDynamicAccVec(_dynamicAccVec);

		if (!octreeElem._bUseChildren)
		{
			continue;
		}

		newStaticAccVec.push_back(&(octreeElem._potentialStaticComponents));
		newDynamicAccVec.push_back(&(octreeElem._potentialDynamicComponents));

		// propagete copied compVec's vector
		tbb::parallel_for(tbb::blocked_range<size_t>(0, 8), CTBB_staticCollideTest(_octreeForCollision, cIndex, newStaticAccVec, newDynamicAccVec));

		newStaticAccVec.pop_back();
		newDynamicAccVec.pop_back();
	}

	/*
	std::vector<VectorP<CollisionComponent*>*> newStaticAccVec(_staticAccVec);
	std::vector<VectorP<CollisionComponent*>*> newDynamicAccVec(_dynamicAccVec);

	
	for (int i = 0; i < OCT_NUM; i++)
	{
		int cIndex = _startIndex * 8 + i + 1;
		OctreeElem& octreeElem = _octreeForCollision->_octreeElemVec[cIndex];

		if (!octreeElem._bUsed || !octreeElem._bUseChildren)
		{
			continue;
		}
		
		newStaticAccVec.push_back(&(octreeElem._potentialStaticComponents));
		newDynamicAccVec.push_back(&(octreeElem._potentialDynamicComponents));

		// propagete copied compVec's vector
		tbb::parallel_for(tbb::blocked_range<size_t>(0, 8), CTBB_staticCollideTest(_octreeForCollision, cIndex, newStaticAccVec, newDynamicAccVec));

		newStaticAccVec.pop_back();
		newDynamicAccVec.pop_back();
	}
	*/

	return;
}


CTBB_staticCollideTest_EXXX::CTBB_staticCollideTest_EXXX(OctreeForCollision* octreeForCollision)
	: _octreeForCollision(octreeForCollision)
{
}

void CTBB_staticCollideTest_EXXX::operator()(const tbb::blocked_range<size_t>& r) const
{
	for (size_t _startIndex = r.begin(); _startIndex != r.end(); ++_startIndex)
	{
		// startElem
		OctreeElem& octreeElem = _octreeForCollision->_octreeElemVec[_startIndex];

		std::vector<VectorP<CollisionComponent*>*> _staticAccVec;
		std::vector<VectorP<CollisionComponent*>*> _dynamicAccVec;

		int parentIdx = (_startIndex - 1);
		while (parentIdx >= 0)
		{
			parentIdx /= 8;
			_staticAccVec.push_back(&(_octreeForCollision->_octreeElemVec[parentIdx]._potentialStaticComponents));
			_dynamicAccVec.push_back(&(_octreeForCollision->_octreeElemVec[parentIdx]._potentialDynamicComponents));
			parentIdx -= 1;
		}

		if (!octreeElem._bUsed)
			return;

		// accDynamic vs curDynamic
		for (auto accVector : _dynamicAccVec)
		{
			_octreeForCollision->dynamicDynamicCollisionVecTest(*accVector, octreeElem._potentialDynamicComponents);
		}
		// accStatic vs curDynamic
		for (auto accVector : _staticAccVec)
		{
			_octreeForCollision->staticDynamicCollisionVecTest(*accVector, octreeElem._potentialDynamicComponents);
		}

		// accDynamic vs curStatic
		for (auto accVector : _dynamicAccVec)
		{
			_octreeForCollision->staticDynamicCollisionVecTest(octreeElem._potentialStaticComponents, *accVector);
		}
		// accStatic vs curStatic
		for (auto accVector : _staticAccVec)
		{
			_octreeForCollision->staticStaticCollisionVecTest(*accVector, octreeElem._potentialStaticComponents);
		}

		// curDynamic vs curStatic
		_octreeForCollision->staticDynamicCollisionVecTest(octreeElem._potentialStaticComponents, octreeElem._potentialDynamicComponents);

		// curStatic vs curStatic
		_octreeForCollision->staticSelfCollisionVecTest(octreeElem._potentialStaticComponents);

		// curDynamic vs curDynamic
		_octreeForCollision->dynamicSelfCollisionVecTest(octreeElem._potentialDynamicComponents);

		if (!octreeElem._bUseChildren)
		{
			return;
		}

		std::vector<VectorP<CollisionComponent*>*> newStaticAccVec(_staticAccVec);
		std::vector<VectorP<CollisionComponent*>*> newDynamicAccVec(_dynamicAccVec);

		newStaticAccVec.push_back(&(octreeElem._potentialStaticComponents));
		newDynamicAccVec.push_back(&(octreeElem._potentialDynamicComponents));

		return;
	}
}



CTBB_UpdateStaticCollision_do::CTBB_UpdateStaticCollision_do(OctreeForCollision & octree)
	:_octreeForCollision(octree)
{
}

void CTBB_UpdateStaticCollision_do::operator()(CollisionComponent * comp) const
{
	comp->updateCollisionComp();				// update local collision box -> world collision box
												
	if (comp->isCollisionComponentDirty())		// check aabb is modified
		return;

	// check is in same block
	if (_octreeForCollision._octreeElemVec[comp->getOctreeElemIndex()].IsInBoxFitTestStaticComp(comp))
		_octreeForCollision._pushToSelfBoxCompsCvec.push_back(comp);
	else
		_octreeForCollision._pushToRootCompsCvec.push_back(comp);
}

CTBB_UpdateDynamicCollision_do::CTBB_UpdateDynamicCollision_do(OctreeForCollision & octree)
	:_octreeForCollision(octree)
{
}

void CTBB_UpdateDynamicCollision_do::operator()(CollisionComponent * comp) const
{
	comp->updateCollisionComp();				// update local collision box -> world collision box
	comp->updateDynamicLap();					// Dynamic lap update
												// check aabb is modified
	if (comp->isCollisionComponentDirty())
		return;

	// check is in same block
	if (_octreeForCollision._octreeElemVec[comp->getOctreeElemIndex()].IsInBoxFitTestDynamicComp(comp))
		_octreeForCollision._pushToSelfBoxCompsCvec.push_back(comp);
	else
		_octreeForCollision._pushToRootCompsCvec.push_back(comp);
}

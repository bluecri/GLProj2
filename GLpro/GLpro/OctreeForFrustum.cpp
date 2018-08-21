#include "stdafx.h"
#include "OctreeForFrustum.h"

#include "src/Camera/CameraManager.h"
#include "src/Camera/Camera.h"

#include "src/RenderTarget/Normal/NormalFObj.h"

#include "AABBOb.h"
#include "FrustumOb.h"
#include "./src./Entity.h"
#include "CollisionFuncStatic.h"

OctreeForFrustum* GOctreeForFrustum = nullptr;

OctreeForFrustum::OctreeForFrustum(int height, int halfAxisSize, glm::vec3 center)
{
	/*
	_center = center;
	_halfAxisSize = halfAxisSize;
	_height = height;
	_bUsed = false;
	_bUseChildren = false;
	*/

	_octreeElemVec.reserve(pow(8, height) + 1);
	_octreeElemVec = std::vector<OctreeFrustumElem>(pow(8, height) + 1, OctreeFrustumElem());

	int index = 0;
	_octreeElemVec[index]._height = height;
	_octreeElemVec[index]._aabbOb.updateAABBObAxis(halfAxisSize);
	_octreeElemVec[index]._aabbOb.updateAABBObCenter(center);
	_octreeElemVec[index]._index = index;
	_octreeElemVec[index]._potentialThreshold = 8;
	
	initOctreeElem(_octreeElemVec[index]);
}

void OctreeForFrustum::newlyInsertComponent(RENDER::RNormal::SharedDrawElement sharedElem)
{
	_usingComponents.push_back(sharedElem);
	insertToOctreeForFrustum(sharedElem);
	return;
}

void OctreeForFrustum::removeCopmInOctreeElem(RENDER::RNormal::SharedDrawElement sharedElem)
{
	int index = sharedElem->first->getPElemIdx();
	OctreeFrustumElem& octreeFrustumElem = _octreeElemVec[index];
	octreeFrustumElem._potentialComponents.erase(sharedElem.get());

	// reduce count all parent

}

/*
void OctreeForFrustum::setFrustumBitWithMainCamera()
{
	CAMERA::Camera* mainCam = *(GCameraManager->GetMainCamera());
	setFrustumBitWithCamera(mainCam);
}


void OctreeForFrustum::setFrustumBitWithCamera(CAMERA::Camera* cam)
{
	int result = cam->sphereInFrustum(_center, _frustumRadius2);

	switch (result)
	{
	case -1:
		// completely out
		break;

	case 0:
		// not cull int this octree list
		for (auto elem : _potentialDrawElems)
			elem->first->offFrustumCull();

		if (_height == 0)
			return;

		// resursive check in child octree
		for (int i = 0; i < OCT_NUM; i++)
		{
			_childTree[i]->setFrustumBitWithCamera(cam);
		}

		break;

	case 1:
		// completely in
		for (int i = 0; i < OCT_NUM; i++)
			_childTree[i]->notCullAllOctree();

		break;
	}

	return;
}

void OctreeForFrustum::notCullAllOctree()
{
	for (auto elem : _potentialDrawElems)
	{
		elem->first->offFrustumCull();
	}

	if (_height == 0)
	{
		return;
	}

	// resursive check
	for (int i = 0; i < OCT_NUM; i++)
	{
		_childTree[i]->notCullAllOctree();
	}
}
*/

/*
void OctreeForFrustum::clearPotentialCompPropa()
{
	if (_bUsed == false)
		return;

	_bUsed = false;
	_maxCountOfObjects = 0;

	_potentialDrawElems.clear();

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
*/

int OctreeForFrustum::getFitChildBoxIndex(OctreeFrustumElem& curFrustumElem, RENDER::RNormal::DrawElement* drawElemPtr)
{
	if (curFrustumElem._height == 0)
		return -1;

	int spaceIndex = curFrustumElem.getSpaceOfMatchedCenter(drawElemPtr);
	int childIndex = curFrustumElem._index * 8 + spaceIndex + 1;

	if (_octreeElemVec[childIndex].sphereIsInBoxTest(drawElemPtr))
	{
		return spaceIndex + 1;
	}

	return -1;	// 일치하는 하위 child box가 존재하지 않음.
}

void OctreeForFrustum::getAllObjListNoTest(std::list<RENDER::RNormal::DrawElement*>& outList, int elemIdx)
{
	OctreeFrustumElem& octreeElem = _octreeElemVec[elemIdx];

	if (!octreeElem._bUsed)
		return;

	for (auto elem : octreeElem._potentialComponents)
		outList.push_back(elem);

	// test child octree
	if (octreeElem._bUseChildren)
	{
		for (int i = 0; i < OCT_POS::OCT_NUM; i++)
		{
			getAllObjListNoTest(outList, elemIdx * 8 + i + 1);
		}
	}

	return;
}

void OctreeForFrustum::getObjListWithFrustumObLoop(FrustumOb & frustumOb, std::list<RENDER::RNormal::DrawElement*>& outList, int elemIdx)
{
	OctreeFrustumElem& octreeElem = _octreeElemVec[elemIdx];
	if (!octreeElem._bUsed)
		return;

	int testResult = CollisionFuncStatic::staticCheck_FRUSTUM_AABB_INOUTOVERLAP(frustumOb, octreeElem._aabbOb);
	// outside
	if (testResult == -1)
		return;

	if (testResult == 0) {
		// add this octree draw element
		for (auto elem : octreeElem._potentialComponents)
			outList.push_back(elem);

		// test child octree
		if (octreeElem._bUseChildren)
		{
			for (int i = 0; i < OCT_POS::OCT_NUM; i++)
			{
				getObjListWithFrustumObLoop(frustumOb, outList, elemIdx * 8 + i + 1);
			}
		}
		return;
	}

	if (testResult == 1)
	{
		// push all element in this octree and child octree
		getAllObjListNoTest(outList, elemIdx);
	}

	return;
}

void OctreeForFrustum::getObjListWithAABBObLoop(AABBOb & aabbOb, std::list<RENDER::RNormal::DrawElement*>& outList, int elemIdx)
{
	OctreeFrustumElem& octreeElem = _octreeElemVec[elemIdx];
	if (!octreeElem._bUsed)
		return;

	int testResult = CollisionFuncStatic::staticCheck_AABB_AABB_INOUTOVERLAP(frustumOb, octreeElem._aabbOb);
	// outside
	if (testResult == -1)
		return;

	if (testResult == 0) {
		// add this octree draw element
		for (auto elem : octreeElem._potentialComponents)
			outList.push_back(elem);

		// test child octree
		if (octreeElem._bUseChildren)
		{
			for (int i = 0; i < OCT_POS::OCT_NUM; i++)
			{
				getObjListWithFrustumObLoop(frustumOb, outList, elemIdx * 8 + i + 1);
			}
		}
		return;
	}

	if (testResult == 1)
	{
		// push all element in this octree and child octree
		getAllObjListNoTest(outList, elemIdx);
	}

	return;
}

void OctreeForFrustum::initOctreeElem(OctreeFrustumElem & elem)
{
	if (elem._height <= 0)
	{
		return;
	}

	for (int FB = 0; FB < 2; FB++) {
		for (int TB = 0; TB < 2; TB++) {
			for (int LR = 0; LR < 2; LR++) {
				{
					int childIdx = elem._index * 8 + (LR + TB * 2 + FB * 4 + 1);
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

void OctreeForFrustum::getObjListWithFrustumOb(FrustumOb& frustumOb, std::list<RENDER::RNormal::DrawElement*>& outList)
{
	return getObjListWithFrustumObLoop(frustumOb, outList, 0);
}

void OctreeForFrustum::getObjListWithAABBOb(AABBOb& aabbOb, std::list<RENDER::RNormal::DrawElement*>& outList)
{
	return	getObjListWithAABBObLoop(aabbOb, outList, 0);
}

void OctreeForFrustum::updateOctreeForFrustum()
{
	doOctreeUpdate();			// octree update
	insertSleepWaitComponent();	// insert new or sleep component to octree
}

void OctreeForFrustum::addNewSleepComp(RENDER::RNormal::SharedDrawElement comp)
{
	_potentialSleepDrawElems.push_back(comp);
}

RigidbodyComponent* OctreeForFrustum::getNearestRigidbodyComp(glm::vec3& pos, ENUM_ENTITY_TYPE entityTypeFilter, int categoryMaskBit)
{
	std::priority_queue<Dist, std::pair<BoxIndex, RENDER::RNormal::DrawElement*>> prioQue;
	prioQue.push(std::make_pair(0, std::make_pair(0, nullptr)));

	while (!prioQue.empty())
	{
		std::pair < Dist, std::pair<BoxIndex, RENDER::RNormal::DrawElement*> > topElem = prioQue.top();
		prioQue.pop();

		// top elem = box
		if (topElem.second.first != -1)
		{
			OctreeFrustumElem& octreeFrustumElem = _octreeElemVec[topElem.second.first];

			// push all comp.
			for (auto& elem : octreeFrustumElem._potentialComponents)
			{
				if ((false == elem->first->isBDeleted()) && (false == elem->second->getBindedEntity()->isBeDeleted() ))
				{
					// class test
					if ( (entityTypeFilter != ENUM_ENTITY_NOFILTER) && (elem->second->getBindedEntity()->getType() != entityTypeFilter))
					{
						continue;
					}

					// category test
					if (!elem->second->getBindedEntity()->testEntityCategoryBit(categoryMaskBit))
						continue;

					prioQue.push(std::make_pair(elem->first->getNearestDistToPoint(pos), std::make_pair(-1, elem)));
				}
			}

			if (octreeFrustumElem._height == 0)
			{
				// if leaf box, continue.
				continue;
			}

			for (int i = 1; i <= 8; i++)
			{
				// push all child box.
				OctreeFrustumElem& octreeFrustumChildElem = _octreeElemVec[topElem.second.first * 8 + i];
				prioQue.push(std::make_pair(octreeFrustumChildElem.getNearestDistToPoint(pos), std::make_pair(topElem.second.first * 8 + i, nullptr)));
			}
		}
		// top elem = comp
		else
		{
			// result
			return topElem.second.second->second;
		}
	}

	// no result
	return nullptr;
}

RigidbodyComponent * OctreeForFrustum::getRayTraceRigidbodyComp(const LineOb& lineOb, ENUM_ENTITY_TYPE entityTypeFilter)
{
	std::priority_queue<Dist, std::pair<BoxIndex, RENDER::RNormal::DrawElement*>> prioQue;
	prioQue.push(std::make_pair(0, std::make_pair(0, nullptr)));

	while (!prioQue.empty())
	{
		std::pair < Dist, std::pair<BoxIndex, RENDER::RNormal::DrawElement*> > topElem = prioQue.top();
		prioQue.pop();

		// top elem = box
		if (topElem.second.first != -1)
		{
			OctreeFrustumElem& octreeFrustumElem = _octreeElemVec[topElem.second.first];

			// push all comp.
			for (auto& elem : octreeFrustumElem._potentialComponents)
			{
				if ((false == elem->first->isBDeleted()) && (false == elem->second->getBindedEntity()->isBeDeleted()))
				{
					// class test
					if ((entityTypeFilter != ENUM_ENTITY_NOFILTER) && (elem->second->getBindedEntity()->getType() != entityTypeFilter))
					{
						continue;
					}

					// category test
					if (!elem->second->getBindedEntity()->testEntityCategoryBit(categoryMaskBit))
						continue;

					float sphereObToLineDist;
					// return dist if collision occur
					if (false == CollisionFuncStatic::staticCheck_LINE_SPHERE(lineOb, elem->first->getSphereObForFrustumRef(), sphereObToLineDist))	
						continue;

					prioQue.push(std::make_pair(sphereObToLineDist, std::make_pair(-1, elem)));
				}
			}

			if (octreeFrustumElem._height == 0)
			{
				// if leaf box, continue.
				continue;
			}

			for (int i = 1; i <= 8; i++)
			{
				// push all child box.
				OctreeFrustumElem& octreeFrustumChildElem = _octreeElemVec[topElem.second.first * 8 + i];
				prioQue.push(std::make_pair(octreeFrustumChildElem.getNearestDistToPoint(pos), std::make_pair(topElem.second.first * 8 + i, nullptr)));
			}
		}
		// top elem = comp
		else
		{
			// result
			return topElem.second.second->second;
		}
	}

	// no result
	return nullptr;
}

void OctreeForFrustum::insertSleepWaitComponent()
{
	// move sleep comp in Octree
	for (auto it = _potentialSleepDrawElems.begin(); it != _potentialSleepDrawElems.end();	)
	{
		if ((*it)->first->isBDeleted())
		{
			it = _potentialSleepDrawElems.erase(it);
			continue;
		}

		if (! ((*it)->first->isRender()) )
		{
			continue;
		}

		(*it)->first->onFrustumCull();		// default value = cull ON

		// update frustum
		(*it)->first->setFrustumPos((*it)->second);
		newlyInsertComponent(*it);
		it = _potentialSleepDrawElems.erase(it);
	}
}

void OctreeForFrustum::doOctreeUpdate()
{
	for (auto it = _usingComponents.begin(); it != _usingComponents.end();)
	{
		if ((*it)->first->isBDeleted())
		{
			// remove from octree & usingComponent
			removeCopmInOctreeElem(*it);
			it = _usingComponents.erase(it);

			continue;
		}

		if (!((*it)->first->isRender()))
		{
			// move from usingComponent to sleepCompContainer
			_potentialSleepDrawElems.push_back(*it);

			// remove from octree & usingComponent
			removeCopmInOctreeElem(*it);
			it = _usingComponents.erase(it);

			continue;
		}

		// update frustum
		(*it)->first->setFrustumPos((*it)->second);	
													// check aabb is modified
		if (!(*it)->second->isDirtyAll())
		{
			return;
		}

		// check is in same block
		if (_octreeElemVec[(*it)->first->getPElemIdx()].sphereIsInBoxTest((*it).get()))
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
			insertToOctreeForFrustum(*it);
		}
	}
}

void OctreeForFrustum::insertToOctreeForFrustum(RENDER::RNormal::SharedDrawElement drawElemShared)
{
	//std::stack<OctreeFrustumElem> elemStk;
	//std::stack<RENDER::RNormal::DrawElement*> compPtrStk;

	int index = 0;
	OctreeFrustumElem& curOctreeElem = _octreeElemVec[index];
	RENDER::RNormal::DrawElement* curCompPtr = drawElemShared.get();
	//elemStk.push(_octreeElemVec[index]);
	//compPtrStk.push(drawElemShared.get());

	while (1)
	{
		curElem._bUsed = true;
		int childSubIdx = -1;

		if (curElem._bUseChildren)
		{
			if (-1 == (childSubIdx = getFitChildBoxIndex(curElem, curCompPtr)))
			{
				curElem._potentialComponents.push_back(curCompPtr);
				curCompPtr->first->setOctreeElemIdx(index);
				return;
			}

			index = index * 8 + childSubIdx;
			curElem = _octreeElemVec[index];
		}
		else
		{
			curElem._potentialComponents.push_back(curCompPtr);
			curCompPtr->first->setOctreeElemIdx(index);

			//_potentialComponents 개수가 적으면 stop. 일정 수가 넘어가면 children으로.
			if (curElem._potentialThreshold < curElem._potentialComponents.vecPSize() && curElem._height != 0)
			{
				curElem._bUseChildren = true;

				for (auto it = curElem._potentialComponents.begin(); it != curElem._potentialComponents.end(); )
				{
					if (-1 != (childSubIdx = getFitChildBoxIndex(curElem, *it)))
					{
						// push to child from this(parent)
						RENDER::RNormal::DrawElement* moveComp = (*it);
						it = curElem._potentialComponents.erase(it);

						int childIndex = index * 8 + childSubIdx;
						OctreeFrustumElem& childOctreeElem = _octreeElemVec[childIndex];

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
		}
	}

	return;
}

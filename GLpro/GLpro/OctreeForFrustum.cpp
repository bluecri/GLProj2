#include "stdafx.h"
#include "OctreeForFrustum.h"

#include "src/Camera/CameraManager.h"
#include "src/Camera/Camera.h"

#include "src/RenderTarget/Normal/NormalFObj.h"

#include "AABBOb.h"
#include "FrustumOb.h"

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

	_maxCountOfObjects = 8;

	_octreeElemVec.reserve(pow(8, height) + 1);
	_octreeElemVec = std::vector<OctreeFrustumElem>(pow(8, height) + 1, OctreeFrustumElem());

	int index = 0;
	_octreeElemVec[index]._height = height;
	_octreeElemVec[index]._halfAxisSize = halfAxisSize;
	_octreeElemVec[index]._center = center;
	_octreeElemVec[index]._index = index;

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


// inside(octree totally in frustum), outside, overlap( + frustum totally in octree)
int OctreeForFrustum::isOctreeInFrustumOb(FrustumOb& frustumOb)
{
	glm::vec3 octreeMinPoint, octreeMaxPoint;
	glm::vec3 minPoint, maxPoint;
	glm::vec3 halfAxisVec3 = glm::vec3(_halfAxisSize, _halfAxisSize, _halfAxisSize);
	std::vector<glm::vec4>& frustum = frustumOb.getFrustum();

	int ret = 1;	// inside

	octreeMinPoint = _center - halfAxisVec3;
	octreeMaxPoint = _center + halfAxisVec3;

	for (int i = 0; i < 6; i++)
	{
		//frustum[i] = -frustum[i];

		if (frustum[i].x > 0.0)
		{
			minPoint.x = octreeMinPoint.x;
			maxPoint.x = octreeMaxPoint.x;
		}
		else
		{
			minPoint.x = octreeMaxPoint.x;
			maxPoint.x = octreeMinPoint.x;
		}

		if (frustum[i].y > 0.0)
		{
			minPoint.y = octreeMinPoint.y;
			maxPoint.y = octreeMaxPoint.y;
		}
		else
		{
			minPoint.y = octreeMaxPoint.y;
			maxPoint.y = octreeMinPoint.y;
		}

		if (frustum[i].z > 0.0)
		{
			minPoint.z = octreeMinPoint.z;
			maxPoint.z = octreeMaxPoint.z;
		}
		else
		{
			minPoint.z = octreeMaxPoint.z;
			maxPoint.z = octreeMinPoint.z;
		}

		// get plane <> point dist
		float dotMinVal = 0.0f;
		float dotMaxVal = 0.0f;
		for (int k = 0; k < 3; k++)
		{
			dotMinVal += frustum[i][k] * minPoint[k];
			dotMaxVal += frustum[i][k] * maxPoint[k];
		}
		dotMinVal += frustum[i][3];
		dotMaxVal += frustum[i][3];

		if (dotMinVal > 0.0f)
			return -1;	// outside
		if (dotMaxVal >= 0.0f)
			ret = 0;	// intersect
	}

	return ret;
}

// inside, outside, overlap, totally inside
int OctreeForFrustum::isOctreeInAABBOb(AABBOb& aabbObFrustum)
{
	int ret = 1;		// inside

	const glm::vec3& aabbCemter = aabbObFrustum.getCenterConstRef();
	const glm::vec3& aabbAxis = aabbObFrustum.getAxisConstRef();

	// in box text
	for (int i = 0; i < 3; i++)
	{
		// out condition ( center diff > HALF + half )
		if (fabs(_center[i] - aabbCemter[i]) > _halfAxisSize + aabbAxis[i])
			return -1;	// outside

						// intersection condition ( center diff <= HALF - half )
		if (fabs(_center[i] - aabbCemter[i]) > fabs(_halfAxisSize - aabbAxis[i]))
			ret = 0;	// overlap
	}

	if (ret == 1)
	{
		for (int i = 0; i < 3; i++)
		{
			if (aabbAxis[i] < _halfAxisSize)
				return ret;
		}
		ret = 2;	// octree is totaly in frustum.
	}

	return ret;
}

void OctreeForFrustum::getAllObjListNoTest(RENDER::RNormal::DrawElemContainer & outList)
{
	if (!_bUsed)
		return;

	for (auto elem : _potentialSleepDrawElems)
		outList.push_back(elem);

	// test child octree
	if (_bUseChildren)
	{
		for (int i = 0; i < OCT_POS::OCT_NUM; i++)
		{
			_childTree[i]->getAllObjListNoTest(outList);
		}
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
					glm::vec3 childCenter = elem._center;
					int childIdx = elem._index * 8 + (LR + TB * 2 + FB * 4 + 1);
					if (childIdx >= _octreeElemVec.size())
						return;

					childCenter.x += ((LR * 2) - 1) * elem._halfAxisSize / 2;	// (-1, 1) * half_half size
					childCenter.y += ((TB * 2) - 1) * elem._halfAxisSize / 2;
					childCenter.z += ((FB * 2) - 1) * elem._halfAxisSize / 2;

					_octreeElemVec[childIdx]._height = elem._height - 1;
					_octreeElemVec[childIdx]._halfAxisSize = elem._halfAxisSize / 2;
					_octreeElemVec[childIdx]._center = childCenter;
					_octreeElemVec[childIdx]._index = childIdx;
					_octreeElemVec[childIdx]._potentialAllCount = 0;
					_octreeElemVec[childIdx]._useChildBit = 0;
					_octreeElemVec[childIdx]._potentialThreshold = _maxCountOfObjects + elem._height + 1;

					initOctreeElem(_octreeElemVec[childIdx]);
				}
			}
		}
	}
}

void OctreeForFrustum::getObjListWithFrustumOb(FrustumOb& frustumOb, RENDER::RNormal::DrawElemContainer & outList)
{
	if (!_bUsed)
		return;

	// outside
	int testResult = isOctreeInFrustumOb(frustumOb);
	if (testResult == -1)
		return;

	if (testResult == 0) {
		// add this octree draw element
		for (auto elem : _potentialSleepDrawElems)
			outList.push_back(elem);

		// test child octree
		if (_bUseChildren)
		{
			for (int i = 0; i < OCT_POS::OCT_NUM; i++)
			{
				_childTree[i]->getObjListWithFrustumOb(frustumOb, outList);
			}
		}
		return;
	}
		
	if (testResult == 1)
	{
		// push all element in this octree and child octree
		getAllObjListNoTest(outList);
	}
	
	return;
}

void OctreeForFrustum::getObjListWithAABBOb(AABBOb& aabbOb, RENDER::RNormal::DrawElemContainer & outList)
{
	if (!_bUsed)
		return;

	// outside
	int testResult = isOctreeInAABBOb(aabbOb);
	if (testResult == -1)
		return;

	if (testResult == 0 || testResult == 1) {
		// add this octree draw element
		for (auto elem : _potentialSleepDrawElems)
			outList.push_back(elem);

		// test child octree
		if (_bUseChildren)
		{
			for (int i = 0; i < OCT_POS::OCT_NUM; i++)
			{
				_childTree[i]->getObjListWithAABBOb(aabbOb, outList);
			}

		}
		return;
	}

	if (testResult == 2)
	{
		// push all element in this octree and child octree
		getAllObjListNoTest(outList);
	}

	return;
}

void OctreeForFrustum::updateOctreeForFrustum()
{
	doOctreeUpdate();			// octree update
	insertSleepWaitComponent();	// insert new or sleep component to octree
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
	std::stack<OctreeFrustumElem> elemStk;
	std::stack<RENDER::RNormal::DrawElement*> compPtrStk;

	int index = 0;
	elemStk.push(_octreeElemVec[index]);
	compPtrStk.push(drawElemShared.get());

	while (!elemStk.empty())
	{
		OctreeFrustumElem curElem = elemStk.top();
		RENDER::RNormal::DrawElement* curCompPtr = compPtrStk.top();
		elemStk.pop();
		compPtrStk.pop();

		curElem._bUsed = true;
		index = curElem._index;
		int targetIdx = -1;

		if (curElem._bUseChildren)
		{
			if (-1 == (targetIdx = getFitChildBoxIndex(curElem, curCompPtr)))
			{
				curElem._potentialComponents.push_back(curCompPtr);
				curCompPtr->first->setPElemIdx(index);
				return;
			}

			index = index * 8 + targetIdx;
			elemStk.push(_octreeElemVec[index]);
			compPtrStk.push(curCompPtr);
		}
		else
		{
			curElem._potentialComponents.push_back(curCompPtr);
			curCompPtr->first->setPElemIdx(index);

			//_potentialComponents 개수가 적으면 stop. 일정 수가 넘어가면 children으로.
			if (curElem._potentialThreshold < curElem._potentialComponents.vecPSize() && curElem._height != 0)
			{
				curElem._bUseChildren = true;

				for (auto it = curElem._potentialComponents.begin(); it != curElem._potentialComponents.end(); )
				{
					if (-1 != (targetIdx = getFitChildBoxIndex(curElem, *it)))
					{
						// push to child from this(parent)
						index = index * 8 + targetIdx;
						elemStk.push(_octreeElemVec[index]);
						compPtrStk.push((*it));

						it = curElem._potentialComponents.erase(it);
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

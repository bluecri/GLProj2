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
						_childTree[childIdx] = new OctreeForFrustum(height - 1, _halfAxisSize / 2, childCenter);
					}
				}
			}
		}
	}
}

void OctreeForFrustum::insertSharedDrawElem(RENDER::RNormal::DrawElemContainer sharedElemContainer)
{
	for (auto sharedElem : sharedElemContainer)
	{
		if (sharedElem->first->isBDeleted())
			continue;

		sharedElem->first->onFrustumCull();		// default value = cull ON

		insertToOctreeForFrustum(sharedElem);
	}
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

int OctreeForFrustum::getFitChildBoxIndex(RENDER::RNormal::SharedDrawElement drawElem)
{
	if (_height == 0)
		return -1;

	for (int i = 0; i < OCT_POS::OCT_NUM; i++)
	{
		if (_childTree[i]->sphereIsInBoxTest(drawElem))
		{
			return i;
		}
	}
	return -1;	// 일치하는 하위 child box가 존재하지 않음.
}

// intersect == out
bool OctreeForFrustum::sphereIsInBoxTest(RENDER::RNormal::SharedDrawElement drawElem)
{
	glm::vec3&	sphereCenter = drawElem->first->getFrustumPosRef();
	float&		sphereRadius = drawElem->first->getFrustumRadiusRef();

	// in box text
	for (int i = 0; i < 3; i++)
	{
		// out condition ( center diff + half > HALF )
		if (fabs(_center[i] - sphereCenter[i]) > fabs(_halfAxisSize - sphereRadius))
			return false;
	}

	return true;
}

// check in/out/intersect
int OctreeForFrustum::sphereIsInBoxAllTest(RENDER::RNormal::SharedDrawElement drawElem)
{
	int ret = 1;		// inside
	glm::vec3&	sphereCenter = drawElem->first->getFrustumPosRef();
	float&		sphereRadius = drawElem->first->getFrustumRadiusRef();

	// in box text
	for (int i = 0; i < 3; i++)
	{
		// out condition ( center diff > HALF + half )
		if (fabs(_center[i] - sphereCenter[i]) > _halfAxisSize + sphereRadius)
			return -1;	// outside

						// intersection condition ( center diff <= HALF - half )
		if (fabs(_center[i] - sphereCenter[i]) > fabs(_halfAxisSize - sphereRadius))
			ret = 0;	// outside
	}

	return ret;
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

	for (auto elem : _potentialDrawElems)
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
		for (auto elem : _potentialDrawElems)
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
		for (auto elem : _potentialDrawElems)
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

void OctreeForFrustum::insertToOctreeForFrustum(RENDER::RNormal::SharedDrawElement drawElem)
{
	_bUsed = true;
	int targetIdx = -1;

	if (_bUseChildren)
	{
		if (-1 == (targetIdx = getFitChildBoxIndex(drawElem)))
		{
			_potentialDrawElems.push_back(drawElem);
			return;
		}

		_childTree[targetIdx]->insertToOctreeForFrustum(drawElem);
		return;
	}
	else
	{
		_potentialDrawElems.push_back(drawElem);

		//_potentialComponents 개수가 적으면 stop. 일정 수가 넘어가면 children으로.
		if (_maxCountOfObjects < _potentialDrawElems.size() && _height != 0)
		{
			_bUseChildren = true;

			for (auto it = _potentialDrawElems.begin(); it != _potentialDrawElems.end(); )
			{
				if (-1 != (targetIdx = getFitChildBoxIndex(*it)))
				{
					// push to child from this(parent)
					_childTree[targetIdx]->insertToOctreeForFrustum(*it);
					it = _potentialDrawElems.erase(it);
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

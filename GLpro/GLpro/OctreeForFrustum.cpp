#include "stdafx.h"
#include "OctreeForFrustum.h"

#include "src/Camera/CameraManager.h"
#include "src/Camera/Camera.h"

OctreeForFrustum* GOctreeForFrustum = nullptr;

OctreeForFrustum::OctreeForFrustum(int height, int halfAxisSize, glm::vec3 center)
{
	_frustumRadius2 = halfAxisSize*halfAxisSize * 3;

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

		// update pos with world
		sharedElem->first->setFrustumPos(sharedElem->second->getWorldPosVec());
		sharedElem->first->onFrustumCull();		// default value = cull ON
		insert(sharedElem->first);
	}
}

void OctreeForFrustum::insert(RENDER_TARGET::NORMAL::NormalFObj * drawElem)
{
	int targetIdx = -1;
	
	if (-1 == (targetIdx = getFitChildSphereBoxIndex(drawElem)))
	{
		_potentialDrawElems.push_back(drawElem);
		return;
	}

	_childTree[targetIdx]->insert(drawElem);

	return;
}

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
			elem->offFrustumCull();

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
		elem->offFrustumCull();
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


void OctreeForFrustum::clearPotentialCompPropa()
{
	_potentialDrawElems.clear();
	if (_height == 0)
	{
		return;
	}

	for (int i = 0; i < OCT_POS::OCT_NUM; i++)
	{
		_childTree[i]->clearPotentialCompPropa();
	}
}

int OctreeForFrustum::getFitChildSphereBoxIndex(RENDER_TARGET::NORMAL::NormalFObj * drawElem)
{
	if (_height == 0)
		return -1;

	if (_frustumRadius2 > drawElem->getFrustumRadius())
		return -1;

	for (int i = 0; i < OCT_POS::OCT_NUM; i++)
	{
		if (_childTree[i]->IsInSphereBoxTest(drawElem))
		{
			return i;
		}
	}
	return -1;	// 일치하는 하위 child box가 존재하지 않음.
}

bool OctreeForFrustum::IsInSphereBoxTest(RENDER_TARGET::NORMAL::NormalFObj * drawElem)
{
	if (glm::distance2(_center, drawElem->getFrustumPosRef()) <= _frustumRadius2 - drawElem->getFrustumRadius())
	{
		return true;
	}
	return false;
}

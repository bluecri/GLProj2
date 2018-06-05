#pragma once
#include "stdafx.h"
#include "src/RenderTarget/Normal/NormalFObj.h"
#include "Octree.h"
#include "src/Render/RNormal.h"

namespace CAMERA {
	class Camera;
}

class OctreeForFrustum
{
public:
	OctreeForFrustum(int height, int halfAxisSize, glm::vec3 center);
	void insertSharedDrawElem(RENDER::RNormal::DrawElemContainer sharedElemContainer);
	void setFrustumBitWithMainCamera();

	// 등록해놓은 모든 RENDER_TARGET::NORMAL::NormalFObj refresh.
	void clearPotentialCompPropa();

private:
	// insert 가능한 child box index return
	int getFitChildSphereBoxIndex(RENDER_TARGET::NORMAL::NormalFObj* drawElem);

	bool IsInSphereBoxTest(RENDER_TARGET::NORMAL::NormalFObj* drawElem);
	void insert(RENDER_TARGET::NORMAL::NormalFObj* drawElem);
	void setFrustumBitWithCamera(CAMERA::Camera * cam);
	void notCullAllOctree();


public:
	int _height;
	int _halfAxisSize;
	float _frustumRadius2;

	glm::vec3 _center;
	OctreeForFrustum* _childTree[OCT_POS::OCT_NUM];
	std::list<RENDER_TARGET::NORMAL::NormalFObj*> _potentialDrawElems;
};

extern OctreeForFrustum* GOctreeForFrustum;
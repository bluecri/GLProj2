#pragma once
#include "stdafx.h"
#include "src/RenderTarget/Normal/NormalFObj.h"
#include "Octree.h"
#include "src/Render/RNormal.h"

class FrustumOb;
class AABBOb;

namespace CAMERA {
	class Camera;
}

class OctreeForFrustum
{
public:
	OctreeForFrustum(int height, int halfAxisSize, glm::vec3 center);
	void insertSharedDrawElem(RENDER::RNormal::DrawElemContainer sharedElemContainer);
	void clearPotentialCompPropa();

	void getObjListWithFrustumOb(FrustumOb& frustumOb, RENDER::RNormal::DrawElemContainer& outList);
	void getObjListWithAABBOb(AABBOb& aabbOb, RENDER::RNormal::DrawElemContainer& outList);

private:

	// insert °¡´ÉÇÑ child box index return
	void	insertToOctreeForFrustum(RENDER::RNormal::SharedDrawElement drawElem);
	int		getFitChildBoxIndex(RENDER::RNormal::SharedDrawElement drawElem);
	bool	sphereIsInBoxTest(RENDER::RNormal::SharedDrawElement drawElem);
	int		sphereIsInBoxAllTest(RENDER::RNormal::SharedDrawElement drawElem);

	int isOctreeInFrustumOb(FrustumOb& frustumOb);
	int isOctreeInAABBOb(AABBOb& aabbObFrustum);
	void getAllObjListNoTest(RENDER::RNormal::DrawElemContainer& outList);

	//void setFrustumBitWithCamera(CAMERA::Camera * cam);
	//void notCullAllOctree();
	//void setFrustumBitWithMainCamera();

public:
	int _height;
	int _halfAxisSize;

	glm::vec3 _center;
	OctreeForFrustum* _childTree[OCT_POS::OCT_NUM];
	RENDER::RNormal::DrawElemContainer _potentialDrawElems;

	bool _bUsed;				// Whether this octree & children is used or not
	bool _bUseChildren;
	int  _maxCountOfObjects;

private:
};

extern OctreeForFrustum* GOctreeForFrustum;
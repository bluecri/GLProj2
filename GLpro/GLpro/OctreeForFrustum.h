#pragma once
#include "stdafx.h"
#include "src/RenderTarget/Normal/NormalFObj.h"
#include "Octree.h"
#include "OctreeFrustumElem.h"
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
	//void clearPotentialCompPropa();

	void getObjListWithFrustumOb(FrustumOb& frustumOb, RENDER::RNormal::DrawElemContainer& outList);
	void getObjListWithAABBOb(AABBOb& aabbOb, RENDER::RNormal::DrawElemContainer& outList);

	void updateOctreeForFrustum();

private:
	void insertSleepWaitComponent();
	void doOctreeUpdate();

	// insert °¡´ÉÇÑ child box index return
	void	insertToOctreeForFrustum(RENDER::RNormal::SharedDrawElement comp);
	int		getFitChildBoxIndex(OctreeFrustumElem& curFrustumElem, RENDER::RNormal::DrawElement* drawElemPtr);
	//bool	sphereIsInBoxTest(RENDER::RNormal::SharedDrawElement drawElem);
	int		sphereIs_InOutInter_BoxTest(RENDER::RNormal::SharedDrawElement drawElem);

	int isOctreeInFrustumOb(FrustumOb& frustumOb);
	int isOctreeInAABBOb(AABBOb& aabbObFrustum);
	void getAllObjListNoTest(RENDER::RNormal::DrawElemContainer& outList);

	//void setFrustumBitWithCamera(CAMERA::Camera * cam);
	//void notCullAllOctree();
	//void setFrustumBitWithMainCamera();

	void initOctreeElem(OctreeFrustumElem& elem);
	void newlyInsertComponent(RENDER::RNormal::SharedDrawElement sharedElem);
	void removeCopmInOctreeElem(RENDER::RNormal::SharedDrawElement sharedElem);
public:
	int _height;
	int _halfAxisSize;

	glm::vec3 _center;
	//OctreeForFrustum* _childTree[OCT_POS::OCT_NUM];

	bool _bUsed;				// Whether this octree & children is used or not
	bool _bUseChildren;
	int  _maxCountOfObjects;

private:
	RENDER::RNormal::DrawElemContainer	_potentialSleepDrawElems;

	std::vector<OctreeFrustumElem>		_octreeElemVec;
	RENDER::RNormal::DrawElemContainer	_usingComponents;
};

extern OctreeForFrustum* GOctreeForFrustum;
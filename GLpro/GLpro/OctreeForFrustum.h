#pragma once
#include "stdafx.h"
#include "src/RenderTarget/Normal/NormalFObj.h"
#include "Octree.h"
#include "OctreeFrustumElem.h"
#include "src/Render/RNormal.h"

class FrustumOb;
class AABBOb;
class LineOb;

namespace CAMERA {
	class Camera;
}

enum ENUM_ENTITY_TYPE;

class OctreeForFrustum
{
	using BoxIndex = int;
	using Dist = float;
public:
	OctreeForFrustum(int height, int halfAxisSize, glm::vec3 center);
	//void clearPotentialCompPropa();

	void getObjListWithFrustumOb(FrustumOb& frustumOb, std::list<RENDER::RNormal::DrawElement*>& outList);
	void getObjListWithAABBOb(AABBOb& aabbOb, std::list<RENDER::RNormal::DrawElement*>& outList);

	void updateOctreeForFrustum();
	void addNewSleepComp(RENDER::RNormal::SharedDrawElement comp);

	RigidbodyComponent* getNearestRigidbodyComp(glm::vec3& pos, ENUM_ENTITY_TYPE entityTypeFilter, int categoryMaskBit);
	RigidbodyComponent* getRayTraceRigidbodyComp(const LineOb& lineOb, ENUM_ENTITY_TYPE entityTypeFilter, int categoryMaskBit);

private:
	void insertSleepWaitComponent();
	void doOctreeUpdate();

	// insert °¡´ÉÇÑ child box index return
	void	insertToOctreeForFrustum(RENDER::RNormal::SharedDrawElement comp);
	int		getFitChildBoxIndex(OctreeFrustumElem& curFrustumElem, RENDER::RNormal::DrawElement* drawElemPtr);

	void getAllObjListNoTest(std::list<RENDER::RNormal::DrawElement*>& outList, int elemIdx);

	void getObjListWithFrustumObLoop(FrustumOb& frustumOb, std::list<RENDER::RNormal::DrawElement*>& outList, int elemIdx);
	void getObjListWithAABBObLoop(AABBOb& aabbOb, std::list<RENDER::RNormal::DrawElement*> & outList, int elemIdx);
	
	//void setFrustumBitWithCamera(CAMERA::Camera * cam);
	//void notCullAllOctree();
	//void setFrustumBitWithMainCamera();

	void initOctreeElem(OctreeFrustumElem& elem);
	void newlyInsertComponent(RENDER::RNormal::SharedDrawElement sharedElem);
	void removeCopmInOctreeElem(RENDER::RNormal::SharedDrawElement sharedElem);

private:
	RENDER::RNormal::DrawElemContainer	_potentialSleepDrawElems;

	std::vector<OctreeFrustumElem>		_octreeElemVec;
	RENDER::RNormal::DrawElemContainer	_usingComponents;
};

extern OctreeForFrustum* GOctreeForFrustum;
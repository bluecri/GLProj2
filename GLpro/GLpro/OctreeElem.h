#pragma once

#include "stdafx.h"
#include "AABBOb.h"
#include "VectorP.h"

class CollisionComponent;

class OctreeElem
{
public:
	OctreeElem();

	bool IsInBoxFitTest(CollisionComponent * comp);
	bool IsInBoxFitTestStaticComp(CollisionComponent * comp);
	bool IsInBoxFitTestDynamicComp(CollisionComponent * comp);
	int getSpaceOfMatchedCenter(CollisionComponent * comp);
	int getSpaceOfMatchedCenterStaticComp(CollisionComponent * comp);
	int getSpaceOfMatchedCenterDynamicComp(CollisionComponent * comp);

	int getIndex();;
	void setIndex(int idx);

private:
	int getSpaceOfMatchedCenterWithPos(glm::vec3& pos);

public:
	VectorP<CollisionComponent*> _potentialStaticComponents;
	VectorP<CollisionComponent*> _potentialDynamicComponents;

	int _height;
	//int _halfAxisSize;
	//glm::vec3 _center;
	AABBOb _aabbOb;

	bool _bUsed;				// Whether this octree & children is used or not
	bool _bUseChildren;

	int	_useChildBit;

	int _potentialAllCount;
	int _potentialThreshold;

private:
	int	_index;
};
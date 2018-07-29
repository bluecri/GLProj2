#pragma once

#include "stdafx.h"

class CollisionComponent;

class OctreeElem
{
public:
	OctreeElem();

	bool IsInBoxFitTest(CollisionComponent * comp);

public:
	VectorP<CollisionComponent*> _potentialComponents;

	int	_index;

	int _height;
	int _halfAxisSize;
	glm::vec3 _center;

	bool _bUsed;				// Whether this octree & children is used or not
	bool _bUseChildren;

	int	_useChildBit;

	int _potentialAllCount;
	int _potentialThreshold;
};
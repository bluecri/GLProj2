#pragma once

#include "stdafx.h"

class CollisionComponent;

class OctreeElem
{
public:
	OctreeElem();

	bool IsInBoxFitTest(CollisionComponent * comp);
	bool IsUseChild()
	{
		return _useChildBit != 0;
	}
	bool IsUseThis()
	{
		// parent _useChildBit check
	}

public:
	std::list<CollisionComponent*> _potentialComponents;

	int	_index;

	int _height;
	int _halfAxisSize;
	glm::vec3 _center;

	bool _bUsed;				// Whether this octree & children is used or not
	bool _bUseChildren;

	int	_useChildBit;

	int _potentialAllCount;
};
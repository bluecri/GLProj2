#pragma once
#include "stdafx.h"

namespace RENDER_TARGET { namespace NORMAL { class NormalFObj; } }

class OctreeFrustumElem
{
public:
	using DrawElement = std::pair<RENDER_TARGET::NORMAL::NormalFObj*, RigidbodyComponent*>;

	OctreeFrustumElem();

	bool sphereIsInBoxTest(DrawElement* drawElemPtr);

	// intersect == out
	int sphereIs_InOutInter_BoxTest(DrawElement * drawElemPtr);
	int getSpaceOfMatchedCenter(DrawElement* drawElemPtr);

public:
	VectorP<DrawElement*> _potentialComponents;

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
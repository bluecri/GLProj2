#pragma once
#include "stdafx.h"
#include "VectorPPairFirst.h"
#include "AABBOb.h"

namespace RENDER_TARGET { namespace NORMAL { class NormalFObj; } }

class OctreeFrustumElem
{
public:
	using DrawElement = std::pair<RENDER_TARGET::NORMAL::NormalFObj*, RigidbodyComponent*>;

	OctreeFrustumElem();

	// intersect == out
	bool sphereIsInBoxTest(DrawElement* drawElemPtr);

	// int sphereIs_InOutInter_BoxTest(DrawElement * drawElemPtr);
	int getSpaceOfMatchedCenter(DrawElement* drawElemPtr);
	
	float getNearestDistToPoint(glm::vec3& pos);

public:
	VectorPPairFirst<DrawElement::first_type, DrawElement::second_type> _potentialComponents;	// vector<DrawElement*>

	int	_index;

	int _height;
	//int _halfAxisSize;
	//glm::vec3 _center;
	AABBOb _aabbOb;

	bool _bUsed;				// Whether this octree & children is used or not
	bool _bUseChildren;

	int	_useChildBit;

	int _potentialAllCount;
	int _potentialThreshold;
};
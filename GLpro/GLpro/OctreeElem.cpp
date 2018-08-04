#include "OctreeElem.h"
#include "CollisionComponent.h"
#include "AABBOb.h"

OctreeElem::OctreeElem()
{
	_bUsed = false;
	_bUseChildren = false;
	_potentialComponents = VectorP<CollisionComponent*>();

	_useChildBit = 0;
}

/*
*	check if comp center is in octreeElem
*/
bool OctreeElem::IsInBoxFitTest(CollisionComponent * comp)
{
	/* AABB test
	for (int i = 0; i < 3; i++)
	{
	if (fabs(_center[i] - comp->_center[i]) >(_halfAxisSize[i] + comp->_halfAxisSize[i]))
	return false;
	}
	*/

	const AABBOb& aabbOb = comp->_aabbObForOctree;
	const glm::vec3& aabbCemter = aabbOb.getCenterConstRef();
	const glm::vec3& aabbAxis = aabbOb.getAxisConstRef();


	// in box text
	for (int i = 0; i < 3; i++)
	{
		// out condition ( center diff + half > HALF )
		if (fabs(_center[i] - aabbCemter[i]) > fabs(_halfAxisSize - aabbAxis[i]))
			return false;
	}

	return true;
}
/*
*	find matched child Space[0~7] of comp center
*	This is not check comp center in octreeElem Index. 8 SPACE! 
*/
int OctreeElem::getSpaceOfMatchedCenter(CollisionComponent * comp)
{
	const glm::vec3& center = comp->_aabbObForOctree.getCenterConstRef();
	int ret = 0;

	if (center.x > _center.x)
	{
		ret += 1;
	}

	if (center.y > _center.y)
	{
		ret += 2;
	}

	if (center.z > _center.z)
	{
		ret += 4;
	}

	return ret;
}

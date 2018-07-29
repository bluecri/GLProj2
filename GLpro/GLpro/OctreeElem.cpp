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

// insert °¡´ÉÇÑ child box index return
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

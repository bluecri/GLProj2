#include "OctreeFrustumElem.h"
#include "src/RenderTarget/Normal/NormalFObj.h"
#include "CollisionFuncStatic.h"

OctreeFrustumElem::OctreeFrustumElem()
{
	_bUsed = false;
	_bUseChildren = false;
	_potentialComponents = VectorP<DrawElement*>();

	_useChildBit = 0;
}

// intersect == out

bool OctreeFrustumElem::sphereIsInBoxTest(DrawElement* drawElemPtr)
{
	SphereOb& objSphere = drawElemPtr->first->getSphereObForFrustumRef();

	return CollisionFuncStatic::staticCheck_AABB_SPHERE_IN(_aabbOb, objSphere);
}

// check in /out /intersect
/*
int OctreeFrustumElem::sphereIs_InOutInter_BoxTest(DrawElement* drawElemPtr)
{
	SphereOb& objSphere = drawElemPtr->first->getSphereObForFrustumRef();
	return CollisionFuncStatic::staticCheck_AABB_SPHERE_IN(_aabbOb, objSphere);

	int ret = 1;		// inside
	glm::vec3&	sphereCenter = drawElemPtr->first->getFrustumPosRef();
	float&		sphereRadius = drawElemPtr->first->getFrustumRadiusRef();

	// in box text
	for (int i = 0; i < 3; i++)
	{
		// out condition ( center diff > HALF + half )
		if (fabs(_center[i] - sphereCenter[i]) > _halfAxisSize + sphereRadius)
			return -1;	// outside

						// intersection condition ( center diff <= HALF - half )
		if (fabs(_center[i] - sphereCenter[i]) > fabs(_halfAxisSize - sphereRadius))
			ret = 0;	// intersection
	}

	return ret;
}
*/

int OctreeFrustumElem::getSpaceOfMatchedCenter(DrawElement* drawElemPtr)
{
	const glm::vec3& center = drawElemPtr->first->getFrustumPosRef();
	const glm::vec3& octElemCenter = _aabbOb.getCenterConstRef();
	int ret = 0;

	if (center[0] > octElemCenter[0])
	{
		ret += 1;
	}

	if (center[1] > octElemCenter[1])
	{
		ret += 2;
	}

	if (center[2] > octElemCenter[2])
	{
		ret += 4;
	}

	return ret;
}

float OctreeFrustumElem::getNearestDistToPoint(glm::vec3 & pos)
{
	float retDist = 0.0f;
	CollisionFuncStatic::staticCheck_Dist_POINT_AABB(pos, _aabbOb, retDist);
	return retDist;
}

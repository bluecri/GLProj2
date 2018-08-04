#include "OctreeFrustumElem.h"
#include "src/RenderTarget/Normal/NormalFObj.h"

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
	glm::vec3&	sphereCenter = drawElemPtr->first->getFrustumPosRef();
	float&		sphereRadius = drawElemPtr->first->getFrustumRadiusRef();

	// in box text
	for (int i = 0; i < 3; i++)
	{
		// out condition ( center diff + half > HALF )
		if (fabs(_center[i] - sphereCenter[i]) > fabs(_halfAxisSize - sphereRadius))
			return false;
	}

	return true;
}

// check in /out /intersect
int OctreeFrustumElem::sphereIs_InOutInter_BoxTest(DrawElement* drawElemPtr)
{
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

int OctreeFrustumElem::getSpaceOfMatchedCenter(DrawElement* drawElemPtr)
{
	const glm::vec3& center = drawElemPtr->first->getFrustumPosRef();
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

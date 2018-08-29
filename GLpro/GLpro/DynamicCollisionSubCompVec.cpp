#include "DynamicCollisionSubCompVec.h"
#include "AABBOb.h"
#include "SphereOb.h"

void DynamicCollisionSubCompVec::updateLapWithSphere(SphereOb & spherePost)
{
	glm::vec3 prevToPostVecHalf = ( spherePost.getCenterConstRef() - _prevWorldVec ) / 2.0f;
	_lap.updateAABBObCenter(_prevWorldVec + prevToPostVecHalf);

	// make axis
	prevToPostVecHalf[0] = fabsf(prevToPostVecHalf[0]) + spherePost.getRadius();
	prevToPostVecHalf[1] = fabsf(prevToPostVecHalf[1]) + spherePost.getRadius();
	prevToPostVecHalf[2] = fabsf(prevToPostVecHalf[2]) + spherePost.getRadius();

	_lap.updateAABBObAxis(prevToPostVecHalf);
}

void DynamicCollisionSubCompVec::updateLapWithAABBOb(AABBOb & aabbPost)
{
	glm::vec3 prevToPostVec = (aabbPost.getCenter() - _prevWorldVec) / 2.0f;
	_lap.updateAABBObCenter(_prevWorldVec + prevToPostVec);

	prevToPostVec[0] = fabsf(prevToPostVec[0]);
	prevToPostVec[1] = fabsf(prevToPostVec[1]);
	prevToPostVec[2] = fabsf(prevToPostVec[2]);

	_lap.updateAABBObAxis(aabbPost.getAxisConstRef() + prevToPostVec);
}

void DynamicCollisionSubCompVec::saveCollideInfoToPrevInfo(const glm::vec3 & collsionWorldVec)
{
	_prevWorldVec = collsionWorldVec;
}
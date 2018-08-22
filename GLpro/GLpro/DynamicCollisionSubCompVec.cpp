#include "DynamicCollisionSubCompVec.h"
#include "AABBOb.h"

void DynamicCollisionSubCompVec::updateLapWithAABBOb(AABBOb & aabbPost)
{
	glm::vec3 prevToPostVec = aabbPost.getCenter() - _prevWorldVec;
	_lap.updateAABBObCenter(_prevWorldVec + prevToPostVec / 2.0f);

	prevToPostVec[0] = fabsf(prevToPostVec[0]);
	prevToPostVec[1] = fabsf(prevToPostVec[1]);
	prevToPostVec[2] = fabsf(prevToPostVec[2]);

	_lap.updateAABBObAxis(aabbPost.getAxisConstRef() + prevToPostVec);
}

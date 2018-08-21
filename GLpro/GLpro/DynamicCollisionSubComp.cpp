#include "DynamicCollisionSubComp.h"
#include "CollisionComponent.h"
#include "AABBCollisionComp.h"
#include "OBBCollisionComp.h"
#include "SphereCollisionComp.h"
#include "LineCollisionComp.h"

#include "RigidbodyComponent.h"

DynamicCollisionSubComp::DynamicCollisionSubComp()
{
}

const AABBOb & DynamicCollisionSubComp::getLapConstRef()
{
	return _lap;
}
/*
void DynamicCollisionSubComp::updateAABBObLapByCollisionComp(CollisionComponent * comp)
{
	// update aabb _lap
	switch ((int)(comp->collisionType))
	{
	case COLLISION_TYPE::COLLISION_AABB:
		makeOverlapAABBOb(comp);
		break;
	case COLLISION_TYPE::COLLISION_OBB:
		makeOverlapOBBOb(comp);
		break;
	case COLLISION_TYPE::COLLISION_SPHERE:
		makeOverlapSphereOb(comp);
		break;
	case COLLISION_TYPE::COLLISION_LINE:
		makeOverlapLineOb(comp);
		break;
	}
}

void DynamicCollisionSubComp::updatePrevWorldMat(RigidbodyComponent * rigidComp)
{
	_prevWorldMat = rigidComp->getWorldMatRef();
}
*/
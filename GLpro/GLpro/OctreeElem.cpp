#include "OctreeElem.h"
#include "CollisionComponent.h"
#include "AABBOb.h"
#include "CollisionFuncStatic.h"
#include "AABBCollisionComp.h"
#include "OBBCollisionComp.h"
#include "SphereCollisionComp.h"
#include "LineCollisionComp.h"
#include "DynamicCollisionSubComp.h"


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
	if (comp->isDynamicComp())
	{
		return CollisionFuncStatic::staticCheck_AABB_AABB_IN(_aabbOb, comp->getDynamicSubComp()->getLapConstRef());
	}

	switch (static_cast<int>(comp->collisionType))
	{
	case COLLISION_TYPE::COLLISION_AABB:
		return CollisionFuncStatic::staticCheck_AABB_AABB_IN(_aabbOb, static_cast<AABBCollisionComp*>(comp)->getAABBConstRef());
		break;
	case COLLISION_TYPE::COLLISION_OBB:
		return CollisionFuncStatic::staticCheck_AABB_AABB_IN(_aabbOb, static_cast<OBBCollisionComp*>(comp)->getAABBObConstRef());
		break;
	case COLLISION_TYPE::COLLISION_SPHERE:
		return CollisionFuncStatic::staticCheck_AABB_SPHERE_IN(_aabbOb, static_cast<SphereCollisionComp*>(comp)->getSphereObConstRef());
		break;
	case COLLISION_TYPE::COLLISION_LINE:
		return CollisionFuncStatic::staticCheck_AABB_LINE_IN(_aabbOb, static_cast<LineCollisionComp*>(comp)->getLineObConstRef());
		break;
	default:
		return false;
		break;

	}
	return false;
}
/*
*	find matched child Space[0~7] of comp center
*	This is not check comp center in octreeElem Index. 8 SPACE! 
*/
int OctreeElem::getSpaceOfMatchedCenter(CollisionComponent * comp)
{
	glm::vec3 center;
	if (comp->isDynamicComp())
	{
		center = comp->getDynamicSubComp()->getLapConstRef().getCenter();
	}
	else
	{
		switch (static_cast<int>(comp->collisionType))
		{
		case COLLISION_TYPE::COLLISION_AABB:
			center = static_cast<AABBCollisionComp*>(comp)->getAABBConstRef().getCenter();
			break;
		case COLLISION_TYPE::COLLISION_OBB:
			center = static_cast<OBBCollisionComp*>(comp)->getAABBObConstRef().getCenter();
			break;
		case COLLISION_TYPE::COLLISION_SPHERE:
			center = static_cast<SphereCollisionComp*>(comp)->getSphereObConstRef().getCenter();
			break;
		case COLLISION_TYPE::COLLISION_LINE:
			center = static_cast<LineCollisionComp*>(comp)->getLineObConstRef().getStartPos();
			break;
		default:
			return false;
			break;
		}
	}
	
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

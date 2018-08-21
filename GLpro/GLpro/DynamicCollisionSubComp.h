#pragma once
#include "stdafx.h"
#include "AABBOb.h"

class CollisionComponent;
class SphereOb;
class LineOb;

class DynamicCollisionSubComp
{
public:
	DynamicCollisionSubComp();

	const AABBOb& getLapConstRef();
	//virtual void updateAABBObLapByCollisionComp(CollisionComponent* comp);
	//void updatePrevWorldMat(RigidbodyComponent * rigidComp);

private:

	// to virtual?
	//void makeOverlapAABBOb(CollisionComponent* comp);
	//void makeOverlapOBBOb(CollisionComponent* comp);
	//void makeOverlapSphereOb(CollisionComponent* comp);
	//void makeOverlapLineOb(CollisionComponent* comp);

protected:
	AABBOb _lap;
};
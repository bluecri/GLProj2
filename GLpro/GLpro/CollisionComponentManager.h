#pragma once

#include "stdafx.h"

class RigidbodyComponent;
class CollisionComponent;
class OctreeForCollision;

enum COLLISION_COMP_TYPE
{
	COLLISION_COMP_OBB
};

class CollisionComponentManager
{
public:
	CollisionComponentManager(int height, int halfAxisSize);

	CollisionComponent* GetNewOBBCollisionComp(RigidbodyComponent* rigidComp, glm::mat4& localMat, glm::vec3& axisLen);
	CollisionComponent* GetNewAABBCollisionComp(RigidbodyComponent* rigidComp, glm::vec3& localVec, glm::vec3& axisLen);

	void eraseCollisionComponent(CollisionComponent* delTargetComp);

	void doCollisionTest();

private:

	void insertTestCompToOctaTree();
	void insertTestCompToOctaTreeWithContainer(std::list<CollisionComponent*>& collisionComponentContainer);
	void actualCollisionTest();
	void collisionTestWithContainer(std::list<CollisionComponent*>& collisionComponentContainer);
	void clearOctree();

private:
	std::list<CollisionComponent*> _collisionComponentContainerOBB;
	std::list<CollisionComponent*> _collisionComponentContainerAABB;
	OctreeForCollision* _octree;
};

extern CollisionComponentManager* GCollisionComponentManager;
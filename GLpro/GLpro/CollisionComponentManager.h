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
	CollisionComponent* GetNewSphereCollisionComp(RigidbodyComponent* rigidComp, glm::vec3& localVec, float radius);
	CollisionComponent* GetNewLineCollisionComp(RigidbodyComponent* rigidComp, glm::vec3& localVec, glm::vec3& unitVec, float len, bool isOneInfiniteLine);

	void eraseCollisionComponent(CollisionComponent* delTargetComp);

	void doCollisionTest();
	void clearOctree();

	void pushToSleepComponentContainer(CollisionComponent* sleepContainer);
	void resetAllCollisionCompDirty();

private:
	void insertSleepCompToOctTree();
	void insertTestCompToOctaTreeWithContainer(std::list<CollisionComponent*>& collisionComponentContainer);
	void actualCollisionTest();
	//void collisionTestWithContainer(std::list<CollisionComponent*>& collisionComponentContainer);

	void doOctreeUpdate();

private:
	std::list<CollisionComponent*> _collisionSleepComponentContainer;
	OctreeForCollision* _octree;
};

extern CollisionComponentManager* GCollisionComponentManager;
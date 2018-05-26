#pragma once

#include <list>

class RigidbodyComponent;
class CollisionComponent;
class Octree;

enum COLLISION_COMP_TYPE
{
	COLLISION_COMP_OBB
};

class CollisionComponentManager
{
public:
	CollisionComponentManager(int height, int halfAxisSize);

	CollisionComponent* GetNewCollisionComp(RigidbodyComponent* rigidComp, glm::mat4& localMat, glm::vec3& axisLen);

	void eraseCollisionComponent(CollisionComponent* delTargetComp);

	void doCollisionTest();

private:

	void insertTestCompToOctaTree();
	void collisionTest();
	void clearOctree();

private:
	std::list<CollisionComponent*> _collisionComponentContainer;
	Octree* _octree;
};

extern CollisionComponentManager* GCollisionComponentManager;
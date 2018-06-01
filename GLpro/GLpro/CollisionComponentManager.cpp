#include "stdafx.h"
#include "CollisionComponent.h"
#include "CollisionComponentManager.h"
#include "./src/Transform.h"
#include "RigidbodyComponent.h"
#include "OBBCollisionComp.h"
#include "AABBCollisionComp.h"
#include "Octree.h"

CollisionComponentManager::CollisionComponentManager(int height, int halfAxisSize)
{
	_octree = new Octree(height, halfAxisSize, glm::vec3());
}

CollisionComponent * CollisionComponentManager::GetNewOBBCollisionComp(RigidbodyComponent * rigidComp, glm::mat4 & localMat, glm::vec3 & axisLen)
{
	CollisionComponent* retComp = new OBBCollisionComp(rigidComp, localMat, axisLen);
	_collisionComponentContainerOBB.push_back(retComp);
	return retComp;
}

CollisionComponent * CollisionComponentManager::GetNewAABBCollisionComp(RigidbodyComponent * rigidComp, glm::vec3 & localVec, glm::vec3 & axisLen)
{
	CollisionComponent* retComp = new AABBCollisionComp(rigidComp, localVec, axisLen);
	_collisionComponentContainerAABB.push_back(retComp);
	return retComp;
}

void CollisionComponentManager::eraseCollisionComponent(CollisionComponent * delTargetComp)
{
	delTargetComp->setDeleted(true);
}

void CollisionComponentManager::doCollisionTest()
{
	insertTestCompToOctaTree();
	actualCollisionTest();
	clearOctree();
}

void CollisionComponentManager::insertTestCompToOctaTree() 
{
	insertTestCompToOctaTreeWithContainer(_collisionComponentContainerOBB);
	insertTestCompToOctaTreeWithContainer(_collisionComponentContainerAABB);
}

void CollisionComponentManager::insertTestCompToOctaTreeWithContainer(std::list<CollisionComponent*>& collisionComponentContainer)
{
	for (auto it = collisionComponentContainer.begin(); it != collisionComponentContainer.end(); )
	{
		// erase before test collision(insert octTree)
		if ((*it)->_bDeleted)
		{
			delete *it;
			it = collisionComponentContainer.erase(it);
			continue;
		}

		if (!((*it)->_bCollisionTest))
		{
			++it;
			continue;
		}

		(*it)->_bAlreadyVelocityUpdated = false;	// init for velocity update
													// 순서 주의
		(*it)->updateAABBForOctree();				// update aabb for _octree
		_octree->insert(*it);						// insert

		++it;
	}
}

void CollisionComponentManager::actualCollisionTest()
{
	collisionTestWithContainer(_collisionComponentContainerAABB);
	collisionTestWithContainer(_collisionComponentContainerOBB);
}

void CollisionComponentManager::collisionTestWithContainer(std::list<CollisionComponent*>& collisionComponentContainer)
{
	// O(k^2) : 각 component의 가능한 충돌 component list 가져오기
	for (auto elem : collisionComponentContainer)
	{
		std::list<CollisionComponent*> potentialCollisionList;
		_octree->getCollisionPotentialList(potentialCollisionList, elem);

		// 충돌 가능한 list와 충돌 Check
		for (auto potentialComp : potentialCollisionList)
		{
			// OBB Test
			if (elem->collideTestToOther(potentialComp))
			{
				// component에 충돌 정보 전달 ( for update logic )
				elem->_collisionLogList.push_back(potentialComp);

				if (elem->_bCollideVelocityUpdate && potentialComp->_bCollideVelocityUpdate && !elem->_bAlreadyVelocityUpdated && !potentialComp->_bAlreadyVelocityUpdated)
				{
					// modify velocity (탄성 충돌)

					elem->_bAlreadyVelocityUpdated = true;
					potentialComp->_bAlreadyVelocityUpdated = true;

					glm::vec3& v1 = elem->_rigidComp->_transform->getVelocityRef();
					glm::vec3& v2 = potentialComp->_rigidComp->_transform->getVelocityRef();
					float m1 = elem->_rigidComp->_transform->getMass();
					float m2 = potentialComp->_rigidComp->_transform->getMass();

					glm::vec3 v1Ret = v1 + m2*(v2 - v1) * 2.0f / (m1 + m2);
					glm::vec3 v2Ret = v2 + m2*(v1 - v2) * 2.0f / (m1 + m2);
				}
			}
		}
	}
}


void CollisionComponentManager::clearOctree()
{
	_octree->clearPotentialCompPropa();
}

CollisionComponentManager* GCollisionComponentManager = nullptr;
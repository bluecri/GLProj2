#include "stdafx.h"
#include "CollisionComponent.h"
#include "CollisionComponentManager.h"
#include "OBBCollisionComp.h"
#include "Octree.h"

CollisionComponentManager::CollisionComponentManager(int height, int halfAxisSize)
{
	_octree = new Octree(height, halfAxisSize, glm::vec3());
}

CollisionComponent * CollisionComponentManager::GetCollisionComp(RigidbodyComponent * rigidComp, glm::mat4 & localMat, glm::vec3 & axisLen)
{
	CollisionComponent* retComp = new OBBCollisionComp(rigidComp, localMat, axisLen);
	_collisionComponentContainer.insert(std::make_pair(retComp, retComp));
	return retComp;
}

void CollisionComponentManager::eraseCollisionComponent(CollisionComponent * delTargetComp)
{
	auto it = _collisionComponentContainer.find(delTargetComp);
	if (it != _collisionComponentContainer.end())
	{
		_collisionComponentContainer.erase(it);
		return;
	}
}

void CollisionComponentManager::doCollisionTest()
{
	insertAllCompToOctaTree();
	collisionTest();
	clearOctree();
}

void CollisionComponentManager::insertAllCompToOctaTree() 
{
	for (auto elem : _collisionComponentContainer)
	{
		// 순서 주의
		elem.first->updateAABBForOctree();		// update aabb for _octree
		_octree->insert(elem.first);				// insert
	}
}

void CollisionComponentManager::collisionTest()
{
	// O(k^2) : 각 component의 가능한 충돌 component list 가져오기
	for (auto elem : _collisionComponentContainer)
	{
		std::list<CollisionComponent*> potentialCollisionList;
		_octree->getCollisionPotentialList(potentialCollisionList, elem.second);

		// 충돌 가능한 list와 충돌 Check
		for (auto potentialComp : potentialCollisionList)
		{
			// OBB Test
			if (elem.second->collideTestToOther(potentialComp))
			{
				// component에 충돌 정보 전달 ( for update logic )
				elem.second->_collisionLogList.push_back(potentialComp);

				//todo : modify velocity
			}
		}

	}
}

void CollisionComponentManager::clearOctree()
{
	_octree->clearPotentialCompPropa();
}

CollisionComponentManager* GCollisionComponentManager = nullptr;
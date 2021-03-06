#include "stdafx.h"
#include "CollisionComponent.h"
#include "CollisionComponentManager.h"
#include "RigidbodyComponent.h"
#include "OBBCollisionComp.h"
#include "AABBCollisionComp.h"
#include "SphereCollisionComp.h"
#include "LineCollisionComp.h"
#include "Octree.h"

CollisionComponentManager::CollisionComponentManager(int height, int halfAxisSize)
{
	_octree = new OctreeForCollision(height, halfAxisSize, glm::vec3());
}

CollisionComponent * CollisionComponentManager::GetNewOBBCollisionComp(RigidbodyComponent * rigidComp, glm::mat4 & localMat, glm::vec3 & axisLen)
{
	CollisionComponent* retComp = new OBBCollisionComp(rigidComp, localMat, axisLen);
	_collisionSleepComponentContainer.push_back(retComp);
	return retComp;
}

CollisionComponent * CollisionComponentManager::GetNewAABBCollisionComp(RigidbodyComponent * rigidComp, glm::vec3 & localVec, glm::vec3 & axisLen)
{
	CollisionComponent* retComp = new AABBCollisionComp(rigidComp, localVec, axisLen);
	_collisionSleepComponentContainer.push_back(retComp);
	return retComp;
}

CollisionComponent * CollisionComponentManager::GetNewSphereCollisionComp(RigidbodyComponent * rigidComp, glm::vec3 & localVec, float radius)
{
	CollisionComponent* retComp = new SphereCollisionComp(rigidComp, localVec, radius);
	_collisionSleepComponentContainer.push_back(retComp);
	return nullptr;
}

CollisionComponent * CollisionComponentManager::GetNewLineCollisionComp(RigidbodyComponent * rigidComp, glm::vec3 & localVec, glm::vec3 & unitVec, float len, bool isOneInfiniteLine)
{
	CollisionComponent* retComp;
	if (isOneInfiniteLine)
	{
		retComp = new LineCollisionComp(rigidComp, localVec, unitVec, len, LINEOB_TYPE_ENUM::LINEOB_TYPE_INFINITE_ONE_DIRTECTION);
	}
	else
	{
		retComp = new LineCollisionComp(rigidComp, localVec, unitVec, len, LINEOB_TYPE_ENUM::LINEOB_TYPE_LEN);
	}
	_collisionSleepComponentContainer.push_back(retComp);
	return nullptr;
}

void CollisionComponentManager::eraseCollisionComponent(CollisionComponent * delTargetComp)
{
	delTargetComp->setDeleted(true);
}

void CollisionComponentManager::doCollisionTest()
{
	doOctreeUpdate();			// octree selp update
	insertSleepCompToOctTree();	// insert new or sleep component to octree
	actualCollisionTest();		// do actual collision test
}

void CollisionComponentManager::insertSleepCompToOctTree() 
{
	insertTestCompToOctaTreeWithContainer(_collisionSleepComponentContainer);
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

		(*it)->updateCollisionComp();				// update local collision box -> world collision box
		_octree->newlyInsertComponent(*it);			// insert

		it = collisionComponentContainer.erase(it);
	}
}

void CollisionComponentManager::actualCollisionTest()
{
	//collisionTestWithContainer(_octree->_usingComponents);
	_octree->doCollisionTest();
}
/*
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
			// self test
			if (elem == potentialComp)
				continue;

			// category test
			if (!elem->testCollisionCategoryBit(potentialComp))
				continue;

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

					glm::vec3& v1 = elem->_rigidComp->getVelocityRef();
					glm::vec3& v2 = potentialComp->_rigidComp->getVelocityRef();
					float m1 = elem->_rigidComp->getMass();
					float m2 = potentialComp->_rigidComp->getMass();

					glm::vec3 v1Ret = v1 + m2*(v2 - v1) * 2.0f / (m1 + m2);
					glm::vec3 v2Ret = v2 + m2*(v1 - v2) * 2.0f / (m1 + m2);
				}
			}
		}
	}
}
*/

void CollisionComponentManager::doOctreeUpdate()
{
	_octree->doOctreeUpdate();
}

void CollisionComponentManager::pushToSleepComponentContainer(CollisionComponent * sleepContainer)
{
	_collisionSleepComponentContainer.push_back(sleepContainer);
}

void CollisionComponentManager::resetAllCollisionCompDirty()
{
	_octree->resetAllCollisionCompDirty();
	_octree->collisionListReset();
}

void CollisionComponentManager::getCollisionCompAll(std::list<CollisionComponent*>** staticList, std::list<CollisionComponent*>** dynamicList)
{
	*staticList = &(_octree->_usingStaticComponents);
	*dynamicList = &(_octree->_usingDynamicComponents);
}

float CollisionComponentManager::getOctreeLevel() const
{
	return _octree->getLevel();
}

glm::vec3 CollisionComponentManager::getOctreeAxisLen() const
{
	return _octree->getAxisLen();
}

void CollisionComponentManager::clearOctree()
{
	_octree->clearPotentialCompPropa();
}

CollisionComponentManager* GCollisionComponentManager = nullptr;
#include "RigidbodyComponentManager.h"
#include "RigidbodyComponent.h"
#include "ObjectPoolLikeVector.h"

//template class ClassTypeLock<RigidbodyComponent>;

void RigidbodyComponentManager::updateRigidbodyComps(float deltaTime)
{
	/*
	for (auto it = _rigidCompList.begin(); it != _rigidCompList.end(); )
	{
		if ((*it)->isBDeleted())
		{
			ObjectPoolLikeVector<RigidbodyComponent>::operator delete(*it);
			//delete (*it);
			it = _rigidCompList.erase(it);
			continue;
		}

		(*it)->updateWorldMatrix(deltaTime);
		++it;
	}
	*/
	
	/*
	int outerSize = ObjectPoolLikeVector<RigidbodyComponentManager>::getListOuterSize();
	int innerSize = ObjectPoolLikeVector<RigidbodyComponentManager>::getListIneerSize();

	for (int outerIdx = 0; outerIdx < outerSize; outerIdx++)
	{
		for (int innerIdx = 0; innerIdx < innerSize; innerIdx++)
		{
			RigidbodyComponentManager* comp = ObjectPoolLikeVector<RigidbodyComponentManager>::getInnerElem(outerIdx, innerIdx);
			if (comp == nullptr)
				continue;

			if (!(comp->isBDeleted()))
			{
				continue
			}

			comp->updateWorldMatrix(deltaTime);
		}
	}
	*/

	int outerSize = ObjectPoolLikeVector<RigidbodyComponent>::getListOuterSize();
	int constInnerSize = ObjectPoolLikeVector<RigidbodyComponent>::getListIneerSize();

	tbb::parallel_for(tbb::blocked_range<size_t>(0, outerSize), CTBB_rigidUpdateOuter(constInnerSize, deltaTime));

	for (auto it = _rigidCompPtrList.begin(); it != _rigidCompPtrList.end();	)
	{
		if ((*it)->isBDeleted())
		{
			delete *it;
			it = _rigidCompPtrList.erase(it);
		}
		else
		{
			++it;
		}
	}

}

void RigidbodyComponentManager::resetAndSwapDirtyAll()
{
	tbb::parallel_do(_rigidCompPtrList.begin(), _rigidCompPtrList.end(), CTBB_Rigid_ResetAndSwapDirtyAll_do());
	/*
	for (auto elem : _rigidCompPtrList)
	{
		elem->resetAndSwapDirty();
	}
	*/
}

void RigidbodyComponentManager::resetRenderDirtyAll()
{
	tbb::parallel_do(_rigidCompPtrList.begin(), _rigidCompPtrList.end(), CTBB_Rigid_ResetRenderDirtyAll_do());
	/*
	for (auto elem : _rigidCompPtrList)
	{
		elem->setDirtyForRender(false);
	}
	*/
}

RigidbodyComponent * RigidbodyComponentManager::getNewRigidbodyComp(Entity * bindedEntity)
{
	RigidbodyComponent* ret = new RigidbodyComponent(bindedEntity);
	_rigidCompPtrList.push_back(ret);
	return ret;
}

CTBB_rigidUpdateOuter::CTBB_rigidUpdateOuter(int innerSize, float deltaTime) : _innerSize(innerSize), _deltaTime(deltaTime)
{}

void CTBB_rigidUpdateOuter::operator()(const tbb::blocked_range<size_t>& r) const
{
	for (size_t i = r.begin(); i != r.end(); ++i)
	{
		uint8_t* outerAddr = ObjectPoolLikeVector<RigidbodyComponent>::getListOuter(i);
		tbb::parallel_for(tbb::blocked_range<size_t>(0, _innerSize), CTBB_rigidUpdateInner(outerAddr, _deltaTime));
	}
}

CTBB_rigidUpdateInner::CTBB_rigidUpdateInner(uint8_t * outerAddr, float deltaTime) : _outerAddr(outerAddr), _deltaTime(deltaTime)
{}

void CTBB_rigidUpdateInner::operator()(const tbb::blocked_range<size_t>& r) const
{
	for (size_t i = r.begin(); i != r.end(); ++i)
	{
		RigidbodyComponent* comp = ObjectPoolLikeVector<RigidbodyComponent>::getElemWithListOuter(_outerAddr, i);

		if (comp == nullptr)
			continue;

		if (comp->isBDeleted())
		{
			continue;
		}

		comp->updateWorldMatrix(_deltaTime);
	}
}

RigidbodyComponentManager* GRigidbodyComponentManager = nullptr;

// template class ObjectPoolLikeVector<RigidbodyComponent>;
//template<typename T, int K = 100> class ObjectPoolLikeVector;

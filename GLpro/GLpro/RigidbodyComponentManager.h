#pragma once
#include "stdafx.h"

class Entity;
class RigidbodyComponent;

class RigidbodyComponentManager
{
public:
	void updateRigidbodyComps(float deltaTime);
	
	void resetAndSwapDirtyAll();
	void resetRenderDirtyAll();
	RigidbodyComponent* getNewRigidbodyComp(Entity * bindedEntity);
	
private:
	std::list<RigidbodyComponent*> _rigidCompPtrList;
};

extern RigidbodyComponentManager* GRigidbodyComponentManager;

class CTBB_rigidUpdateOuter
{
public:
	CTBB_rigidUpdateOuter(int innerSize, float deltaTime);
	void operator()(const tbb::blocked_range<size_t>& r) const;

private:
	const int _innerSize;
	float _deltaTime;
};

class CTBB_rigidUpdateInner
{
public:
	CTBB_rigidUpdateInner(uint8_t* outerAddr, float deltaTime);
	void operator()(const tbb::blocked_range<size_t>& r) const;

private:
	uint8_t* const _outerAddr;
	float _deltaTime;
};
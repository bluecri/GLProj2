#pragma once
#include "stdafx.h"
#include "VectorP.h"

class CollisionComponent;
class OctreeElem;

enum OCT_POS
{
	OCT_LTF,	// left top front
	OCT_RTF,
	OCT_LBF,
	OCT_RBF,
	OCT_LTB,
	OCT_RTB,
	OCT_LBB,
	OCT_RBB,	// right bottom back
	OCT_NUM
};

class OctreeForCollision
{
public:
	OctreeForCollision(int height, int halfAxisSize, glm::vec3 center);
	void initOctreeElem(OctreeElem& elem);
	void newlyInsertComponent(CollisionComponent* comp);

	// octree collision test
	void doCollisionTest();

	// 충돌 가능한 모든 CollisionComponent들을 potentialList에 등록
	void getCollisionPotentialList(std::list<CollisionComponent*>& potentialList, CollisionComponent* comp);

	// collision list reset
	void collisionListReset();

	// 등록해놓은 모든 CollisionComponent clear.
	void clearPotentialCompPropa();

	// dirty bit reset (after collision check finished, before logit update begin)
	void resetAllCollisionCompDirty();

	// self refresh update
	void doOctreeUpdate();

	int getLevel() const;
	glm::vec3 getAxisLen() const;

private:
	void insertComponent(CollisionComponent* comp);
	void insertStaticComponent(CollisionComponent* comp, OctreeElem& octreeElem);
	void insertDynamicComponent(CollisionComponent* comp, OctreeElem& octreeElem);

	void splitComps(OctreeElem& octreeElem);
	void trySplitStaticComp(OctreeElem& octreeElem, CollisionComponent* comp);
	void trySplitDynamicComp(OctreeElem& octreeElem, CollisionComponent* comp);

	void getAllCollisionPotentialList(std::list<CollisionComponent*>& potentialList, int idx);

	void innerDoCollisionTest(OctreeElem& octreeElem, std::vector<VectorP<CollisionComponent*>*> staticVecAcc, std::vector<VectorP<CollisionComponent*>*> dynamicVecAcc);
	void staticStaticCollisionVecTest(VectorP<CollisionComponent*>& staticCompVec1, VectorP<CollisionComponent*>& staticCompVec2);
	void staticDynamicCollisionVecTest(VectorP<CollisionComponent*>& staticCompVec, VectorP<CollisionComponent*>& dynamicCompVec);
	void dynamicDynamicCollisionVecTest(VectorP<CollisionComponent*>& dycompVec1, VectorP<CollisionComponent*>& dycompVec2);
	void staticSelfCollisionVecTest(VectorP<CollisionComponent*>& staticCompVec);
	void dynamicSelfCollisionVecTest(VectorP<CollisionComponent*>& dynamicCompVec);

	void staticStaticCollisionTest(CollisionComponent* staticComp1, CollisionComponent* staticComp2);
	void staticDynamicCollisionTest(CollisionComponent* staticComp, CollisionComponent* dynamicComp);
	void dynamicDynamicCollisionTest(CollisionComponent* dycomp1, CollisionComponent* dycomp2);

	void ssCollsionResolve();
	void ddAndDsCOllsionResolve();
	// comp와 parent octreeElem에 해당하는 child box index return
	int getFitChildBoxIndex(OctreeElem& octreeElem, CollisionComponent * comp);
	int getFitChildBoxIndexStaticComp(OctreeElem& octreeElem, CollisionComponent * comp);
	int getFitChildBoxIndexDynamicComp(OctreeElem& octreeElem, CollisionComponent * comp);

	//bool IsInBoxFitTest(CollisionComponent* comp);
	//int IsInBoxTestAll(CollisionComponent* comp);
	void removeCopmInOctreeElem(CollisionComponent* comp);
	void removeStaticCopmInOctreeElem(CollisionComponent* comp);
	void removeDynamicCopmInOctreeElem(CollisionComponent* comp);

	bool IsUseThisOctreeElem(OctreeElem& elem);
	bool IsUseChild(OctreeElem& elem);

	void TEE();
	void TESTTEST(OctreeForCollision* octreeForCollision, int startIndex, std::vector<VectorP<CollisionComponent*>*> staticAccVec, std::vector<VectorP<CollisionComponent*>*> dynamicAccVec);
public:
	std::vector<OctreeElem>		_octreeElemVec;
	std::list<CollisionComponent*> _usingStaticComponents;
	std::list<CollisionComponent*> _usingDynamicComponents;

private:
	//std::vector<std::pair<float, std::pair<CollisionComponent*, CollisionComponent*>>> ddAndDsCOllsionTime;
	tbb::concurrent_vector<std::pair<float, std::pair<CollisionComponent*, CollisionComponent*>>> ddAndDsCOllsionTime;
	tbb::concurrent_vector<std::pair<CollisionComponent*, CollisionComponent*>> ssCollsionCvec;
	
	tbb::concurrent_vector<CollisionComponent*>	_pushToRootCompsCvec;
	tbb::concurrent_vector<CollisionComponent*>	_pushToSelfBoxCompsCvec;

	friend class CTBB_staticCollideTest; 
	friend class CTBB_staticCollideTest_EX;
	friend class CTBB_staticCollideTest_EXXX;
	friend class CTBB_UpdateStaticCollision_do;
	friend class CTBB_UpdateDynamicCollision_do;
	//int  _maxCountOfObjects;

	int _level;
	glm::vec3 _axisLen;
};

class CTBB_staticCollideTest
{
public:
	CTBB_staticCollideTest(OctreeForCollision* octreeForCollision, int startIndex, std::vector<VectorP<CollisionComponent*>*> staticAccVec, std::vector<VectorP<CollisionComponent*>*> dynamicAccVec);
	void operator()(const tbb::blocked_range<size_t>& r) const;

private:
	OctreeForCollision* const _octreeForCollision;
	const int _startIndex;

	std::vector<VectorP<CollisionComponent*>*> const _staticAccVec;
	std::vector<VectorP<CollisionComponent*>*> const _dynamicAccVec;
};


class CTBB_staticCollideTest_EX
{
public:
	CTBB_staticCollideTest_EX(OctreeForCollision* octreeForCollision, int startIndex, std::vector<VectorP<CollisionComponent*>*> staticAccVec, std::vector<VectorP<CollisionComponent*>*> dynamicAccVec);
	void operator()(const tbb::blocked_range<size_t>& r) const;

private:
	OctreeForCollision* const _octreeForCollision;
	const int _startIndex;

	std::vector<VectorP<CollisionComponent*>*> const _staticAccVec;
	std::vector<VectorP<CollisionComponent*>*> const _dynamicAccVec;
};


class CTBB_staticCollideTest_EXXX
{
public:
	CTBB_staticCollideTest_EXXX(OctreeForCollision* octreeForCollision);
	void operator()(const tbb::blocked_range<size_t>& r) const;

private:
	OctreeForCollision* const _octreeForCollision;

};

/*
class CTBB_staticCollideTest_EX
{
public:
	CTBB_staticCollideTest_EX(OctreeForCollision* octreeForCollision, int startIndex, std::vector<VectorP<CollisionComponent*>*> staticAccVec, std::vector<VectorP<CollisionComponent*>*> dynamicAccVec);
	void operator()(const tbb::blocked_range<size_t>& r) const;

private:
	OctreeForCollision* const _octreeForCollision;
	const int _startIndex;

	std::vector<VectorP<CollisionComponent*>*> const _staticAccVec;
	std::vector<VectorP<CollisionComponent*>*> const _dynamicAccVec;
};
*/



class CTBB_UpdateStaticCollision_do
{
private:
	OctreeForCollision& const _octreeForCollision;
public:
	CTBB_UpdateStaticCollision_do(OctreeForCollision& octree);
	void operator()(CollisionComponent* comp) const;
};

class CTBB_UpdateDynamicCollision_do
{
private:
	OctreeForCollision& const _octreeForCollision;
public:
	CTBB_UpdateDynamicCollision_do(OctreeForCollision& octree);
	void operator()(CollisionComponent* comp) const;
};

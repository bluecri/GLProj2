#pragma once
#include "stdafx.h"

template <class TOBJECT, int ALLOC_COUNT = 100>
class ObjectPoolLikeVector : public ClassTypeLock<TOBJECT>
{
public:
	// [empty check bit : next Ptr] OR [empty check bit : Object]
	static void* operator new(size_t dummy)
	{
		LockGuard criticalSection;
		if (!mFreeList)
		{
			mFreeList = new uint8_t[sizeof(TOBJECT)*ALLOC_COUNT + ALLOC_COUNT];	// object size + empty check 1 byte
			listStartVec.push_back(mFreeList);

			uint8_t* pNext = mFreeList;
			uint8_t** ppCurr = reinterpret_cast<uint8_t**>(mFreeList + 1);	// after 1 byte

			for (int i = 0; i < ALLOC_COUNT - 1; ++i)
			{
				/// OBJECT의 크기가 반드시 포인터 크기보다 커야 한다
				pNext[0] = 0;	// byte init
				pNext += sizeof(TOBJECT) + 1;
				*ppCurr = pNext;	// save next before byte pos addr
				ppCurr = reinterpret_cast<uint8_t**>(pNext + 1);	// move next after byte
			}

			pNext[0] = 0;
			*ppCurr = 0; ///< 마지막은 0으로 표시(NULL)
			mTotalAllocCount += ALLOC_COUNT;
		}

		uint8_t* pAvailable = mFreeList;	// ret object pos include byte
		pAvailable[0] = 1;	// use this now.
		pAvailable = pAvailable + 1;	// after 1 byte
		mFreeList = *reinterpret_cast<uint8_t**>(pAvailable);	// set next empty obejct pos include 1 byte
		++mCurrentUseCount;

		return pAvailable;
	}

	static void	operator delete(void* obj)
	{
		LockGuard criticalSection;

		CRASH_ASSERT(mCurrentUseCount > 0);

		--mCurrentUseCount;

		uint8_t* bitPos = reinterpret_cast<uint8_t*>(obj) - 1;
		bitPos[0] = 0;

		*reinterpret_cast<uint8_t**>(obj) = mFreeList;
		mFreeList = bitPos;
	}

	// ---------------------------
	static int getListOuterSize()
	{
		return listStartVec.size();
	}

	static int getListIneerSize()
	{
		return ALLOC_COUNT;
	}

	static TOBJECT* getInnerElemWithIndex(int outerIndex, int innerIndex)
	{
		uint8_t* beforeBit = listStartVec[outerIndex] + innerIndex * (sizeof(TOBJECT) + 1);
		if (beforeBit[0] == 0)
			return nullptr;

		return reinterpret_cast<TOBJECT*>(beforeBit + 1);
	}

	static uint8_t* getListOuter(int outerIndex)
	{
		return listStartVec[outerIndex];
	}

	static TOBJECT* getElemWithListOuter(uint8_t* const outerListAddr, int innerIndex)
	{
		uint8_t* beforeBit = outerListAddr + innerIndex * (sizeof(TOBJECT) + 1);
		if (beforeBit[0] == 0)
			return nullptr;

		return reinterpret_cast<TOBJECT*>(beforeBit + 1);
	}


private:
	static std::vector<uint8_t*> listStartVec;
	static uint8_t*	mFreeList;
	static int		mTotalAllocCount; ///< for tracing
	static int		mCurrentUseCount; ///< for tracing

};


template <class TOBJECT, int ALLOC_COUNT>
uint8_t* ObjectPoolLikeVector<TOBJECT, ALLOC_COUNT>::mFreeList = nullptr;

template <class TOBJECT, int ALLOC_COUNT>
int ObjectPoolLikeVector<TOBJECT, ALLOC_COUNT>::mTotalAllocCount = 0;

template <class TOBJECT, int ALLOC_COUNT>
int ObjectPoolLikeVector<TOBJECT, ALLOC_COUNT>::mCurrentUseCount = 0;

template <class TOBJECT, int ALLOC_COUNT>
std::vector<uint8_t*> ObjectPoolLikeVector<TOBJECT, ALLOC_COUNT>::listStartVec = std::vector<uint8_t*>();
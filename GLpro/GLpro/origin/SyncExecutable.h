#pragma once

#include "XTL.h"
#include "TypeTraits.h"
#include "FastSpinlock.h"
//#include "Timer.h"


class SyncExecutable : public std::enable_shared_from_this<SyncExecutable>
{
public:
	SyncExecutable() : mLock(LO_BUSINESS_CLASS)
	{}
	virtual ~SyncExecutable() {}

	template <class R, class T, class... Args>
	R DoSync(R (T::*memfunc)(Args...), Args... args)
	{
		static_assert(true == std::is_convertible<T, SyncExecutable>::value, "DoSync T should be derived from SyncExecutable");

		//VSTODO: mLock���� ��ȣ�� ���¿���, memfunc�� �����ϰ� ����� R�� ����
		FastSpinlockGuard(mLock, true);
		//EnterLock();
		R ret = memFunc(args...);		//vs return (static_cast<T*>(this)->*memfunc)(args...);
		//LeaveLock();
	
		return ret;
	}
	
	void EnterReadLock() { mLock.EnterReadLock(); }
	void LeaveReadLock() { mLock.LeaveReadLock(); }
	void EnterWriteLock() { mLock.EnterWriteLock(); }
	void LeaveWriteLock() { mLock.LeaveWriteLock(); }
	
 	template <class T>
	std::shared_ptr<T> GetSharedFromThis()
 	{
		//static_assert(true == std::is_convertible<T, SyncExecutable>::value, "GetSharedFromThis T should be derived from SyncExecutable");
		
		// this �����͸� std::shared_ptr<T>���·� ��ȯ.
		//(HINT: �� Ŭ������ std::enable_shared_from_this���� ��ӹ޾Ҵ�. �׸��� static_pointer_cast ���)
		//return std::shared_ptr<T>((Player*)this); ///< X X X
		return std::static_pointer_cast<T>( shared_from_this() );
 	}

private:
	
	FastSpinlock mLock;
};


template <class T, class F, class... Args>
void DoSyncAfter(uint32_t after, T instance, F memfunc, Args&&... args)
{
	static_assert(true == is_shared_ptr<T>::value, "DoSyncAfter T should be shared_ptr");
	static_assert(true == std::is_convertible<T, std::shared_ptr<SyncExecutable>>::value, "DoSyncAfter T should be shared_ptr SyncExecutable");

	//TOooDO: instance�� memfunc�� bind�� ��� LTimer->PushTimerJob() ����

	LTimer->PushTimerJob(instance, std::bind(memfunc, std::forward<Args>(args)...), after);
	
}
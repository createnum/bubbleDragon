#ifndef NEWBEEUTILS_NBLOCK_H
#define NEWBEEUTILS_NBLOCK_H
#include "newbeeUtils/exports.h"
#if defined(UNIX)
#include <pthread.h>
#else
#include <Windows.h>
#endif
namespace NEWBEE_UTILS{
class NBUAPI NBLock //: public // (4)
{
public :
	NBLock(){
	}
	virtual ~NBLock(){
	}
#if defined(UNIX)
	int init(){
		return pthread_mutex_init(&m_Mutex, NULL);
	} 
	int release(){
		return pthread_mutex_destroy(&m_Mutex);
	}
	int lock(){
		return pthread_mutex_lock(&m_Mutex);
	}
	int unlock(){
		return pthread_mutex_unlock(&m_Mutex);
	}
protected:
	pthread_mutex_t m_Mutex;
#else
	int init(){
		InitializeCriticalSection(&m_Lock);
		return 0;
	}
	int release(){
		DeleteCriticalSection(&m_Lock);
		return 0;
	}
	int lock(){
		EnterCriticalSection(&m_Lock);
		return 0;
	}
	int unlock(){
		LeaveCriticalSection(&m_Lock);
		return 0;
	}
protected:
	CRITICAL_SECTION m_Lock;
#endif
};
class NBUAPI NBSynGuard //: public // (4)
{
public :
	NBSynGuard(NBLock *myLock){
		m_myLock = myLock;
		m_myLock->lock();
	}
	~NBSynGuard(){
		m_myLock->unlock();
	}
	NBLock *m_myLock;
};
}
#endif //NEWBEEUTILS_NBLOCK_H


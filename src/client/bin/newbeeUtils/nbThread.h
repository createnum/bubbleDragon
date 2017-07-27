#ifndef NEWBEEUTILS_NBTHREAD_H
#define NEWBEEUTILS_NBTHREAD_H
#include "newbeeUtils/exports.h"
#if defined(UNIX)
#include<pthread.h>
#else
#endif
namespace NEWBEE_UTILS{
class NBUAPI NBThread
{
public:
    NBThread();
    virtual ~NBThread();
	int setName(const char* name);
	const char* getName();
public :
	int start(); // 应与 release() 配对，成对出现。
	int release(); // 应与 start() 配对，成对出现。
	virtual int run() = 0;
	unsigned long getThreadId(){ // printf 打印 使用 %lu
#if defined(UNIX)
        return (unsigned long)(m_osId);
#else
        return m_osId;
#endif
	}
	static const int THREAD_NAME_LEN = 32;
protected:
	int clear();
	void NBThread_();
	int NBThread__();

	char m_threadName[THREAD_NAME_LEN+1];
	#if defined(UNIX)
    pthread_t m_osId;
	#else
    unsigned long m_osId; // 操作系统分配的Id
	void *m_hThread;
	//HANDLE m_hThread;
	#endif
};


} // namespace NEWBEE_UTILS
//#if defined(UNIX)
//void* nbThreadProcess(void* derivedThread);
//#else
////DWORD WINAPI nbThreadProcess(void* derivedThread);
//#endif
#endif //NEWBEEUTILS_NBTHREAD_H

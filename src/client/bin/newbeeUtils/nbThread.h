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
	int start(); // Ӧ�� release() ��ԣ��ɶԳ��֡�
	int release(); // Ӧ�� start() ��ԣ��ɶԳ��֡�
	virtual int run() = 0;
	unsigned long getThreadId(){ // printf ��ӡ ʹ�� %lu
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
    unsigned long m_osId; // ����ϵͳ�����Id
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

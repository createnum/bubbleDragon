#pragma once
#include "logging2file.h"
namespace NEWBEE_UTILS{
class LoggingAll :
	public Logging2file
{
public:
	static Logging* getInstance(int sort, const char* filename, BUFFERING_MODE bufferMode, REWRITE_MODE rewriteMode, MODE_SEGMENT segmentMode, MODE_THREAD_SAFE tsMode);
	const static int MAX_LOGPOINTER = 16;

protected:
	virtual int switchFile_for_segment(time_t now);
	void println(const char*);
	virtual void justPrint(const char*, const long);
	virtual void justPrint(const char*, const unsigned long);
	virtual void justPrint(const char* prefix, const double num);
	virtual void justPrint(const char*, const char*, int length);
	virtual void justPrint(const char*, const char*);
	virtual void justPrint(const char*, const unsigned char*,int length);
	virtual void justPrint(const char*, const void *);
	virtual void justPrint(const char*, const void *, long);
	virtual void justPrint(const char*, const void **);
	virtual void justPrint(const char c);
	virtual void justPrint(const char* str);
	void justPrints(const char*, va_list argp);
	int createLogs(int sort, const char* filename, BUFFERING_MODE bufferMode, REWRITE_MODE rewriteMode, MODE_SEGMENT segmentMode, MODE_THREAD_SAFE tsMode);
	void LoggingAll_(void);
	void LoggingAll__(void);

	int number_of_logPointer;
	Logging* m_logPointers[MAX_LOGPOINTER];
	virtual int lock();
	virtual int unlock();
private:
	LoggingAll(void);
	virtual ~LoggingAll(void);
	static LoggingAll* m_this4std_file;
	static LoggingAll* m_this4std_autofile;
	static LoggingAll* m_this4file_autofile;
	static LoggingAll* m_this4std_file_autofile;
};
}
inline void NEWBEE_UTILS::LoggingAll::justPrint(const char c)
{
	for (int i=0; i< number_of_logPointer; i++)
	{
		m_logPointers[i]->justPrint(c);
	}
}
inline void NEWBEE_UTILS::LoggingAll::justPrint(const char* str)
{
	for (int i=0; i< number_of_logPointer; i++)
	{
		m_logPointers[i]->justPrint(str);
	}
}


//////////////////////////////////////////////////////////////////////////
//单体，向根据系统时间自动生成的文件中输出记录的类。
//
//////////////////////////////////////////////////////////////////////

#ifndef Logging2file_H
#define Logging2file_H
#include "logging.h"

namespace NEWBEE_UTILS{
class DLL_EXPORT Logging2file : public Logging
{
	friend class Logging;
public:
	static const int MAX_FILE_PATH_LEN = 128; //超过这个的文件名将被截断。
protected:
	inline int embellishFileName_for_segment(char* newFileName, time_t now){
		return embellishFileName_for_segment(newFileName, m_originalFileName, now, m_segmentMode);
	}
	int embellishFileName_for_segment(char* newFileName, const char *originalFileName, time_t now, MODE_SEGMENT segmentMode);
	virtual int switchFile_for_segment(time_t now);
	virtual void clear();
	virtual int switchFile(const char* fileName);
	virtual const char* getCurFileName();
	static Logging2file* getInstance(int sort, const char* filename, BUFFERING_MODE mode, REWRITE_MODE rewriteMode, MODE_SEGMENT segmentMode, MODE_THREAD_SAFE tsMode);
	//全部的虚函数，以实现自己的功能
	//void justPrints(const char*, va_list argp);
	void enableSingleLogFileMode();
	//void println();
	//void println(const char*);
	//virtual void justPrint(const char*, const long);
	//virtual void justPrint(const char*, const unsigned long);
	//virtual void justPrint(const char* prefix, const double num);
	//virtual void justPrint(const char*, const char*, int length);
	//virtual void justPrint(const char*, const char*);
	//virtual void justPrint(const char*, const unsigned char*, int length);
	//virtual void justPrint(const char*, const void *);//打印指针本身的值，就是把void*类型转换成长整形十六进制数%lx打印出来。
	//virtual void justPrint(const char*, const void *, long length);//打印指针所指内存区域的值，长度由length指定。
	//virtual void justPrint(const char*, const void **);
	//virtual void justPrint(const char c);
	//virtual void justPrint(const char* str);
	static Logging2file *createOne(const char *filename, BUFFERING_MODE bufferMode, REWRITE_MODE rewriteMode, MODE_SEGMENT segmentMode, MODE_THREAD_SAFE tsMode);
	int openFile_for_logging(const char* filename);
	void Logging2file__();
	void Logging2file_();

	virtual ~Logging2file();
	Logging2file(FILE *fp): Logging(fp)	{
		Logging2file_();
	};
	char m_originalFileName[MAX_FILE_PATH_LEN+1];
	//char m_curFileName[MAX_FILE_PATH_LEN+ Timing::LEAST_LENGTH_TIME_STRING + 1];
	int m_curHour, m_curDay; // 为 按天分文件打印 和 按小时分文件打印 准备

private:
	static Logging2file* m_this;//// for OUTPUT_FILE
	static Logging2file* m_autoThis;// for OUTPUT_AUTOFILE
};
}

//inline void NEWBEE_UTILS::Logging2file::justPrint(const char c)
//{
//	fprintf(m_fp, "%c", c);
//}
//inline void NEWBEE_UTILS::Logging2file::justPrint(const char* str)
//{
//	fprintf(m_fp, "%s", str);
//}
#endif // !defined(Logging2file_H)


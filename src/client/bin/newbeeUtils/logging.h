/* Logging.h: 打印日志工具，打印时间戳，可以设定打印到标准输出，错误输出，和任意文件。
//格式打印，
单体模式设计。向OUTPUT_STDOUT(和OUTPUT_STDERR同义)或OUTPUT_FILE或同时向OUTPUT_FILE和OUTPUT_STDOUT输出ANSI或Unicode编码文件。
使用方法：Logging* p = Logging::getInstance(Logging::OUTPUT_FILE);
////////////////////////////////////////////////////////////////////*/
#ifndef LOGGING_H
#define LOGGING_H
#include "newbeeUtils/exports.h"
//#include <string.h>	//strlen等
#include <stdio.h>
#include <memory.h> //memset等
#include <time.h>
#ifdef UNIX
	#include <stdarg.h>
#else
	#include <STDARG.H> //变长函数头文件
#endif

#define  INDENT_STRING "    |"
#define  INDENT_STRING1 "    |"
#define  INDENT_STRING2 "    |    |"
#define  INDENT_STRING3 "    |    |    |"
//#define  INDENT_ORNAMENT_CHAR '|'

//#define ERRORLOG(a) errorlogs("%s: %s,%d,%s", typeid(*this).name(), (a), __LINE__, __FILE__)

namespace NEWBEE_UTILS{
class NBLock;
class LoggingAll;
class DLL_EXPORT Logging
{
	friend class LoggingAll;
public:
	enum MODE_SEGMENT{ // 日志分段输出
		MS_NONE, // 无分段（默认）
		MS_HOUR, // 每小时输出
		MS_DAY,	 // 每天输出
		MS_100M,	 // 限定大小
	};
	enum MODE_THREAD_SAFE{ // 线程安全：是否支持多线程输出
		MTS_NONE,	// 无分段
		MTS_SINGLE,  // 单线程，无锁，不支持多线程
		MTS_MULTI,	// 支持多线程（默认），有锁。
	};
	enum LOG_ORNAMENT{
		ORNAMENT_NONE = 0x00, //0: 无任何装饰
		ORNAMENT_INDENT = 0x01, //1: 缩进
		ORNAMENT_TAG = 0x02, //2. 标记
		ORNAMENT_TIME = 0x04, //3. 时间
		ORNAMENT_THREAD_INFO = 0x08, //4. 线程标识
		ORNAMENT_SEPARATOR= 0x10, //4. 结尾字符
		ORNAMENT_PREFIX= ORNAMENT_TIME|ORNAMENT_THREAD_INFO|ORNAMENT_TAG|ORNAMENT_INDENT, //所有前缀
		ORNAMENT_ALL= ORNAMENT_SEPARATOR|ORNAMENT_PREFIX, //4. 所有装饰
	};
	enum LOG_OUTPUT{
		OUTPUT_STDOUT = 0x0001,
		OUTPUT_AUTOFILE = 0x0010,
		OUTPUT_FILE = 0x0100,//返回一个对指定文件输出的日志指针，但该文件只在第一次调用getInstance()时指定名字，其后再有指定的名字将被忽略。
	};
	enum LogLeveL{
		LOG_LEVEL_NONE = 0x0,
		LOG_LEVEL_ERROR = 0x1,
		LOG_LEVEL_WARN = 0x2,
		LOG_LEVEL_INFO = 0x4,
		LOG_LEVEL_DEBUG = 0x8,
		LOG_LEVEL_ALL = 0xF,
	};
	//这个模式目前对标准输出没有影响。
	enum BUFFERING_MODE{
		BUFFERING_NONE, //无缓冲模式输出，注：这个功能可以用于初期调试，他保证即使我们只打印一个字节，
		//都会立即在文件中看到。但是他极大地影响性能！！！
		BUFFERING_SPECIFIED, //指定buffer的大小。
		BUFFERING_AUTO,//程序不管由系统自行决定。
	};
	//对于重名文件是追加还是重写
	enum REWRITE_MODE{
		REWRITE_NONE,
		REWRITE_OVERWRITE, // 覆盖
		REWRITE_APPEND, // 追加
	};
	BUFFERING_MODE getBufferingMode(){
		return m_bufferMode;
	}
	void setPrintLevel(int level){
		m_detailLevel = level;
	}
	int getPrintLevel(){
		return m_detailLevel;
	}
	void limitPrintLevel2(LogLeveL level)
	{//限定打印的级别到这个级别为止。
		m_detailLevel =  (level<<1) -1;
	}

	char setSeparator(char c)
	{
		char old =m_separator;
		m_separator = c;
		return old;
	}
	char getSeparator()
	{
		return m_separator;
	}
	FILE* getfp();
	void release();
	void printlog(LogLeveL printLevel, int format){
		int old = setFormat(format);
		printlog(printLevel);
		setFormat(old);
	}
	void printlog(LogLeveL printLevel, int format, const char* prefix, long num){
		int old = setFormat(format);
		printlog(printLevel, prefix, num);
		setFormat(old);

	}
	void printlog(LogLeveL printLevel, int format, const char* prefix, unsigned long num){
		int old = setFormat(format);
		printlog(printLevel, prefix, num);
		setFormat(old);

	}
	void printlog(LogLeveL printLevel, int format, const char* prefix, int num){
		int old = setFormat(format);
		printlog(printLevel, prefix, num);
		setFormat(old);

	}
	void printlog(LogLeveL printLevel, int format, const char* prefix, double num){
		int old = setFormat(format);
		printlog(printLevel, prefix, num);
		setFormat(old);

	}
	void printlog(LogLeveL printLevel, int format, const char* str)
	{
		int old = setFormat(format);
		printlog(printLevel, str);
		setFormat(old);
	}
	void printlog(LogLeveL printLevel, int format, const char* prefix, char* str, int length){
		int old = setFormat(format);
		printlog(printLevel, prefix, str, length);
		setFormat(old);
	}
	void printlog(LogLeveL printLevel, int format, const char* prefix, void * pVoid){
		int old = setFormat(format);
		printlog(printLevel, prefix, pVoid);
		setFormat(old);
	} 
	void printlog(LogLeveL printLevel, int format, const char* prefix, const char* str){
		int old = setFormat(format);
		printlog(printLevel, prefix, str);
		setFormat(old);
	}
	void printlog(LogLeveL printLevel, int format, const char* prefix, unsigned char* str,int length){
		int old = setFormat(format);
		printlog(printLevel, prefix, str, length);
		setFormat(old);
	}
	void printlog(LogLeveL printLevel, int format, const char* prefix, const void * pVoid, long length){
		int old = setFormat(format);
		printlog(printLevel, prefix, pVoid, length);
		setFormat(old);
	}
	void printlog(LogLeveL printLevel, int format, const char* prefix, void ** ppVoid){
		int old = setFormat(format);
		printlog(printLevel, prefix, ppVoid);
		setFormat(old);
	}
	void printlog(LogLeveL printLevel, int format, const char c)
	{
		int old = setFormat(format);
		printlog(printLevel, c);
		setFormat(old);
	}
	void printlogs(LogLeveL printLevel, int format, const char *str, ...){
		int old = setFormat(format);
		if(willPrint(printLevel)){
			printAhead(printLevel);
			va_list argp; 
			va_start(argp, str); /* 将可变长参数转换为va_list */ 
			justPrints(str,argp);
			va_end (argp);
			printAfter();
		}
		setFormat(old);
	}
///////////////////////////////////////////
	void printlog(LogLeveL printLevel){
		if(willPrint(printLevel)){
			printAhead(printLevel);
			printAfter();
		}
	}
	void printlog(LogLeveL printLevel, const char* prefix, long num){
		if(willPrint(printLevel)){
			printAhead(printLevel);
			justPrint(prefix, num);
			printAfter();
		}
	}
	void printlog(LogLeveL printLevel, const char* prefix, unsigned long num){
		if(willPrint(printLevel)){
			printAhead(printLevel);
			justPrint(prefix, num);
			printAfter();
		}
	}
	void printlog(LogLeveL printLevel, const char* prefix, int num){
		if(willPrint(printLevel)){
			printAhead(printLevel);
			justPrint(prefix, static_cast<long>(num));
			printAfter();
		}
	}
	void printlog(LogLeveL printLevel, const char* prefix, double num){
		if(willPrint(printLevel)){
			printAhead(printLevel);
			justPrint(prefix, num);
			printAfter();
		}
	}
	void printlog(LogLeveL printLevel, const char* str)
	{
		if(willPrint(printLevel)){
			printAhead(printLevel);
			justPrint(str);
			printAfter();
		}
	}
	void printlog(LogLeveL printLevel, const char* prefix, char* str, int length){
		if(willPrint(printLevel)){
			printAhead(printLevel);
			justPrint(prefix, str, length);
			printAfter();
		}
	}
	void printlog(LogLeveL printLevel, const char* prefix, void * pVoid){
		if(willPrint(printLevel)){
			printAhead(printLevel);
			justPrint(prefix, pVoid);
			printAfter();
		}
	} 
	void printlog(LogLeveL printLevel, const char* prefix, const char* str){
		if(willPrint(printLevel)){
			printAhead(printLevel);
			justPrint(prefix,str);
			printAfter();
		}
	}
	void printlog(LogLeveL printLevel, const char* prefix, unsigned char* str,int length)
	{
		if(willPrint(printLevel)){
			printAhead(printLevel);
			justPrint(prefix, str,length);
			printAfter();
		}
	}
	void printlog(LogLeveL printLevel, const char* prefix, const void * pVoid, long length){
		if(willPrint(printLevel)){
			printAhead(printLevel);
			justPrint(prefix, pVoid, length);
			printAfter();
		}
	}
	void printlog(LogLeveL printLevel, const char* prefix, void ** ppVoid){
		if(willPrint(printLevel)){
			printAhead(printLevel);
			justPrint(prefix, ppVoid);
			printAfter();
		}
	}
	void printlog(LogLeveL printLevel, const char c)
	{
		if(willPrint(printLevel)){
			printAhead(printLevel);
			justPrint(c);
			printAfter();
		}
	}
	void printlogs(LogLeveL printLevel, const char *str, ...){
		if(willPrint(printLevel)){
			printAhead(printLevel);
			va_list argp; 
			va_start(argp, str); /* 将可变长参数转换为va_list */ 
			justPrints(str,argp);
			va_end (argp);
			printAfter();
		}
	}
	void debuglog(){
		if(willPrint(LOG_LEVEL_DEBUG)){
			printAhead(LOG_LEVEL_DEBUG);
			printAfter();
		}
	}
	void debuglog(const char* prefix, long num)
	{
		if(willPrint(LOG_LEVEL_DEBUG)){
			printAhead(LOG_LEVEL_DEBUG);
			justPrint(prefix, num);
			printAfter();
		}
	}
	void debuglog(const char* prefix, unsigned long num)
	{
		if(willPrint(LOG_LEVEL_DEBUG)){
			printAhead(LOG_LEVEL_DEBUG);
			justPrint(prefix, num);
			printAfter();
		}
	}
	void debuglog(const char* prefix, int num)
	{
		if(willPrint(LOG_LEVEL_DEBUG)){
			printAhead(LOG_LEVEL_DEBUG);
			justPrint(prefix, static_cast<long>(num));
			printAfter();
		}
	}
	void debuglog(const char* prefix, double num)
	{
		if(willPrint(LOG_LEVEL_DEBUG)){
			printAhead(LOG_LEVEL_DEBUG);
			justPrint(prefix, num);
			printAfter();
		}
	}
	void debuglog(const char* str)
	{
		if(willPrint(LOG_LEVEL_DEBUG)){
			printAhead(LOG_LEVEL_DEBUG);
			justPrint(str);
			printAfter();
		}
	}
	void debuglog(const char* prefix, char* str, int length){
		if(willPrint(LOG_LEVEL_DEBUG)){
			printAhead(LOG_LEVEL_DEBUG);
			justPrint(prefix, str, length);
			printAfter();
		}
	}
	void debuglog(const char* prefix, const char* str){
		if(willPrint(LOG_LEVEL_DEBUG)){
			printAhead(LOG_LEVEL_DEBUG);
			justPrint(prefix,str);
			printAfter();
		}
	}
	void debuglog(const char* prefix, unsigned char* str,int length)
	{
		if(willPrint(LOG_LEVEL_DEBUG)){
			printAhead(LOG_LEVEL_DEBUG);
			justPrint(prefix, str,length);
			printAfter();
		}
	}
	void debuglog(const char* prefix, void * pVoid){
		if(willPrint(LOG_LEVEL_DEBUG)){
			printAhead(LOG_LEVEL_DEBUG);
			justPrint(prefix, pVoid);
			printAfter();
		}
	}
	void debuglog(const char* prefix, const void * pVoid, long length){
		if(willPrint(LOG_LEVEL_DEBUG)){
			printAhead(LOG_LEVEL_DEBUG);
			justPrint(prefix, pVoid, length);
			printAfter();
		}
	}

	void debuglog(const char* prefix, void ** ppVoid){
		if(willPrint(LOG_LEVEL_DEBUG)){
			printAhead(LOG_LEVEL_DEBUG);
			justPrint(prefix, ppVoid);
			printAfter();
		}
	}
	void debuglog(const char c)
	{
		if(willPrint(LOG_LEVEL_DEBUG)){
			printAhead(LOG_LEVEL_DEBUG);
			justPrint(c);
			printAfter();
		}
	}
	void debuglogs(const char* str,...){
		if(willPrint(LOG_LEVEL_DEBUG)){
			printAhead(LOG_LEVEL_DEBUG);
			va_list argp; 
			va_start(argp, str); /* 将可变长参数转换为va_list */ 
			justPrints(str,argp);
			va_end (argp);
			printAfter();
		}
	}
	void debuglog(LOG_ORNAMENT orn){
		if(willPrint(LOG_LEVEL_DEBUG))
		{
			switch(orn)
			{
			case ORNAMENT_INDENT:
				justPrintPrefixIndent();
				break;
			case ORNAMENT_TAG:
				justPrintPrefixTag(LOG_LEVEL_INFO);
				break;
			case ORNAMENT_TIME:
				justPrintPrefixTime();
				break;
			case ORNAMENT_THREAD_INFO:
				justPrintPrefixThreadInfo();
				break;
			case ORNAMENT_SEPARATOR:
				justPrintPrefixSeparator();
				break;
			case ORNAMENT_PREFIX:
				break;
			default://ORNAMENT_ALL
				break;
			}
		}
	}
	void infolog(){
		if(willPrint(LOG_LEVEL_INFO))
		{
			printAhead(LOG_LEVEL_INFO);
			printAfter();
		}
	}
	void infolog(LOG_ORNAMENT orn){
		if(willPrint(LOG_LEVEL_INFO))
		{
			switch(orn)
			{
			case ORNAMENT_INDENT:
				justPrintPrefixIndent();
				break;
			case ORNAMENT_TAG:
				justPrintPrefixTag(LOG_LEVEL_INFO);
				break;
			case ORNAMENT_TIME:
				justPrintPrefixTime();
				break;
			case ORNAMENT_THREAD_INFO:
				justPrintPrefixThreadInfo();
				break;
			case ORNAMENT_SEPARATOR:
				justPrintPrefixSeparator();
				break;
			case ORNAMENT_PREFIX:
				break;
			default://ORNAMENT_ALL
			    break;
			}
		}
	}
	void infolog(const char* prefix, long num){
		if(willPrint(LOG_LEVEL_INFO)){
			printAhead(LOG_LEVEL_INFO);
			justPrint(prefix, num);
			printAfter();
		}
	}
	void infolog(const char* prefix, unsigned long num){
		if(willPrint(LOG_LEVEL_INFO)){
			printAhead(LOG_LEVEL_INFO);
			justPrint(prefix, num);
			printAfter();
		}
	}
	void infolog(const char* prefix, int num){
		if(willPrint(LOG_LEVEL_INFO)){
			printAhead(LOG_LEVEL_INFO);
			justPrint(prefix, static_cast<long>(num));
			printAfter();
		}
	}
	void infolog(const char* prefix, double num){
		if(willPrint(LOG_LEVEL_INFO)){
			printAhead(LOG_LEVEL_INFO);
			justPrint(prefix, num);
			printAfter();
		}
	}
	void infolog(const char* str){
		if(willPrint(LOG_LEVEL_INFO)){
			printAhead(LOG_LEVEL_INFO);
			justPrint(str);
			printAfter();
		}
	}
	void infolog(const char* prefix, char* str, int length){
		if(willPrint(LOG_LEVEL_INFO)){
			printAhead(LOG_LEVEL_INFO);
			justPrint(prefix, str, length);
			printAfter();
		}
	}
	void infolog(const char* prefix, const char* str){
		if(willPrint(LOG_LEVEL_INFO)){
			printAhead(LOG_LEVEL_INFO);
			justPrint(prefix,str);
			printAfter();
		}
	}
	void infolog(const char* prefix, unsigned char* str,int length){
		if(willPrint(LOG_LEVEL_INFO)){
			printAhead(LOG_LEVEL_INFO);
			justPrint(prefix, str, length);
			printAfter();
		}
	}
	void infolog(const char* prefix, void * pVoid){
		if(willPrint(LOG_LEVEL_INFO)){
			printAhead(LOG_LEVEL_INFO);
			justPrint(prefix, pVoid);
			printAfter();
		}
	} 
	void infolog(const char* prefix, const void * pVoid, long length){
		if(willPrint(LOG_LEVEL_INFO)){
			printAhead(LOG_LEVEL_INFO);
			justPrint(prefix, pVoid, length);
			printAfter();
		}
	}
	void infolog(const char* prefix, void ** ppVoid){
		if(willPrint(LOG_LEVEL_INFO)){
			printAhead(LOG_LEVEL_INFO);
			justPrint(prefix, ppVoid);
			printAfter();
		}
	}
	void infolog(const char c){
		if(willPrint(LOG_LEVEL_INFO)){
			printAhead(LOG_LEVEL_INFO);
			justPrint(c);
			printAfter();
		}
	}
	void infologs(const char* str,...){
		if(willPrint(LOG_LEVEL_INFO)){
			printAhead(LOG_LEVEL_INFO);
			va_list argp; 
			va_start(argp, str); /* 将可变长参数转换为va_list */ 
			justPrints(str,argp);
			va_end (argp);
			printAfter();
		}
	}
	void errorlog(){//只打印装饰信息
		if(willPrint(LOG_LEVEL_ERROR)){
			printAhead(LOG_LEVEL_ERROR);
			printAfter();
		}
	}
	void errorlog(const char* prefix, long num){
		if(willPrint(LOG_LEVEL_ERROR)){
			printAhead(LOG_LEVEL_ERROR);
			justPrint(prefix, num);
			printAfter();
		}
	}
	void errorlog(const char* prefix, unsigned long num){
		if(willPrint(LOG_LEVEL_ERROR)){
			printAhead(LOG_LEVEL_ERROR);
			justPrint(prefix, num);
			printAfter();
		}
	}
	void errorlog(const char* str){
		if(willPrint(LOG_LEVEL_ERROR)){
			printAhead(LOG_LEVEL_ERROR);
			justPrint(str);printAfter();
		}
	}
	void errorlog(const char* prefix, char* str, int length){
		if(willPrint(LOG_LEVEL_ERROR)){
			printAhead(LOG_LEVEL_ERROR);
			justPrint(prefix, str, length);
			printAfter();
		}
	}
	void errorlog(const char* prefix, const char* str){
		if(willPrint(LOG_LEVEL_ERROR)){
			printAhead(LOG_LEVEL_ERROR);
			justPrint(prefix,str);
			printAfter();
		}
	}
	void errorlog(const char* prefix, unsigned char* str,int length){
		if(willPrint(LOG_LEVEL_ERROR)){
			printAhead(LOG_LEVEL_ERROR);
			justPrint(prefix, str,length);
			printAfter();
		}
	}
	void errorlog(const char* prefix, void * pVoid){if(willPrint(LOG_LEVEL_ERROR)){printAhead(LOG_LEVEL_ERROR);justPrint(prefix, pVoid);printAfter();}} 
	void errorlog(const char* prefix, const void * pVoid, long length){if(willPrint(LOG_LEVEL_ERROR)){printAhead(LOG_LEVEL_ERROR);justPrint(prefix, pVoid, length);printAfter();}}
	void errorlog(const char* prefix, void ** ppVoid){if(willPrint(LOG_LEVEL_ERROR)){printAhead(LOG_LEVEL_ERROR);justPrint(prefix, ppVoid);printAfter();}}
	void errorlog(const char c){if(willPrint(LOG_LEVEL_ERROR)){printAhead(LOG_LEVEL_ERROR);justPrint(c);printAfter();}}
	void errorlogs(const char* str,...){
		if(willPrint(LOG_LEVEL_ERROR)){
			printAhead(LOG_LEVEL_ERROR);
			va_list argp; 
			va_start(argp, str); /* 将可变长参数转换为va_list */ 
			justPrints(str,argp);
			va_end (argp);
			printAfter();
		}
	}
	void errorlog(LOG_ORNAMENT orn){
		if(willPrint(LOG_LEVEL_ERROR))
		{
			switch(orn)
			{
			case ORNAMENT_INDENT:
				justPrintPrefixIndent();
				break;
			case ORNAMENT_TAG:
				justPrintPrefixTag(LOG_LEVEL_INFO);
				break;
			case ORNAMENT_TIME:
				justPrintPrefixTime();
				break;
			case ORNAMENT_THREAD_INFO:
				justPrintPrefixThreadInfo();
				break;
			case ORNAMENT_SEPARATOR:
				justPrintPrefixSeparator();
				break;
			case ORNAMENT_PREFIX:
				break;
			default://ORNAMENT_ALL
				break;
			}
		}
	}
	void warnlog(){
		if(willPrint(LOG_LEVEL_WARN)){
			printAhead(LOG_LEVEL_WARN);
			printAfter();
		}
	}
	void warnlog(const char* prefix, long num){
		if(willPrint(LOG_LEVEL_WARN)){
			printAhead(LOG_LEVEL_WARN);
			justPrint(prefix, num);
			printAfter();
		}
	}
	void warnlog(const char* prefix, unsigned long num){
		if(willPrint(LOG_LEVEL_WARN)){
			printAhead(LOG_LEVEL_WARN);
			justPrint(prefix, num);
			printAfter();
		}
	}
	void warnlog(const char* str){
		if(willPrint(LOG_LEVEL_WARN)){
			printAhead(LOG_LEVEL_WARN);
			justPrint(str);
			printAfter();
		}
	}
	void warnlog(const char* prefix, char* str, int length){if(willPrint(LOG_LEVEL_WARN)){printAhead(LOG_LEVEL_WARN);justPrint(prefix, str, length);printAfter();}}
	void warnlog(const char* prefix, const char* str){if(willPrint(LOG_LEVEL_WARN)){printAhead(LOG_LEVEL_WARN);justPrint(prefix,str);printAfter();}}
	void warnlog(const char* prefix, unsigned char* str,int length){if(willPrint(LOG_LEVEL_WARN)){printAhead(LOG_LEVEL_WARN);justPrint(prefix, str,length);printAfter();}}
	void warnlog(const char* prefix, void * pVoid){if(willPrint(LOG_LEVEL_WARN)){printAhead(LOG_LEVEL_WARN);justPrint(prefix, pVoid);printAfter();}} 
	void warnlog(const char* prefix, const void * pVoid, long length){if(willPrint(LOG_LEVEL_WARN)){printAhead(LOG_LEVEL_WARN);justPrint(prefix, pVoid, length);printAfter();}}
	void warnlog(const char* prefix, void ** ppVoid){
		if(willPrint(LOG_LEVEL_WARN)){
			printAhead(LOG_LEVEL_WARN);
			justPrint(prefix, ppVoid);
			printAfter();
		}
	}
	void warnlog(const char c){
		if(willPrint(LOG_LEVEL_WARN)){
			printAhead(LOG_LEVEL_WARN);
			justPrint(c);
			printAfter();
		}
	}
	void warnlogs(const char* str,...)
	{
		if(willPrint(LOG_LEVEL_WARN)){
		printAhead(LOG_LEVEL_WARN);
		va_list argp; 
		va_start(argp, str); /* 将可变长参数转换为va_list */ 
		justPrints(str,argp);
		va_end (argp);
		printAfter();
		}
	}
	void warnlog(LOG_ORNAMENT orn){
		if(willPrint(LOG_LEVEL_WARN))
		{
			switch(orn)
			{
			case ORNAMENT_INDENT:
				justPrintPrefixIndent();
				break;
			case ORNAMENT_TAG:
				justPrintPrefixTag(LOG_LEVEL_INFO);
				break;
			case ORNAMENT_TIME:
				justPrintPrefixTime();
				break;
			case ORNAMENT_THREAD_INFO:
				justPrintPrefixThreadInfo();
				break;
			case ORNAMENT_SEPARATOR:
				justPrintPrefixSeparator();
				break;
			case ORNAMENT_PREFIX:
				break;
			default://ORNAMENT_ALL
				break;
			}
		}
	}
	inline short setIndentCounter(short counter){
		short temp = m_indent_counter;
		m_indent_counter = counter;
		return temp;
	}
	inline short getIndentCounter()
	{
		return m_indent_counter;
	}
	inline short plusIndentCounter(int times = 1){
		short temp = m_indent_counter;
		m_indent_counter += times;
		return temp;
	}
	inline short reduceIndentCounter(int times = 1){
		short temp = m_indent_counter;
		m_indent_counter-= times;
		return temp;
	}
	void enableTagPrint(){
		m_isTagPrinted = true;
	}
	void disableTagPrint(){
		m_isTagPrinted = false;
	}
	void enableTimePrint(){
		m_isTimePrinted =true;
	}
	void disableTimePrint(){
		m_isTimePrinted =false;
	}
	void enableThreadInfoPrint(){
		m_isThreadInfoPrinted =true;
	}
	void disableThreadInfoPrint(){
		m_isThreadInfoPrinted =false;
	}
	static Logging *createLog2file(const char *filename, BUFFERING_MODE mode = BUFFERING_AUTO, REWRITE_MODE rewriteMode = REWRITE_APPEND, MODE_SEGMENT segmentMode = MS_NONE, MODE_THREAD_SAFE tsMode = MTS_MULTI);
	static void releaseLog2file(Logging *log);
	static Logging* getInstance(int sort = OUTPUT_STDOUT, const char* filename = 0, BUFFERING_MODE mode = BUFFERING_AUTO, REWRITE_MODE rewriteMode = REWRITE_OVERWRITE, MODE_SEGMENT segmentMode = MS_NONE, MODE_THREAD_SAFE tsMode = MTS_MULTI);
	virtual inline int switchFile_for_segment(time_t now){
		return 0;
	}
	virtual inline int switchFile(const char* fileName){
		return 0;
	}
	virtual const char* getCurFileName()
	{
		return "stdout";
	}
	int removeOrnament(int format);//去除掉某种装饰，如果有。
	int addOrnament(int format);//添加某种装饰，如果没有。
	int setFormat(int format);//设置某种装饰格式。参数 format 中包含的装饰标签，该Log对象都打印。
	int antiFormat(int format);//反向设置某种装饰格式。参数 format 中包含的装饰标签，该Log对象都不再打印。
	int getFormat(){
		return m_isTimePrinted * ORNAMENT_TIME + m_isThreadInfoPrinted * ORNAMENT_THREAD_INFO + m_isIndentPrinted * ORNAMENT_INDENT + m_isTagPrinted * ORNAMENT_TAG + m_isSeparatorPrinted * ORNAMENT_SEPARATOR;
	}
	void enableIndent(){
		m_isIndentPrinted = true;
	}
	void disableIndent(){
		m_isIndentPrinted = false;
	}
	virtual void clear();
	
protected:
	void printAhead(int tag);
	void printAfter();/*{
		printPrefixSeparator();
	}	*/
	//virtual void println();
	virtual void println(const char*);
	virtual void justPrint(const char*, const long);
	virtual void justPrint(const char*, const unsigned long);
	virtual void justPrint(const char* prefix, const double num);
	virtual void justPrint(const char*, const char*, int length);
	virtual void justPrint(const char*, const char*);
	virtual void justPrint(const char*, const unsigned char*,int length);
	virtual void justPrint(const char*, const void *);//打印指针本身的值，就是把void*类型转换成长整形十六进制数%lx打印出来。	
	virtual void justPrint(const char*, const void *, long length);//打印指针所指内存区域的值，长度由length指定。阅读方法就是从左往右，地址由低到高——打印内存信息的正确不二人选！！！
	virtual void justPrint(const char*, const void **);
	virtual void justPrint(const char c)
	{
		fprintf(m_fp, "%c",c);
	}
	virtual void justPrint(const char* str)
	{
		fprintf(m_fp, "%s", str);
	}
	virtual void justPrints(const char*, va_list argp);
////////////////////打印各种标志位//////////////////////////////////////////////////////
	inline void printPrefixIndent(){
		if (m_isIndentPrinted){
			justPrintPrefixIndent();
		}
	}
	inline void justPrintPrefixIndent(){
		switch(m_indent_counter)
		{
		case 0:
			break;
		case 1:
			justPrint(INDENT_STRING1);
			break;
		case 2:
			justPrint(INDENT_STRING2);
			break;
		case 3:
			justPrint(INDENT_STRING3);
			break;
		default:
			for(int i = 0; i<m_indent_counter; i++){
				justPrint(INDENT_STRING);
			}
			break;
		}
	}
	inline void justPrintPrefixTag( int tag ){
		justPrint(m_detailLevelStringArray[tag]);
	}
	void printPrefixTag( int tag ){
		if (m_isTagPrinted)
		{
			justPrint(m_detailLevelStringArray[tag]);
		}
	}
	inline void justPrintPrefixSeparator(){
		justPrint(m_separator);
	}
	inline void printPrefixSeparator(){
		if (m_isSeparatorPrinted){
			justPrint(m_separator);
		}
	}
	void printPrefixTime(time_t now);
	void printPrefixThreadInfo();
	void justPrintPrefixTime();
	void justPrintPrefixThreadInfo();
//////////////////////////////////////////////////////////////////////////
	int willPrint(int tag){
		return m_detailLevel&tag;
	}
	virtual ~Logging();
	void setBufferingMode(BUFFERING_MODE mode);
	void setRewriteMode(REWRITE_MODE mode);
	void setMode_threadSafe(MODE_THREAD_SAFE mode);
	void setMode_segment(MODE_SEGMENT mode);
	int initLock(MODE_THREAD_SAFE mode);
	virtual int lock();
	virtual int unlock();
	Logging(FILE *fp){
		Logging_();
		m_fp = fp;
	}
	void Logging__();
	void Logging_();
	bool m_isIndentPrinted;//

	bool m_isTagPrinted;
	bool m_isSeparatorPrinted;
	bool m_isTimePrinted;
	bool m_isThreadInfoPrinted;
	char m_separator;
	int m_indent_counter;
	FILE *m_fp;
	const char* m_detailLevelStringArray[LOG_LEVEL_ALL+1];
	int m_detailLevel;
	BUFFERING_MODE m_bufferMode;
	REWRITE_MODE m_rewriteMode;
	MODE_SEGMENT m_segmentMode;
	MODE_THREAD_SAFE m_threadSafeMode;
	NBLock *m_lock;
private:
	static Logging* m_this;
};

}
#endif // !defined(LOGGING_H)




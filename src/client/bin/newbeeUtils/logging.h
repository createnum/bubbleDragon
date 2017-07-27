/* Logging.h: ��ӡ��־���ߣ���ӡʱ����������趨��ӡ����׼���������������������ļ���
//��ʽ��ӡ��
����ģʽ��ơ���OUTPUT_STDOUT(��OUTPUT_STDERRͬ��)��OUTPUT_FILE��ͬʱ��OUTPUT_FILE��OUTPUT_STDOUT���ANSI��Unicode�����ļ���
ʹ�÷�����Logging* p = Logging::getInstance(Logging::OUTPUT_FILE);
////////////////////////////////////////////////////////////////////*/
#ifndef LOGGING_H
#define LOGGING_H
#include "newbeeUtils/exports.h"
//#include <string.h>	//strlen��
#include <stdio.h>
#include <memory.h> //memset��
#include <time.h>
#ifdef UNIX
	#include <stdarg.h>
#else
	#include <STDARG.H> //�䳤����ͷ�ļ�
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
	enum MODE_SEGMENT{ // ��־�ֶ����
		MS_NONE, // �޷ֶΣ�Ĭ�ϣ�
		MS_HOUR, // ÿСʱ���
		MS_DAY,	 // ÿ�����
		MS_100M,	 // �޶���С
	};
	enum MODE_THREAD_SAFE{ // �̰߳�ȫ���Ƿ�֧�ֶ��߳����
		MTS_NONE,	// �޷ֶ�
		MTS_SINGLE,  // ���̣߳���������֧�ֶ��߳�
		MTS_MULTI,	// ֧�ֶ��̣߳�Ĭ�ϣ���������
	};
	enum LOG_ORNAMENT{
		ORNAMENT_NONE = 0x00, //0: ���κ�װ��
		ORNAMENT_INDENT = 0x01, //1: ����
		ORNAMENT_TAG = 0x02, //2. ���
		ORNAMENT_TIME = 0x04, //3. ʱ��
		ORNAMENT_THREAD_INFO = 0x08, //4. �̱߳�ʶ
		ORNAMENT_SEPARATOR= 0x10, //4. ��β�ַ�
		ORNAMENT_PREFIX= ORNAMENT_TIME|ORNAMENT_THREAD_INFO|ORNAMENT_TAG|ORNAMENT_INDENT, //����ǰ׺
		ORNAMENT_ALL= ORNAMENT_SEPARATOR|ORNAMENT_PREFIX, //4. ����װ��
	};
	enum LOG_OUTPUT{
		OUTPUT_STDOUT = 0x0001,
		OUTPUT_AUTOFILE = 0x0010,
		OUTPUT_FILE = 0x0100,//����һ����ָ���ļ��������־ָ�룬�����ļ�ֻ�ڵ�һ�ε���getInstance()ʱָ�����֣��������ָ�������ֽ������ԡ�
	};
	enum LogLeveL{
		LOG_LEVEL_NONE = 0x0,
		LOG_LEVEL_ERROR = 0x1,
		LOG_LEVEL_WARN = 0x2,
		LOG_LEVEL_INFO = 0x4,
		LOG_LEVEL_DEBUG = 0x8,
		LOG_LEVEL_ALL = 0xF,
	};
	//���ģʽĿǰ�Ա�׼���û��Ӱ�졣
	enum BUFFERING_MODE{
		BUFFERING_NONE, //�޻���ģʽ�����ע��������ܿ������ڳ��ڵ��ԣ�����֤��ʹ����ֻ��ӡһ���ֽڣ�
		//�����������ļ��п����������������Ӱ�����ܣ�����
		BUFFERING_SPECIFIED, //ָ��buffer�Ĵ�С��
		BUFFERING_AUTO,//���򲻹���ϵͳ���о�����
	};
	//���������ļ���׷�ӻ�����д
	enum REWRITE_MODE{
		REWRITE_NONE,
		REWRITE_OVERWRITE, // ����
		REWRITE_APPEND, // ׷��
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
	{//�޶���ӡ�ļ����������Ϊֹ��
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
			va_start(argp, str); /* ���ɱ䳤����ת��Ϊva_list */ 
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
			va_start(argp, str); /* ���ɱ䳤����ת��Ϊva_list */ 
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
			va_start(argp, str); /* ���ɱ䳤����ת��Ϊva_list */ 
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
			va_start(argp, str); /* ���ɱ䳤����ת��Ϊva_list */ 
			justPrints(str,argp);
			va_end (argp);
			printAfter();
		}
	}
	void errorlog(){//ֻ��ӡװ����Ϣ
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
			va_start(argp, str); /* ���ɱ䳤����ת��Ϊva_list */ 
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
		va_start(argp, str); /* ���ɱ䳤����ת��Ϊva_list */ 
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
	int removeOrnament(int format);//ȥ����ĳ��װ�Σ�����С�
	int addOrnament(int format);//���ĳ��װ�Σ����û�С�
	int setFormat(int format);//����ĳ��װ�θ�ʽ������ format �а�����װ�α�ǩ����Log���󶼴�ӡ��
	int antiFormat(int format);//��������ĳ��װ�θ�ʽ������ format �а�����װ�α�ǩ����Log���󶼲��ٴ�ӡ��
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
	virtual void justPrint(const char*, const void *);//��ӡָ�뱾���ֵ�����ǰ�void*����ת���ɳ�����ʮ��������%lx��ӡ������	
	virtual void justPrint(const char*, const void *, long length);//��ӡָ����ָ�ڴ������ֵ��������lengthָ�����Ķ��������Ǵ������ң���ַ�ɵ͵��ߡ�����ӡ�ڴ���Ϣ����ȷ������ѡ������
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
////////////////////��ӡ���ֱ�־λ//////////////////////////////////////////////////////
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




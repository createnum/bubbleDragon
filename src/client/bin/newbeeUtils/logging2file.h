//////////////////////////////////////////////////////////////////////////
//���壬�����ϵͳʱ���Զ����ɵ��ļ��������¼���ࡣ
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
	static const int MAX_FILE_PATH_LEN = 128; //����������ļ��������ضϡ�
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
	//ȫ�����麯������ʵ���Լ��Ĺ���
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
	//virtual void justPrint(const char*, const void *);//��ӡָ�뱾���ֵ�����ǰ�void*����ת���ɳ�����ʮ��������%lx��ӡ������
	//virtual void justPrint(const char*, const void *, long length);//��ӡָ����ָ�ڴ������ֵ��������lengthָ����
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
	int m_curHour, m_curDay; // Ϊ ������ļ���ӡ �� ��Сʱ���ļ���ӡ ׼��

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


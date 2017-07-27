// Time.h: interface for the Timing class.
//
//////////////////////////////////////////////////////////////////////

#ifndef NEWBEE_UTILS_TIME_H
#define NEWBEE_UTILS_TIME_H
#include "newbeeUtils/exports.h"
#include <time.h>

//#include "defOS.h"
#pragma warning (disable: 4996) // �ضϾ���
namespace NEWBEE_UTILS{
class NBUAPI Timing
{
public:
	static const int LEAST_LENGTH_TIME_STRING = 32;
	//static const int MAX_TIME_STRING_LEN = LEAST_LENGTH_TIME_STRING;
	static int getDay();
	static int getDay(time_t now);
	static int getMonth();
	static int getMonth(time_t now);
	static int getYear();
	static int getYear(time_t now);
	static int getHour();
	static int getHour(time_t now);
	//static const long LEAST_LENGTH_TIME_STRING;//ʱ���ַ������������̳��ȡ��������ɱ�֤���¡�
	//��������������Ϊ�˼���Ŀǰֻ����ʹ������Ϊconst�ˡ�
	static const int FORMAT_FORCE2HOUR = 2;//ǿ�ƾ�ȷ��Сʱ�����Է��Ӻ��룿
	static const int FORMAT_FORCE2MIN = 1;//ǿ�ƾ�ȷ�����ӣ������룿
	static const int FORMAT_FORCE2SEC = 0;//ǿ�ƾ�ȷ���룬����ռλ��
	// �ѷ���ת����Сʱ�ͷ��ӣ�������������û�����buffer�Ĵ�С��format����ǿ����ʾ��Сʱ�����Ƿ��ӣ�������
	static bool cvtMin2hour_min(time_t seconds, char* buffer, int bufferLength,  int format=FORMAT_FORCE2SEC);
	static bool cvtSec2hour_min_sec(time_t secs, char* buffer, int bufferLength, int format=FORMAT_FORCE2SEC);
	//str�������20���ֽڡ�
	static void getCurDigitalTimeString(char* str);
	const char* getCurDigitalTimeString();
	static void getTimeStr_literal_y_m_d(char* timeStr);
	static void getTimeStr_literal_y_m_d(char* timeStr, time_t now);
	static void getTimeStr_literal_y_m_d_h(char* timeStr);
	static void getTimeStr_literal_y_m_d_h(char* timeStr, time_t now);
	static void getTimeStr_literal_m_d_h_m_s_ms(char* str);
	static void getTimeStr_literal_m_d_h_m_s_ms(char* timeStr, time_t now);
	const char* getTimeStr_literal_m_d_h_m_s_ms();
	//�ɹ�����true,ʧ�ܷ���false
	static bool convert2ReadableTimeString(char* des, time_t t, int desLength);
	static bool convertTime2ReadableString(char* des, time_t t, int desLength);
	static bool convertTimeStringDigital2Readable(char* des, const char* src, int desLength);
	//static Timing* getInstance(int =0);
	void recordStart();
	clock_t recordEnd();
	void recordContinuousStart();
	clock_t recordContinuousEnd();
	clock_t getTimeLength();

	static inline time_t curTime(){
		time_t st;
		time(&st);
		return st;
	}
	Timing();
	~Timing();
protected:

	clock_t m_timeLength;//��ʱʱ����
	char m_curReadableTimeString[LEAST_LENGTH_TIME_STRING];
	char m_curDigitalTimeString[LEAST_LENGTH_TIME_STRING];
	clock_t m_timeStart;
	clock_t m_timeEnd;
	void resetCurTimeString(void);
};
//inline void Timing::curTime(time_t* curTime)
//{
//	time(curTime);
//}

inline void Timing::recordStart()
{
	m_timeStart = clock();
}
inline clock_t Timing::getTimeLength()
{
	return m_timeLength;
}
inline void Timing::recordContinuousStart()
{
	m_timeStart = clock();
}
inline clock_t Timing::recordContinuousEnd()
{
	return m_timeLength += clock() - m_timeStart;
}
inline clock_t Timing::recordEnd()
{
	m_timeEnd = clock();
	return m_timeEnd - m_timeStart;
}
inline void Timing::resetCurTimeString(void)
{
	for(int i = 1;i< LEAST_LENGTH_TIME_STRING -2; i++){
		m_curReadableTimeString[i] = ' ';
	}
}

}
#endif //NEWBEE_UTILS_TIME_H


// Time.h: interface for the Timing class.
//
//////////////////////////////////////////////////////////////////////

#ifndef NEWBEE_UTILS_TIME_H
#define NEWBEE_UTILS_TIME_H
#include "newbeeUtils/exports.h"
#include <time.h>

//#include "defOS.h"
#pragma warning (disable: 4996) // 截断警告
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
	//static const long LEAST_LENGTH_TIME_STRING;//时间字符串所必须的最短长度。长于它可保证无事。
	//以下三个变量，为了兼容目前只好忍痛不声明为const了。
	static const int FORMAT_FORCE2HOUR = 2;//强制精确到小时，忽略分钟和秒？
	static const int FORMAT_FORCE2MIN = 1;//强制精确到分钟，忽略秒？
	static const int FORMAT_FORCE2SEC = 0;//强制精确到秒，填零占位。
	// 把分钟转换成小时和分钟，这个函数会检测用户所开buffer的大小。format代表强制显示到小时，还是分钟，还是秒
	static bool cvtMin2hour_min(time_t seconds, char* buffer, int bufferLength,  int format=FORMAT_FORCE2SEC);
	static bool cvtSec2hour_min_sec(time_t secs, char* buffer, int bufferLength, int format=FORMAT_FORCE2SEC);
	//str必须大于20个字节。
	static void getCurDigitalTimeString(char* str);
	const char* getCurDigitalTimeString();
	static void getTimeStr_literal_y_m_d(char* timeStr);
	static void getTimeStr_literal_y_m_d(char* timeStr, time_t now);
	static void getTimeStr_literal_y_m_d_h(char* timeStr);
	static void getTimeStr_literal_y_m_d_h(char* timeStr, time_t now);
	static void getTimeStr_literal_m_d_h_m_s_ms(char* str);
	static void getTimeStr_literal_m_d_h_m_s_ms(char* timeStr, time_t now);
	const char* getTimeStr_literal_m_d_h_m_s_ms();
	//成功返回true,失败返回false
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

	clock_t m_timeLength;//计时时长。
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


#include "monitoringCenter.h"
//#include "alert.h"
typedef std::pair<std::string, int> TimerRecordMapEle;
typedef TimerRecordMap::iterator TimerRecordMapItor;
using namespace NEWBEE_UTILS;
MonitoringCenter *MonitoringCenter::instance = 0;
int MonitoringCenter::deleteTimerRecord(const char* className)
{
    NBSynGuard guard(&m_timerRecordLock);
    TimerRecordMapItor itor = m_timerRecords.find(className);
    if (m_timerRecords.end() == itor/* && 0 != strcmp(className, "class Monitoring Timer")*/){
       // return alert();
    }else{
        (itor->second)--;
        if(0 == itor->second){
            m_timerRecords.erase(className);
        }
    }
    return 0;
}
int MonitoringCenter::recordTimer(const char* className)
{
    NBSynGuard guard(&m_timerRecordLock);
    TimerRecordMapItor itor = m_timerRecords.find(className);
    if (m_timerRecords.end() != itor){ // 已有该类记录
        (itor->second)++;
        return 0;
    }
    std::pair<TimerRecordMapItor, bool> insertRet =  m_timerRecords.insert(TimerRecordMapEle(className, 1));
    if(!insertRet.second){
       // return alert();
    }
    return 0;
}
int MonitoringCenter::getTimerRecords(char* buf)
{
    NBSynGuard guard(&m_timerRecordLock);
    int len = 0;
    for( TimerRecordMapItor itor = m_timerRecords.begin(); m_timerRecords.end() != itor; itor++)
    {
        sprintf(buf+len, "%s-%d, ", itor->first.c_str(),  itor->second);
        len += strlen(buf+len);
    }
    return 0;
}

int MonitoringCenter::init()
{
    instance = this;
    m_timerRecordLock.init();
    return 0;
}
void MonitoringCenter::MonitoringCenter_()
{
    //reset (1) here

    //set (2) zero here
}
int MonitoringCenter::MonitoringCenter__()
{
    // release sys resource here
    m_timerRecordLock.release();
    // delete (2) here
    MonitoringCenter_();
    return 0;
}
int MonitoringCenter::clear()
{
    // call (3).clear() here
    // call (4)::clear();
    MonitoringCenter__();    
    return 0;
}

#ifndef GT2_MONITORINGCENTER_H
#define GT2_MONITORINGCENTER_H
#include <string>
#include <map>
#include "newbeeUtils/nbLock.h"
typedef std::map<std::string, int> TimerRecordMap;
class MonitoringCenter
{
public:
    int deleteTimerRecord(const char* className);
    int recordTimer(const char* className);
    int getTimerRecords(char* buf);
    int init();
    static MonitoringCenter* getInstance(){
        return instance;
    }
    MonitoringCenter(){
        MonitoringCenter_();
    }
    virtual ~MonitoringCenter(){
        MonitoringCenter__();
    }
    int clear();
protected: //simple object(including unliable pointer) - (1) liable pointer - (2) complex object - (3) father class - (4)
    void MonitoringCenter_();
    int MonitoringCenter__();
    static MonitoringCenter *instance;
    TimerRecordMap m_timerRecords;
    NEWBEE_UTILS::NBLock m_timerRecordLock;
}; // class MonitoringCenter
#endif //GT2_MONITORINGCENTER_H

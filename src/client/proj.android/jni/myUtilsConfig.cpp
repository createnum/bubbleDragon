#include "MyUtilsConfig.h"
#include "Java_tools_CommonFunc.h"
#include <android/log.h>

MyUtilsConfig* MyUtilsConfig::m_inst=0;

void MyUtilsConfig::init(){
    m_mainLogPath = getSdcardPathJni("skt.bubbleDragon/logs/main.log");
	m_userDefineStageXmlPath = getSdcardPathJni("skt.bubbleDragon/stage.xml");
    //m_mainLogPath = "mnt/sdcard-ext/skt.log";
    //m_timerLogPath = getSdcardPathJni("skt.bubbleDragon/logs/timerThread.log");
    //m_timerLogPath = "mnt/sdcard-ext/timeskt.log";
}

#include "MyUtilsConfig.h"

MyUtilsConfig* MyUtilsConfig::m_inst=0;

void MyUtilsConfig::init(){
    m_mainLogPath = "../logs/main.log";
	m_userDefineStageXmlPath = "../Resources/res/stage.xml";
}
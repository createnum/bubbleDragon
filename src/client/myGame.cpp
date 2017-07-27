#include "myGame.h"
#include "newbeeUtils/logging.h"
#include "myUtilsConfig.h"
#include "platform/CCCommon.h"
#include "SimpleAudioEngine.h"
#include <sstream>
#include <fstream>
#include "platform/CCFileUtils.h"
#include "md5.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "statisticsLog.h"
#include "stageMapDict.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h> 
#include "platform/android/jni/JniHelper.h" 
#include <android/log.h> 
#else 
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOSIAPPort.h"
#endif
#endif 
using namespace cocos2d;
using namespace gui;
using namespace NEWBEE_UTILS;
using namespace std;
using namespace CocosDenshion;

MyGame* MyGame::m_inst = 0;

static const int DEFAULT_GOLD_COUNT = 30;

static const string USER_DATA_FILE_NAME = string("bubbleDragonUserData.dat");//用户数据保存文件

void MyGame::MyGame_(){
	m_gold=100;
	m_HammerNum=1;
	m_bombNum=1;
	m_colorBubbleNum=1;
	m_guidLineNum=1;
	//m_musicON=nullptr;
	//m_effectON = nullptr;
	max_cols=MAX_COLS;
	max_rows=0;
	m_mobileType = CHINA_NONE;
}

int MyGame::MyGame__(){
	if(!isModified)
		saveUserData();
	return 0;
}

int MyGame::init()
{
	cocos2d::CCLog("my game init");
	//m_inst = this;
	m_gold = DEFAULT_GOLD_COUNT;
	std::string path = MyUtilsConfig::getInst()->m_mainLogPath;
	Logging::LogLeveL mainLogLevel = Logging::LOG_LEVEL_DEBUG;
	Logging::BUFFERING_MODE mainLogBufferMode = Logging::BUFFERING_AUTO;
	if (path.length() > 0)
	{
		cocos2d::CCLog("main log target write path:%s", path.c_str());
		m_mainLog = Logging::getInstance(Logging::OUTPUT_FILE| Logging::OUTPUT_STDOUT, path.c_str(), mainLogBufferMode, Logging::REWRITE_OVERWRITE);
		m_mainLog->limitPrintLevel2(mainLogLevel);
		m_mainLog->setFormat(Logging::ORNAMENT_ALL);
	}
	else
	{
		cocos2d::CCLog("sdcard path get failed. no write to file");
		m_mainLog = Logging::getInstance();
	}
	m_mainLog->getInstance()->printlogs(Logging::LOG_LEVEL_DEBUG, "Read main log info successfully!");
	isModified=false;
	initGameData();
	this->initMobileType();
	return 0;
}

void MyGame::initGameData()
{
	m_userDataPath=CCFileUtils::sharedFileUtils()->getWritablePath()+USER_DATA_FILE_NAME;
	if (isFileExist(m_userDataPath.c_str()))
	{
		std::string md5Str = "";
		//ifstream 文件读取 ofstream 文件写操作 fstream 读写操作
		ifstream ifs(m_userDataPath.c_str());
		if (ifs.is_open())//文件读取
		{
			int musicInt;
			int effectInt;

			ifs >> m_gold >> m_HammerNum >> m_bombNum >> m_colorBubbleNum >> m_guidLineNum 
				>> md5Str >> musicInt >> effectInt;

			if (musicInt==0)
				m_musicON = false;
			else
				m_musicON = true;

			if (effectInt == 0)
				m_effectON = false;
			else
				m_effectON = true;

			ifs.close();
		}
		if (!validateKey(m_gold,m_HammerNum,m_bombNum,m_colorBubbleNum,m_guidLineNum, md5Str.c_str()))
		{
			isModified=true;
			exitGame();
			exit(-1);
		}
	}
}

bool MyGame::isFileExist(const char *path)
{
	FILE *fp = fopen(path, "r");
	if (fp)
	{
		fclose(fp);
		return true;
	}
	return false;
}

bool MyGame::validateKey(int gold,int hammer,int bomb,int colorNum,int guidLine, const char* key)
{
	std::string md5Str = encryptToKeyByMD5(gold, hammer, bomb, colorNum, guidLine);
	if (0 == strcmp(md5Str.c_str(), key))
	{
		return true;
	}else{
		return false;
		//return true;
	}
}

std::string MyGame::encryptToKeyByMD5(int gold,int hammer,int bomb,int colorNum,int guidLine)
{
	std::stringstream userDataSS;
	userDataSS<<gold<<hammer<<gold<<bomb<<colorNum<<guidLine;
	MD5 md5 = MD5(userDataSS.str());
	std::string md5Str = md5.hexdigest();
	return md5Str;
}

void MyGame::saveUserData()
{
	std::string md5Str = encryptToKeyByMD5(m_gold,m_HammerNum,m_bombNum,m_colorBubbleNum,m_guidLineNum);
	ofstream ofs(m_userDataPath.c_str());//如果文件不存在会创建文件
	if (ofs.is_open())
	{
		ostringstream musicos;
		ostringstream effectos;
		musicos << m_musicON;
		effectos << m_effectON;

		ofs << m_gold << " " << m_HammerNum << " " << m_bombNum << " " << m_colorBubbleNum << " " 
			<< m_guidLineNum << " " << md5Str.c_str() << " " << string(musicos.str()) << " " << string(effectos.str()) << " ";
		ofs.close();
	}
}

void MyGame::exitGame()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo minfoStore;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(minfoStore,"com/skt/sdk/main/CplusAndJavaInteraction","exitGame", "()V");  
	if (!isHave) { 
		CCLog("jni:not exitGame"); 
	}else{ 
		CCLog("jni:exitGame"); 
		minfoStore.env->CallStaticVoidMethod(minfoStore.classID, minfoStore.methodID);
		CCLog("jni:exitGame 2"); 
		minfoStore.env->DeleteLocalRef(minfoStore.classID);
		CCLog("jni:exitGame 3"); 
	}
#endif 

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	//ScrollTips::showSimpleNote_Center("You pressed the close button. Windows Store Apps do not implement a close button.");
#else
	//CCDirector::sharedDirector()->end();
	//exit(0);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif
}

int MyGame::addGold(int gold){
	m_gold += gold;
	return 0;
}

void MyStatisticsLog::sendFinishLaunchingLog(){
	const char *action = "finishLaunching";
	StatisticsLog::getInstance()->sendLogData(action);
}

void MyStatisticsLog::sendForegroundLog(){
	const char *action = "foreground";
	StatisticsLog::getInstance()->sendLogData(action);
}

void MyStatisticsLog::sendBackgroundLog(){
	const char *action = "background";
	StatisticsLog::getInstance()->sendLogData(action);
}

void MyStatisticsLog::sendLoginLog(){
	const char *action = "login";
	StatisticsLog::getInstance()->sendLogData(action);
}

void MyStatisticsLog::sendLogoutLog(){
	const char *action = "logout";
	StatisticsLog::getInstance()->sendLogData(action);
}

void MyGame::addMyProperty(int gold,int hammer,int bomb,int colorNum,int guidLine)
{
	m_gold+=gold;
	m_HammerNum+=hammer;
	m_bombNum+=bomb;
	m_colorBubbleNum+=colorNum;
	m_guidLineNum+=guidLine;
}

bool MyGame::isGameMusicAbled()
{
	return m_musicON;
}

void MyGame::setGameMusic(bool abled)
{
	m_musicON=abled;
	this->saveUserData();
}

bool MyGame::isGameEffectAbled()
{
	return m_effectON;
}

void MyGame::setGameEffect(bool abled)
{
	m_effectON=abled;
	this->saveUserData();
}

int MyGame::getCurStage()
{
	return m_curStage;
}
void MyGame::setCurStage(int num)
{
	m_curStage=num;
}

MOBILE_TYPE MyGame::getMobileType()
{
	return m_mobileType;
}

void MyGame::initMobileType()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo mInfo;
	if (JniHelper::getStaticMethodInfo(mInfo,"com/skt/sdk/main/CplusAndJavaInteraction","getMobileType", "()I")) 
	{ 
		jint iret = mInfo.env->CallStaticIntMethod(mInfo.classID,mInfo.methodID);
		if(iret==0){
			m_mobileType=CHINA_MOBILE;
		}else if (iret == 1){
			m_mobileType = CHINA_TELECOM;
		}else if (iret==2){
			m_mobileType == CHINA_UNICOM;
		}else{
			m_mobileType == CHINA_NONE;
			CCLog("MyGame:mobileType is CHINA_NODE");
		}
	}
#endif
}
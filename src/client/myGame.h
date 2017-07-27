#ifndef __MY_GAME_H__
#define __MY_GAME_H__
#include <vector>
#include "newbeeUtils/nbString.h"
#include <string>
#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>
#include "publicDefine.h"

using namespace cocos2d::extension;

class ClientGameDesk;
namespace NEWBEE_UTILS
{
	class Logging;
}


class MyGame
{
public:
	static const int CUR_STANDARD_WIDTH = 640;//�������ĵ�ǰ��׼��Ļ���(��������ڲ�ͬ��Ļ������Ӧ���)
	static const int CUR_STANDARD_HEIGHT = 960;//�������ĵ�ǰ��׼��Ļ�߶�(��������ڲ�ͬ��Ļ������Ӧ���)
	static const int BUBBLE_GOLD_REWARD_NUM = 5;
	static MyGame* getInstance()
	{
		if(!m_inst)
		{
			m_inst=new MyGame();
			m_inst->init();
		}
		return m_inst;
	}
	MyGame(){
		MyGame_();
	}
	~MyGame(){
		MyGame__();
	}
	int init();
	void initGameData();//��ʼ����Ϸ����
	void exitGame();//�˳���Ϸ
	bool isFileExist(const char *path);//�ļ��Ƿ����
	void saveUserData();//������Ϸ����
	std::string encryptToKeyByMD5(int gold,int hammer,int bomb,int colorNum,int guidLine);//MD5����
	bool validateKey(int gold,int hammer,int bomb,int colorNum,int guidLine, const char* key);//MD5У��

	int addGold(int gold);
	void addMyProperty(int gold=0,int hammer=0,int bomb=0,int colorNum=0,int guidLine=0);
	bool isGameMusicAbled();
	void setGameMusic(bool abled);
	bool isGameEffectAbled();
	void setGameEffect(bool abled);

	int getCurStage();
	void setCurStage(int num);
	MOBILE_TYPE getMobileType();
	void initMobileType();
protected://simple object(including unliable pointer) - (1) liable pointer - (2) complex object - (3) father class - (4)
	void MyGame_();
	int MyGame__();
	static MyGame* m_inst;
	
	public:
	NEWBEE_UTILS::Logging* m_mainLog;



public:
	int m_gold;//���
	int m_HammerNum;//����
	int m_bombNum;//ը��
	int m_colorBubbleNum;//����
	int m_guidLineNum;//������
	

	int max_rows;
	int max_cols;

private:
	std::string m_userDataPath;
	bool isModified;
	bool m_effectON;
	bool m_musicON;
	int m_curStage;
	MOBILE_TYPE m_mobileType;
};

class MyStatisticsLog{
public:
	static void sendFinishLaunchingLog();
	static void sendForegroundLog();
	static void sendBackgroundLog();
	static void sendLoginLog();
	static void sendLogoutLog();
};

#endif//__MY_GAME_H__


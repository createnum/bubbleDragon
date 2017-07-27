#include "GamePay.h"
#include "storeBuyItenDict.h"
#include "myGame.h"
#include "statisticsLog.h"
#include "uiStoreScene.h"
#include "gameScene.h"
#include "stageSelectScene.h"
#include "XMLTools.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h> 
#include "platform/android/jni/JniHelper.h" 
#endif

USING_NS_CC;

GamePay*GamePay::m_inst=NULL;

GamePay::GamePay()
{

}

GamePay::~GamePay()
{

}

bool GamePay::init()
{
	return true;
}

GamePay*GamePay::getInstance()
{
	if(!m_inst)
	{
		m_inst=new GamePay();
		m_inst->init();
	}
	return m_inst;
}

int GamePay::rmbTransitionTargetID(int rmb, PayType payType) {
	int targetID = 0;
	m_type = payType;
	switch (rmb)
	{
	case 1:
		targetID = 1;
		break;
	case 10:
		targetID = 2;
		break;
	case 100:
		targetID = 3;
		break;
	case 200:
		targetID = 4;
		break;
	case 400:
		if (payType == PAY_RESURRECTION_GIFT)
		{
			targetID = 6;
			break;
		}
		targetID = 5;
		break;
	case 600:
		targetID = 7;
		break;
	case 800:
		targetID = 8;
		break;
	case 1000:
		targetID = 9;
		break;
	case 1800:
		targetID = 10;
		break;
	case 1500:
		targetID = 11;
		break;
	case 2000:
		targetID = 12;
		break;

	default:
		break;
	}
	return targetID;
}

void GamePay::payWithRmb(int targetID)
{
	//m_type = pType;
	CCLOG("targetID is : %d",targetID);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo mInfo;
	if (JniHelper::getStaticMethodInfo(mInfo,"com/skt/sdk/main/CplusAndJavaInteraction","pay", "(I)V")) 
	{ 
		mInfo.env->CallStaticVoidMethod(mInfo.classID,mInfo.methodID,targetID);
	}
#else
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#endif
#endif 
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"
{
	void Java_com_skt_sdk_main_CplusAndJavaInteraction_paySuccessJava(JNIEnv* env, jobject obj, jint rmb, jstring orderId){
		std::string  orderIdStr = JniHelper::jstring2string(orderId); 
		GamePay::getInstance()->payCallBackSuccess(rmb,orderIdStr);
	}
	void Java_com_skt_sdk_main_CplusAndJavaInteraction_payFailJava(JNIEnv* env, jobject obj, jint rmb, jstring context){
		std::string  contextStr = JniHelper::jstring2string(context); 
		GamePay::getInstance()->payCallFailSuccess(contextStr);
	}
};
#endif

void GamePay::payCallBackSuccess(int rmb, std::string statuesCode)
{
	CCLog("GamePay::payCallBackSuccess,the statuesCode is :%s",statuesCode.c_str());
	CCString*str = CCString::createWithFormat("%d", rmb / 200);
	StatisticsLog::getInstance()->sendLogData("PaySuccess", str->getCString(), statuesCode.c_str());
	if (m_type == PAY_NORMAL)
	{
		std::vector<BuyItem>buyItems = StoreBuyItemDict::getInstance()->getBuyItems();
		for (int i = 0; i < buyItems.size(); i++)
		{
			if (rmb == buyItems[i].rmb)
			{
				MyGame::getInstance()->addGold(buyItems[i].gold);
				CCLog("GamePay::payCallBackSuccess the gold is :%d", MyGame::getInstance()->m_gold);
				UIStoreScene::getInstance()->initData();
				return;
			}
		}
	}
	else if (m_type == PAY_RESURRECTION_GIFT)
	{
		MyGame::getInstance()->addMyProperty(0, 3, 3, 3, 3);
		if (GameScene::getInstance())
			GameScene::getInstance()->setPropertyNumTTF();
		GameScene::getInstance()->gameResurrection();
	}
	else if (m_type == PAY_HOT_GIFT)
	{
		MyGame::getInstance()->addMyProperty(4200, 5, 5, 5, 5);
	}
	else if (m_type==PAY_UNLOCK)
	{
		stageInfo*info = XMLTools::getInstance()->getStageInfoByIndex(MyGame::getInstance()->getCurStage());
		if (info->m_isSpecialStage)
			info->m_isSpecialStage = false;
		XMLTools::getInstance()->modifyStageInfoByInfo(info);
	}
	else if (m_type==PAY_OWNUSE)
	{
		if (rmb==1)
			MyGame::getInstance()->addMyProperty(0, 1,1, 1,1);
		else if (rmb==10)
			MyGame::getInstance()->addMyProperty(0, 2,2, 2,2);
	}
}

void GamePay::payCallFailSuccess(std::string errorStr)
{
	StatisticsLog::getInstance()->sendLogData("PayFailed", errorStr.c_str());
	CCLog("GamePay::payCallFailSuccess %s",errorStr.c_str());
	if (m_type == PAY_NORMAL)
	{
		
	}
	else if (m_type == PAY_RESURRECTION_GIFT)
	{
		GameScene::getInstance()->gameIsOver();
	}
	else if (m_type == PAY_HOT_GIFT)
	{
		
	}
	else if (m_type == PAY_UNLOCK)
	{
		CCDirector::sharedDirector()->replaceScene(StageSelectScene::scene());
	}
	else if (m_type == PAY_OWNUSE)
	{
		
	}
}

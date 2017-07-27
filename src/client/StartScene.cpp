#include "StartScene.h"
#include "BubbleConstant.h"
#include "GameScene.h"
#include "stageSelectScene.h"
#include "myGame.h"
#include "uiHelpScene.h"
#include "uiStoreScene.h"
#include "publicDefine.h"
#include "statisticsLog.h"
#include "uiResurrectionScene.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h> 
#include "platform/android/jni/JniHelper.h" 
#include <android/log.h> 
#endif
#include "simulatorGameScene.h"


#define UI_MAIN_SCENE_JSON "ui/startUI_0.json"
#define UI_MAIN_SCENE_PASS_BUTTON_NAME "passModeButton"
#define UI_MAIN_SCENE_GIFT_BUTTON_NAME "giftButton"
#define UI_MAIN_SCENE_MUSIC_BUTTON_NAME "musicButton"
#define UI_MAIN_SCENE_STORE_BUTTON_NAME "storeButton"
#define UI_MAIN_SCENE_EFFECT_BUTTON_NAME "effectButton"
#define UI_MAIN_SCENE_HELP_BUTTON_NAME "helpButton"
#define UI_MAIN_SCENE_BG_IMAGE_NAME "Image_9"

#define UI_MAIN_SCENE_MUSIC_ON "ui/myRes/yinyue.png"
#define UI_MAIN_SCENE_MUSIC_OFF "ui/myRes/yinyuean.png"
#define UI_MAIN_SCENE_EFFECT_ON "ui/myRes/yinxiao.png"
#define UI_MAIN_SCENE_EFFECT_OFF "ui/myRes/yinxiaoan.png"

using namespace cocos2d;
using namespace gui;


StartLayer::StartLayer()
{
	m_musicButton=NULL;
	m_effectButton=NULL;
}

StartLayer::~StartLayer()
{
	m_musicButton=NULL;
	m_effectButton=NULL;
}

CCScene* StartLayer::scene()
{
	CCScene*pScene=CCScene::create();
	StartLayer*pLayer=StartLayer::create();
	pScene->addChild(pLayer);
    return pScene;
}

bool StartLayer::init()
{
	if (!UILayer::init())
	{
		return false;
	}

	this->preLoadMusicFile();
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_STARTSCENE_BG_MUSIC, true);
	if ((!MyGame::getInstance()->isGameMusicAbled())&&SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
	{
		//SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_STARTSCENE_BG_MUSIC, true);
		SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	}

	UIWidget*pWidget=extension::GUIReader::shareReader()->widgetFromJsonFile(UI_MAIN_SCENE_JSON);
	this->addWidget(pWidget);
	this->screenAdaptive(pWidget);
	this->addTouchEventListener(pWidget);
	this->setKeypadEnabled(true);
	return true;
}


void StartLayer::screenAdaptive(cocos2d::gui::UIWidget*pWidget)
{
	CCSize size=CCDirector::sharedDirector()->getVisibleSize();
	pWidget->setSize(ccp(640,640*size.height/size.width));
}
void StartLayer::buttonCallBack(CCObject*pSender,cocos2d::gui::TouchEventType pType)
{
	switch(pType)
	{
	case TOUCH_EVENT_BEGAN:
		break;
	case TOUCH_EVENT_MOVED:
		break;
	case TOUCH_EVENT_ENDED:
		{
			UIWidget*pNode=(UIWidget*)pSender;
			const char*name=pNode->getName();
			touchEventDesponse(name);
		}
		break;
	}
}

void StartLayer::addTouchEventListener(UIWidget*pWidget)
{
	UIButton*passButton=dynamic_cast<UIButton*>(pWidget->getChildByName(UI_MAIN_SCENE_PASS_BUTTON_NAME));
	passButton->addTouchEventListener(this,toucheventselector(StartLayer::buttonCallBack));
	passButton->runAction(CCRepeatForever::create(getButtonAction(3.0f)));

	UIButton*pGiftButton = dynamic_cast<UIButton*>(pWidget->getChildByName(UI_MAIN_SCENE_GIFT_BUTTON_NAME));
	pGiftButton->addTouchEventListener(this, toucheventselector(StartLayer::buttonCallBack));
	pGiftButton->runAction(CCRepeatForever::create(getButtonAction(4.0f)));
	//pGiftButton->setVisible(true);//plist中为false  手动显示
	//pGiftButton->setTouchEnabled(true);//plist中为false

	
	m_musicButton=dynamic_cast<UIButton*>(pWidget->getChildByName(UI_MAIN_SCENE_MUSIC_BUTTON_NAME));
	m_musicButton->addTouchEventListener(this,toucheventselector(StartLayer::buttonCallBack));

	if(!MyGame::getInstance()->isGameMusicAbled())
		m_musicButton->loadTextureNormal(UI_MAIN_SCENE_MUSIC_OFF);

	m_effectButton=dynamic_cast<UIButton*>(pWidget->getChildByName(UI_MAIN_SCENE_EFFECT_BUTTON_NAME));
	m_effectButton->addTouchEventListener(this,toucheventselector(StartLayer::buttonCallBack));
	bool test = MyGame::getInstance()->isGameEffectAbled();
	if(!MyGame::getInstance()->isGameEffectAbled())
		m_effectButton->loadTextureNormal(UI_MAIN_SCENE_EFFECT_OFF);

	UIButton*storeButton=dynamic_cast<UIButton*>(pWidget->getChildByName(UI_MAIN_SCENE_STORE_BUTTON_NAME));
	storeButton->addTouchEventListener(this,toucheventselector(StartLayer::buttonCallBack));
	UIButton*helpButton=dynamic_cast<UIButton*>(pWidget->getChildByName(UI_MAIN_SCENE_HELP_BUTTON_NAME));
	helpButton->addTouchEventListener(this,toucheventselector(StartLayer::buttonCallBack));
}

CCActionInterval*StartLayer::getButtonAction(float delay)
{
	CCScaleTo*pScaleTo1 = CCScaleTo::create(0.3f, 1.1f, 0.9f);
	CCScaleTo*pScaleTo2 = CCScaleTo::create(0.3f, 1.0f);
	CCSequence*seq = CCSequence::create(pScaleTo1, pScaleTo2, NULL);
	CCRepeat*pRepeat = CCRepeat::create(seq, 2);
	CCSequence*pSeq = CCSequence::create(CCDelayTime::create(delay), pRepeat, NULL);
	return pSeq;
}

void StartLayer::touchEventDesponse(const char*name)
{

	if(strcmp(UI_MAIN_SCENE_PASS_BUTTON_NAME,name)==0)
	{
		CCDirector::sharedDirector()->replaceScene(StageSelectScene::scene());

	}
	else if (strcmp(UI_MAIN_SCENE_GIFT_BUTTON_NAME,name)==0)
	{
		this->addChild(UIResurrectionScene::create(1));
	}
	else if (strcmp(UI_MAIN_SCENE_MUSIC_BUTTON_NAME,name)==0)
	{
		if(MyGame::getInstance()->isGameMusicAbled())
		{
			MyGame::getInstance()->setGameMusic(false);
			SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			m_musicButton->loadTextureNormal(UI_MAIN_SCENE_MUSIC_OFF);
		}
		else
		{
			MyGame::getInstance()->setGameMusic(true);
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_STARTSCENE_BG_MUSIC, true);
			m_musicButton->loadTextureNormal(UI_MAIN_SCENE_MUSIC_ON);
		}
	}
	else if (strcmp(UI_MAIN_SCENE_EFFECT_BUTTON_NAME,name)==0)
	{
		if(MyGame::getInstance()->isGameEffectAbled())
		{
			MyGame::getInstance()->setGameEffect(false);
			m_effectButton->loadTextureNormal(UI_MAIN_SCENE_EFFECT_OFF);
		}
		else
		{
			MyGame::getInstance()->setGameEffect(true);
			m_effectButton->loadTextureNormal(UI_MAIN_SCENE_EFFECT_ON);
		}
	}
	else if (strcmp(UI_MAIN_SCENE_STORE_BUTTON_NAME,name)==0)
	{
		CCDirector::sharedDirector()->pushScene(UIStoreScene::scene());
	}
	else if (strcmp(UI_MAIN_SCENE_HELP_BUTTON_NAME,name)==0)
	{
		CCDirector::sharedDirector()->pushScene(UIHelpScene::scene());
	}
	if (MyGame::getInstance()->isGameEffectAbled())
		SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_BUTTON_EFFECT);
}

void StartLayer::keyBackClicked()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/skt/sdk/main/CplusAndJavaInteraction","exitGame", "()V");  
	if (!isHave) 
	{ 
		CCLog("StartLayer::the func of keyBackClicked do not exit"); 
	}
	else
	{ 
		CCLog("StartLayer::the func of keyBackClicked exit"); 
		StatisticsLog::getInstance()->sendLogData("ExitGame");
		minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID);
	}
#endif 
}

void StartLayer::preLoadMusicFile()
{
	//SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(MUSIC_STARTSCENE_BG_MUSIC);
	//SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(MUSIC_GAMESCENE_BG_MUSIC);
	//SimpleAudioEngine::sharedEngine()->preloadEffect(EFFECT_CLEAR1_EFFECT);
	//SimpleAudioEngine::sharedEngine()->preloadEffect(EFFECT_CLEAR2_EFFECT);
	//SimpleAudioEngine::sharedEngine()->preloadEffect(EFFECT_CLEAR3_EFFECT);
	//SimpleAudioEngine::sharedEngine()->preloadEffect(EFFECT_CLEAR4_EFFECT);
	//SimpleAudioEngine::sharedEngine()->preloadEffect(EFFECT_CLEAR5_EFFECT);
	//SimpleAudioEngine::sharedEngine()->preloadEffect(EFFECT_CLEAR6_EFFECT);
	//SimpleAudioEngine::sharedEngine()->preloadEffect(EFFECT_COMBO1_EFFECT);
	//SimpleAudioEngine::sharedEngine()->preloadEffect(EFFECT_COMBO2_EFFECT);
	//SimpleAudioEngine::sharedEngine()->preloadEffect(EFFECT_COMBO3_EFFECT);
	//SimpleAudioEngine::sharedEngine()->preloadEffect(EFFECT_COMBO4_EFFECT);
	//SimpleAudioEngine::sharedEngine()->preloadEffect(EFFECT_GOLD_EFFECT);
	//SimpleAudioEngine::sharedEngine()->preloadEffect(EFFECT_HAMMER_EFFECT);
	//SimpleAudioEngine::sharedEngine()->preloadEffect(EFFECT_LIGHTING_EFFECT);
	//SimpleAudioEngine::sharedEngine()->preloadEffect(EFFECT_PASSFAILED_EFFECT);
	//SimpleAudioEngine::sharedEngine()->preloadEffect(EFFECT_PASSGAME_EFFECT);
	//SimpleAudioEngine::sharedEngine()->preloadEffect(EFFECT_BUTTON_EFFECT);
	//SimpleAudioEngine::sharedEngine()->preloadEffect(EFFECT_READYGO_EFFECT);
	//SimpleAudioEngine::sharedEngine()->preloadEffect(EFFECT_BOMB_EFFECT);
}
#include "uiResurrectionScene.h"
#include "publicDefine.h"
#include "SimpleAudioEngine.h"
#include "myGame.h"
#include "GamePay.h"
#include "gameScene.h"
#include "statisticsLog.h"

#define UI_RESURRECTION_SCENE_JSON "ui/resurrectionGift.json"
#define UI_HOT_SCENE_JSON "ui/hotGift.json"
#define UI_RESURRECTION_SCENE_BASEIMAGE "Image_1"
#define UI_RESURRECTION_SCENE_GOBACK "goBack"
#define UI_RESURRECTION_SCENE_BUY "buyButton"

using namespace cocos2d;
using namespace gui;
using namespace CocosDenshion;

UIResurrectionScene::UIResurrectionScene(void)
{
}

UIResurrectionScene::~UIResurrectionScene(void)
{
}

UIResurrectionScene*UIResurrectionScene::create(int resOrHot)
{
	UIResurrectionScene *pRet = new UIResurrectionScene(); 
	if (pRet && pRet->init(resOrHot)) 
	{ 
		pRet->autorelease(); 
	} 
	else 
	{ 
		delete pRet; 
		pRet = NULL; 
	} 
	return pRet;
}

bool UIResurrectionScene::init(int resOrHot)
{
	if (!UILayer::init())
	{
		return false;
	}
	UIWidget*pWidget = NULL;
	pSceneType = resOrHot;
	if (pSceneType == 0)
	{
		pWidget = extension::GUIReader::shareReader()->widgetFromJsonFile(UI_RESURRECTION_SCENE_JSON);
		StatisticsLog::getInstance()->sendLogData("UIResurrectionScene");
	}
	else
	{
		pWidget = extension::GUIReader::shareReader()->widgetFromJsonFile(UI_HOT_SCENE_JSON);
		StatisticsLog::getInstance()->sendLogData("UIHotGiftScene");
	}
		
	this->addWidget(pWidget);
	this->screenAdaptive(pWidget);

	UIImageView*rootImage=dynamic_cast<UIImageView*>(pWidget->getChildByName(UI_RESURRECTION_SCENE_BASEIMAGE));
	this->addTouchEventListener(rootImage);

	return true;
}

void UIResurrectionScene::screenAdaptive(cocos2d::gui::UIWidget*pWidget)
{
	CCSize size=CCDirector::sharedDirector()->getVisibleSize();
	pWidget->setSize(ccp(640,640*size.height/size.width));
}


void UIResurrectionScene::addTouchEventListener(UIWidget*pWidget)
{
	UIButton*closeButton=dynamic_cast<UIButton*>(pWidget->getChildByName(UI_RESURRECTION_SCENE_GOBACK));
	closeButton->addTouchEventListener(this,toucheventselector(UIResurrectionScene::touchCallBack));
	UIButton*buyButton=dynamic_cast<UIButton*>(pWidget->getChildByName(UI_RESURRECTION_SCENE_BUY));
	buyButton->addTouchEventListener(this, toucheventselector(UIResurrectionScene::touchCallBack));
	if (MyGame::getInstance()->getMobileType() != CHINA_MOBILE){
		buyButton->setBright(true);
		buyButton->setTouchEnabled(true);
	}
}

void UIResurrectionScene::disposeTouchEvent(cocos2d::gui::UIWidget*pWidget)
{
	if (MyGame::getInstance()->isGameEffectAbled())
		SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_BUTTON_EFFECT);
	const char*name=pWidget->getName();
	if (strcmp(name, UI_RESURRECTION_SCENE_GOBACK) == 0)
	{
		if (pSceneType==0)
			StatisticsLog::getInstance()->sendLogData("UIResurrectionSceneGoBack");
		else
			StatisticsLog::getInstance()->sendLogData("UIHotGiftSceneGoBack");
		this->removeFromParentAndCleanup(true);
		if (pSceneType==0)
			GameScene::getInstance()->gameIsOver();
	} 
	else if (strcmp(name, UI_RESURRECTION_SCENE_BUY) == 0)
	{
		if (pSceneType == 0)
			StatisticsLog::getInstance()->sendLogData("UIResurrectionSceneBuy");
		else
			StatisticsLog::getInstance()->sendLogData("UIHotGiftSceneBuy");
		this->removeFromParentAndCleanup(true);
		if (pSceneType==0)
			GamePay::getInstance()->payWithRmb(GamePay::getInstance()->rmbTransitionTargetID(400, PAY_RESURRECTION_GIFT));
		else
			GamePay::getInstance()->payWithRmb(GamePay::getInstance()->rmbTransitionTargetID(1800, PAY_HOT_GIFT));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
		//GameScene::getInstance()->gameResurrection();
#endif
	}
}

void UIResurrectionScene::touchCallBack(CCObject*pSender,TouchEventType pType)
{
	switch(pType)
	{
	case TOUCH_EVENT_BEGAN:
		break;
	case TOUCH_EVENT_MOVED:
		break;
	case TOUCH_EVENT_ENDED:
		{
			UIWidget*pWidget=(UIWidget*)pSender;
			this->disposeTouchEvent(pWidget);
		}
		break;
	}
}
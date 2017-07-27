#include "uiDeblockingScene.h"
#include "GamePay.h"
#include "publicDefine.h"
#include "stageSelectScene.h"
#include "myGame.h"
#include "uiHelpScene.h"

#define UI_BLOCKING_STAGE_JSON "ui/unblockingStage.json"
#define UI_BLOCKING_STAGE_BACK_BUTTON "Button_154"
#define UI_BLOCKING_STAGE_BUY_BUTTON "Button_109"

#define UI_NEWPLAYER_GUIDE_JSON "ui/yindao.json"
#define UI_NEWPLAYER_GUIDE_IAMGE "Image_28"
#define UI_NEWPLAYER_GUIDE_MORE_BUTTON "moreHelp"
#define UI_NEWPLAYER_GUIDE_BEGIN_BUTTON "beginGame"

USING_NS_CC;
using namespace gui;

UIDeblockingScene::UIDeblockingScene(void)
{
}


UIDeblockingScene::~UIDeblockingScene(void)
{
}

CCScene*UIDeblockingScene::scene()
{
	CCScene*pScene=CCScene::create();
	UIDeblockingScene*player=UIDeblockingScene::create();
	pScene->addChild(player);
	return pScene;
}

bool UIDeblockingScene::init()
{
	if (!UILayer::init())
	{
		return false;
	}
	UIWidget*pWidget=extension::GUIReader::shareReader()->widgetFromJsonFile(UI_BLOCKING_STAGE_JSON);
	this->addWidget(pWidget);
	CCSize size=CCDirector::sharedDirector()->getVisibleSize();
	pWidget->setSize(ccp(640,640*size.height/size.width));

	UIButton*pBackButton=dynamic_cast<UIButton*>(pWidget->getChildByName(UI_BLOCKING_STAGE_BACK_BUTTON));
	pBackButton->addTouchEventListener(this,toucheventselector(UIDeblockingScene::touchCallBack));

	UIButton*pBuyButton=dynamic_cast<UIButton*>(pWidget->getChildByName(UI_BLOCKING_STAGE_BUY_BUTTON));
	pBuyButton->addTouchEventListener(this,toucheventselector(UIDeblockingScene::touchCallBack));

	if (MyGame::getInstance()->getMobileType() != CHINA_MOBILE)
	{
		pBuyButton->setBright(true);
		pBuyButton->setTouchEnabled(true);
	}

	return true;
}

void UIDeblockingScene::touchCallBack(CCObject*pSender,TouchEventType pType)
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

void UIDeblockingScene::disposeTouchEvent(UIWidget*pWidget)
{
	const char*name=pWidget->getName();
	if (strcmp(name,UI_BLOCKING_STAGE_BACK_BUTTON)==0)
	{
		//CCDirector::sharedDirector()->popScene();
		this->removeFromParentAndCleanup(true);
		CCDirector::sharedDirector()->replaceScene(StageSelectScene::scene());
	} 
	else if(strcmp(name,UI_BLOCKING_STAGE_BUY_BUTTON)==0)
	{
		this->removeFromParentAndCleanup(true);
		GamePay::getInstance()->payWithRmb(GamePay::getInstance()->rmbTransitionTargetID(400, PAY_UNLOCK));
		//GamePay::getInstance()->payWithRmb(400);
	}
}

/****************************************************************************************
 *
 *新手引导界面
 *
 ****************************************************************************************/

NewPlayerGuide::NewPlayerGuide(void)
{
}


NewPlayerGuide::~NewPlayerGuide(void)
{
}

CCScene*NewPlayerGuide::scene()
{
	CCScene*pScene = CCScene::create();
	NewPlayerGuide*player = NewPlayerGuide::create();
	pScene->addChild(player);
	return pScene;
}

bool NewPlayerGuide::init()
{
	if (!UILayer::init())
	{
		return false;
	}
	UIWidget*pWidget = extension::GUIReader::shareReader()->widgetFromJsonFile(UI_NEWPLAYER_GUIDE_JSON);
	this->addWidget(pWidget);
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	pWidget->setSize(ccp(640, 640 * size.height / size.width));

	UIImageView*pImage = dynamic_cast<UIImageView*>(pWidget->getChildByName(UI_NEWPLAYER_GUIDE_IAMGE));

	UIButton*pMoreButton = dynamic_cast<UIButton*>(pImage->getChildByName(UI_NEWPLAYER_GUIDE_MORE_BUTTON));
	pMoreButton->addTouchEventListener(this, toucheventselector(NewPlayerGuide::touchCallBack));

	UIButton*pBeginButton = dynamic_cast<UIButton*>(pImage->getChildByName(UI_NEWPLAYER_GUIDE_BEGIN_BUTTON));
	pBeginButton->addTouchEventListener(this, toucheventselector(NewPlayerGuide::touchCallBack));

	return true;
}

void NewPlayerGuide::touchCallBack(CCObject*pSender, TouchEventType pType)
{
	switch (pType)
	{
	case TOUCH_EVENT_BEGAN:
		break;
	case TOUCH_EVENT_MOVED:
		break;
	case TOUCH_EVENT_ENDED:
	{
		UIWidget*pWidget = (UIWidget*)pSender;
		this->disposeTouchEvent(pWidget);
	}
		break;
	}
}

void NewPlayerGuide::disposeTouchEvent(UIWidget*pWidget)
{
	const char*name = pWidget->getName();
	if (strcmp(name, UI_NEWPLAYER_GUIDE_MORE_BUTTON) == 0)
	{
		//this->removeFromParentAndCleanup(true);
		CCDirector::sharedDirector()->pushScene(UIHelpScene::scene());
	}
	else if (strcmp(name, UI_NEWPLAYER_GUIDE_BEGIN_BUTTON) == 0)
	{
		this->removeFromParentAndCleanup(true);
	}
}
#include "uiStoreScene.h"
#include "StartScene.h"
#include "support/tinyxml2/tinyxml2.h"
#include "uiBuyPropertyScene.h"
#include "myGame.h"
#include "GamePay.h"
#include "publicDefine.h"
#include "SimpleAudioEngine.h"
#include "statisticsLog.h"
#include "gameScene.h"

#define UI_STORE_SCENE_JSON "ui/storeUI.json"
#define UI_STORE_SCENE_BACK_BUTTON_NAME "Button_154"
#define UI_STORE_SCENE_HAMMER_BUTTON_NAME "Button_37"
#define UI_STORE_SCENE_BOMB_BUTTON_NAME "Button_47"
#define UI_STORE_SCENE_GUIDE_BUTTON_NAME "Button_38"
#define UI_STORE_SCENE_COLOR_BUTTON_NAME "Button_50"
#define UI_STORE_SCENE_SCROLL_NAME "ScrollView_17"
#define UI_STORE_SCENE_BG_IMAGE_NAME "Image_29"

#define UI_STORE_SCENE_GOLD_IMAGE "Image_34"
#define UI_STORE_SCENE_GOLD_ALTAS "AtlasLabel_36"

#define UI_STORE_SCENE_HAMMER_IMAGE "Image_41_0"
#define UI_STORE_SCENE_HAMMER_ALTAS "AtlasLabel_42"

#define UI_STORE_SCENE_COLOR_IMAGE "Image_41_2"
#define UI_STORE_SCENE_COLOR_ALTAS "AtlasLabel_42"

#define UI_STORE_SCENE_BOMB_IMAGE "Image_41_1"
#define UI_STORE_SCENE_BOMB_ALTAS "AtlasLabel_42"

#define UI_STORE_SCENE_GUIDE_IMAGE "Image_41"
#define UI_STORE_SCENE_GUIDE_ALTAS "AtlasLabel_42"


#define UI_STORE_BUYITEM_JSON "ui/buyItem.json"
#define UI_STORE_BUY_ITEM_NAME "buyItem"
#define UI_STORE_BUY_GOLD_NAME "goldNum"
#define UI_STORE_BUY_BUTTON_NAME "buyButton"
#define UI_STORE_BUY_BUTTON_COST "costNum"

#define UI_BUY_ITEM_HEIGHT 150.0f

USING_NS_CC;
using namespace gui;
using namespace tinyxml2;
using namespace CocosDenshion;

UIStoreScene*UIStoreScene::m_inst = NULL;

UIStoreScene::UIStoreScene(void)
{
	
}


UIStoreScene::~UIStoreScene(void)
{
	MyGame::getInstance()->saveUserData();
}

CCScene*UIStoreScene::scene()
{
	CCScene*pScene=CCScene::create();
	UIStoreScene*player=UIStoreScene::create();
	pScene->addChild(player);
	return pScene;
}

UIStoreScene*UIStoreScene::getInstance()
{
	if (m_inst)
		return m_inst;
	else
		return NULL;
}

bool UIStoreScene::init()
{
	if (!UILayer::init())
	{
		return false;
	}

	UIWidget*pWidget=extension::GUIReader::shareReader()->widgetFromJsonFile(UI_STORE_SCENE_JSON);
	this->addWidget(pWidget);
	this->screenAdaptive(pWidget);

	UIButton*pButton=dynamic_cast<UIButton*>(pWidget->getChildByName(UI_STORE_SCENE_BACK_BUTTON_NAME));
	pButton->addTouchEventListener(this,toucheventselector(UIStoreScene::touchCallBack));

	UIImageView*pGoldImage=dynamic_cast<UIImageView*>(pWidget->getChildByName(UI_STORE_SCENE_GOLD_IMAGE));
	m_goldAtlas=dynamic_cast<UILabelAtlas*>(pGoldImage->getChildByName(UI_STORE_SCENE_GOLD_ALTAS));
	UIImageView*pHammerImage=dynamic_cast<UIImageView*>(pWidget->getChildByName(UI_STORE_SCENE_HAMMER_IMAGE));
	m_hammerAtlas=dynamic_cast<UILabelAtlas*>(pHammerImage->getChildByName(UI_STORE_SCENE_HAMMER_ALTAS));
	UIImageView*pColorImage=dynamic_cast<UIImageView*>(pWidget->getChildByName(UI_STORE_SCENE_COLOR_IMAGE));
	m_colorAtlas=dynamic_cast<UILabelAtlas*>(pColorImage->getChildByName(UI_STORE_SCENE_COLOR_ALTAS));
	UIImageView*pGuideImage=dynamic_cast<UIImageView*>(pWidget->getChildByName(UI_STORE_SCENE_GUIDE_IMAGE));
	m_guideAtlas=dynamic_cast<UILabelAtlas*>(pGuideImage->getChildByName(UI_STORE_SCENE_GUIDE_ALTAS));
	UIImageView*pBombImage=dynamic_cast<UIImageView*>(pWidget->getChildByName(UI_STORE_SCENE_BOMB_IMAGE));
	m_bombAtlas=dynamic_cast<UILabelAtlas*>(pBombImage->getChildByName(UI_STORE_SCENE_BOMB_ALTAS));

	UIScrollView*pScrollView=dynamic_cast<UIScrollView*>(pWidget->getChildByName(UI_STORE_SCENE_SCROLL_NAME));
	this->createBuyItemsOnScrollView(pScrollView);

	this->addTouchEventListener(pWidget);

	this->initData();
	StatisticsLog::getInstance()->sendLogData("EnterStore");
	m_inst = this;
	this->setKeypadEnabled(true);
	return true;
}

void UIStoreScene::screenAdaptive(cocos2d::gui::UIWidget*pWidget)
{
	CCSize size=CCDirector::sharedDirector()->getVisibleSize();
	pWidget->setSize(ccp(640,640*size.height/size.width));
}

void UIStoreScene::addTouchEventListener(UIWidget*pWidget)
{
	UIButton*hammerButton=dynamic_cast<UIButton*>(pWidget->getChildByName(UI_STORE_SCENE_HAMMER_BUTTON_NAME));
	hammerButton->addTouchEventListener(this,toucheventselector(UIStoreScene::touchCallBack));
	UIButton*bombButton=dynamic_cast<UIButton*>(pWidget->getChildByName(UI_STORE_SCENE_BOMB_BUTTON_NAME));
	bombButton->addTouchEventListener(this,toucheventselector(UIStoreScene::touchCallBack));
	UIButton*colorButton=dynamic_cast<UIButton*>(pWidget->getChildByName(UI_STORE_SCENE_COLOR_BUTTON_NAME));
	colorButton->addTouchEventListener(this,toucheventselector(UIStoreScene::touchCallBack));
	UIButton*guideButton=dynamic_cast<UIButton*>(pWidget->getChildByName(UI_STORE_SCENE_GUIDE_BUTTON_NAME));
	guideButton->addTouchEventListener(this,toucheventselector(UIStoreScene::touchCallBack));
}


void UIStoreScene::touchCallBack(CCObject*pSender,TouchEventType pType)
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

void UIStoreScene::disposeTouchEvent(UIWidget*pWidget)
{
	if (MyGame::getInstance()->isGameEffectAbled())
		SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_BUTTON_EFFECT);
	const char*name=pWidget->getName();
	if (strcmp(name,UI_STORE_SCENE_BACK_BUTTON_NAME)==0)
	{
		if (GameScene::getInstance())
			GameScene::getInstance()->setPropertyNumTTF();
		CCDirector::sharedDirector()->popScene();
	} 
	else if(strcmp(name,UI_STORE_BUY_BUTTON_NAME)==0)
	{
		UILabelAtlas*pCost=dynamic_cast<UILabelAtlas*>(pWidget->getChildByName(UI_STORE_BUY_BUTTON_COST));
		CCString str=pCost->getStringValue();
		int payNum=str.intValue()*100;
		StatisticsLog::getInstance()->sendLogData("PayWithRMB",str.getCString());
		GamePay::getInstance()->payWithRmb(GamePay::getInstance()->rmbTransitionTargetID(payNum, PAY_NORMAL));
		//GamePay::getInstance()->payWithRmb(payNum);
	}
	else
	{
		CCLOG(name);
		this->addChild(UIBuyPropertyScene::create(name));
	}
}

void UIStoreScene::createBuyItemsOnScrollView(UIWidget*pWidget)
{
	UIScrollView*pScrollView=(UIScrollView*)pWidget;
	buyItems=StoreBuyItemDict::getInstance()->getBuyItems();
	pScrollView->setInnerContainerSize(CCSizeMake(pScrollView->getContentSize().width, UI_BUY_ITEM_HEIGHT*buyItems.size()));
	for (int i = 0; i< buyItems.size(); ++i)
	{
		if (buyItems.at(i).isvisible == false)
		{
			continue;
		}
		UIWidget*imageView=createImageViewWithInfo(buyItems[i].gold,buyItems[i].rmb);
		pScrollView->addChild(imageView);
	}
}

UIWidget*UIStoreScene::createImageViewWithInfo(int gold,int rmb)
{
	UIWidget*pWidget=extension::GUIReader::shareReader()->widgetFromJsonFile(UI_STORE_BUYITEM_JSON);
	UIImageView*pImage=dynamic_cast<UIImageView*>(pWidget->getChildByName(UI_STORE_BUY_ITEM_NAME));
	UILabelAtlas*pAtlas=dynamic_cast<UILabelAtlas*>(pImage->getChildByName(UI_STORE_BUY_GOLD_NAME));
	{
		std::stringstream intToString;
		std::string temp;
		intToString<<gold;intToString>>temp;
		pAtlas->setStringValue(temp);
	}
	UIButton*pButton=dynamic_cast<UIButton*>(pImage->getChildByName(UI_STORE_BUY_BUTTON_NAME));
	pButton->addTouchEventListener(this,toucheventselector(UIStoreScene::touchCallBack));
	UILabelAtlas*pCost=dynamic_cast<UILabelAtlas*>(pButton->getChildByName(UI_STORE_BUY_BUTTON_COST));
	{
		std::stringstream intToString;
		std::string temp;
		intToString<<(float)rmb/100;intToString>>temp;
		if (rmb < 10)
		{
			pCost->setScaleX(0.7f);
		}
		pCost->setStringValue(temp);
	}
	return pWidget;
}

void UIStoreScene::initData()
{
	if(!m_goldAtlas)
		return;
	CCString*str=CCString::createWithFormat("%d",MyGame::getInstance()->m_gold);
	m_goldAtlas->setStringValue(str->getCString());

	CCString*strHammer=CCString::createWithFormat("%d",MyGame::getInstance()->m_HammerNum);
	m_hammerAtlas->setStringValue(strHammer->getCString());
	CCString*strColor=CCString::createWithFormat("%d",MyGame::getInstance()->m_colorBubbleNum);
	m_colorAtlas->setStringValue(strColor->getCString());
	CCString*strBomb=CCString::createWithFormat("%d",MyGame::getInstance()->m_bombNum);
	m_bombAtlas->setStringValue(strBomb->getCString());
	CCString*strGuide=CCString::createWithFormat("%d",MyGame::getInstance()->m_guidLineNum);
	m_guideAtlas->setStringValue(strGuide->getCString());
}

void UIStoreScene::keyBackClicked()
{
	CCDirector::sharedDirector()->popScene();
}
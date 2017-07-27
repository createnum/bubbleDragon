#include "uiBuyPropertyScene.h"
#include "myGame.h"
#include "uiStoreScene.h"
#include "publicDefine.h"
#include "SimpleAudioEngine.h"
#include "statisticsLog.h"
#include "GameScene.h"

#define UI_BUY_PROPERTY_JSON "ui/buyPropertyItem.json"
#define UI_BUY_PROPERTY_ROOT_IMAGE "Image_40"
#define UI_BUY_PROPERTY_CURNUM_IMAGE "Image_47"

#define UI_BUY_PROPERTY_IMAGE_NAME "propertyImage"
#define UI_BUY_PROPERTY_ICON_IMAGE "Image_44"
#define UI_BUY_PROPERTY_ICON_TTF "Image_45"

#define UI_BUY_PROPERTY_CURNUM_NAME "propertyNum"

#define UI_BUY_PROPERTY_CLOSE_BUTTON_NAME "closeButton"
#define UI_BUY_PROPERTY_PLUS_BUTTON_NAME "plusButton"
#define UI_BUY_PROPERTY_SUBTRACT_BUTTON_NAME "subtractButton"
#define UI_BUY_PROPERTY_BUY_BUTTON_NAME "buyButton"
#define UI_BUY_PROPERTY_BUY_BUTTON_NUM "goldNumCost"

#define UI_STORE_SCENE_HAMMER_BUTTON_NAME "Button_37"
#define UI_STORE_SCENE_BOMB_BUTTON_NAME "Button_47"
#define UI_STORE_SCENE_GUIDE_BUTTON_NAME "Button_38"
#define UI_STORE_SCENE_COLOR_BUTTON_NAME "Button_50"

#define HAMMER_IMAGE "ui/myRes/daojuchuizi.png"
#define HAMMER_TTF "ui/myRes/wenzixiaomuchui.png"

#define COLOR_IMAGE "ui/myRes/daojucaiqiu.png"
#define COLOR_TTF "ui/myRes/wenzicaiqiu.png"

#define GUIDE_IMAGE "ui/myRes/daojuyindaoxian.png"
#define GUIDE_TTF "ui/myRes/wenzimiaozhunxian.png"

#define BOMB_IMAGE "ui/myRes/daojuzhadan.png"
#define BOMB_TTF "ui/myRes/wenzizhadan.png"

USING_NS_CC;
using namespace gui;
using namespace CocosDenshion;

UIBuyPropertyScene::UIBuyPropertyScene(void)
{
	m_curNum=1;
	m_curPrice=0;
	m_countPrice=0;
}

UIBuyPropertyScene::~UIBuyPropertyScene(void)
{
	m_fileName->release();
}

UIBuyPropertyScene*UIBuyPropertyScene::create(const char*pFileName)
{
	UIBuyPropertyScene *pRet = new UIBuyPropertyScene(); 
	if (pRet && pRet->init(pFileName)) 
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

bool UIBuyPropertyScene::init(const char*pFileName)
{
	if (!UILayer::init())
	{
		return false;
	}
	UIWidget*pWidget=extension::GUIReader::shareReader()->widgetFromJsonFile(UI_BUY_PROPERTY_JSON);
	this->addWidget(pWidget);
	this->screenAdaptive(pWidget);

	UIImageView*rootImage=dynamic_cast<UIImageView*>(pWidget->getChildByName(UI_BUY_PROPERTY_ROOT_IMAGE));

	UIImageView*numImage=dynamic_cast<UIImageView*>(rootImage->getChildByName(UI_BUY_PROPERTY_CURNUM_IMAGE));
	m_numAtlas=dynamic_cast<UILabelAtlas*>(numImage->getChildByName(UI_BUY_PROPERTY_CURNUM_NAME));

	UIImageView*propertyImage=dynamic_cast<UIImageView*>(rootImage->getChildByName(UI_BUY_PROPERTY_IMAGE_NAME));
	m_curIconImage=dynamic_cast<UIImageView*>(propertyImage->getChildByName(UI_BUY_PROPERTY_ICON_IMAGE));
	m_curTTFImage=dynamic_cast<UIImageView*>(propertyImage->getChildByName(UI_BUY_PROPERTY_ICON_TTF));
	
	this->addTouchEventListener(rootImage);

	m_fileName=CCString::create(pFileName);
	m_fileName->retain();
	this->initData();
	
	this->updateNumAtlas();

	return true;
}

void UIBuyPropertyScene::screenAdaptive(cocos2d::gui::UIWidget*pWidget)
{
	CCSize size=CCDirector::sharedDirector()->getVisibleSize();
	pWidget->setSize(ccp(640,640*size.height/size.width));
}

void UIBuyPropertyScene::initData()
{
	if(m_fileName->compare(UI_STORE_SCENE_HAMMER_BUTTON_NAME)==0)
	{
		m_curIconImage->loadTexture(HAMMER_IMAGE);
		m_curTTFImage->loadTexture(HAMMER_TTF);
		m_curPrice=100;
		m_countPrice=100;
	}
	else if (m_fileName->compare(UI_STORE_SCENE_COLOR_BUTTON_NAME)==0)
	{
		m_curIconImage->loadTexture(COLOR_IMAGE);
		m_curTTFImage->loadTexture(COLOR_TTF);
		m_curPrice=100;
		m_countPrice=100;
	}
	else if (m_fileName->compare(UI_STORE_SCENE_GUIDE_BUTTON_NAME)==0)
	{
		m_curIconImage->loadTexture(GUIDE_IMAGE);
		m_curTTFImage->loadTexture(GUIDE_TTF);
		m_curPrice=80;
		m_countPrice=80;
	}
	else if (m_fileName->compare(UI_STORE_SCENE_BOMB_BUTTON_NAME)==0)
	{
		m_curIconImage->loadTexture(BOMB_IMAGE);
		m_curTTFImage->loadTexture(BOMB_TTF);
		m_curPrice=100;
		m_countPrice=100;
	}
}

void UIBuyPropertyScene::addTouchEventListener(UIWidget*pWidget)
{
	UIButton*closeButton=dynamic_cast<UIButton*>(pWidget->getChildByName(UI_BUY_PROPERTY_CLOSE_BUTTON_NAME));
	closeButton->addTouchEventListener(this,toucheventselector(UIBuyPropertyScene::touchCallBack));
	UIButton*plusButton=dynamic_cast<UIButton*>(pWidget->getChildByName(UI_BUY_PROPERTY_PLUS_BUTTON_NAME));
	plusButton->addTouchEventListener(this,toucheventselector(UIBuyPropertyScene::touchCallBack));
	UIButton*subtractButton=dynamic_cast<UIButton*>(pWidget->getChildByName(UI_BUY_PROPERTY_SUBTRACT_BUTTON_NAME));
	subtractButton->addTouchEventListener(this,toucheventselector(UIBuyPropertyScene::touchCallBack));
	UIButton*buyButton=dynamic_cast<UIButton*>(pWidget->getChildByName(UI_BUY_PROPERTY_BUY_BUTTON_NAME));
	buyButton->addTouchEventListener(this,toucheventselector(UIBuyPropertyScene::touchCallBack));
	m_buyCosts=dynamic_cast<UILabelAtlas*>(buyButton->getChildByName(UI_BUY_PROPERTY_BUY_BUTTON_NUM));
}

void UIBuyPropertyScene::disposeTouchEvent(cocos2d::gui::UIWidget*pWidget)
{
	if (MyGame::getInstance()->isGameEffectAbled())
		SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_BUTTON_EFFECT);
	const char*name=pWidget->getName();
	if (strcmp(name,UI_BUY_PROPERTY_CLOSE_BUTTON_NAME)==0)
	{
		this->removeFromParentAndCleanup(true);
	} 
	else if(strcmp(name,UI_BUY_PROPERTY_PLUS_BUTTON_NAME)==0)
	{
		if (m_countPrice + m_curPrice <= MyGame::getInstance()->m_gold){
			m_curNum++;
			m_countPrice += m_curPrice;
			this->updateNumAtlas();
		}
	}
	else if(strcmp(name,UI_BUY_PROPERTY_SUBTRACT_BUTTON_NAME)==0)
	{
		if(m_curNum-1<1)
			return;
		else
		{
			m_curNum--;
			m_countPrice-=m_curPrice;
			this->updateNumAtlas();
		}
	}
	else if (strcmp(name,UI_BUY_PROPERTY_BUY_BUTTON_NAME)==0)
	{
		if(MyGame::getInstance()->m_gold-m_countPrice<0)
		{
			this->removeFromParentAndCleanup(true);
		}
		else
		{
			MyGame::getInstance()->addGold(-m_countPrice);
			this->buySuccessed();
			UIStoreScene*store = dynamic_cast<UIStoreScene*>(this->getParent());
			if (store)store->initData();
			if (GameScene::getInstance()) GameScene::getInstance()->setPropertyNumTTF();
			this->removeFromParentAndCleanup(true);
		}
	}
}

void UIBuyPropertyScene::touchCallBack(CCObject*pSender,TouchEventType pType)
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

void UIBuyPropertyScene::updateNumAtlas()
{
	if(!m_numAtlas)
		return;
	CCString*str=CCString::createWithFormat("%d",m_curNum);
	m_numAtlas->setStringValue(str->getCString());

	CCString*strTemp=CCString::createWithFormat("%d",m_countPrice);
	m_buyCosts->setStringValue(strTemp->getCString());
}

void UIBuyPropertyScene::buySuccessed()
{
	if(m_fileName->compare(UI_STORE_SCENE_HAMMER_BUTTON_NAME)==0)
	{
		CCString*str = CCString::createWithFormat("%d", m_curNum);
		StatisticsLog::getInstance()->sendLogData("BuyHammer", str->getCString());
		MyGame::getInstance()->m_HammerNum+=m_curNum;
	}
	else if (m_fileName->compare(UI_STORE_SCENE_COLOR_BUTTON_NAME)==0)
	{
		CCString*str = CCString::createWithFormat("%d", m_curNum);
		StatisticsLog::getInstance()->sendLogData("BuyColorBubble", str->getCString());
		MyGame::getInstance()->m_colorBubbleNum+=m_curNum;
	}
	else if (m_fileName->compare(UI_STORE_SCENE_GUIDE_BUTTON_NAME)==0)
	{
		CCString*str = CCString::createWithFormat("%d", m_curNum);
		StatisticsLog::getInstance()->sendLogData("BuyGuideLine", str->getCString());
		MyGame::getInstance()->m_guidLineNum+=m_curNum;
	}
	else if (m_fileName->compare(UI_STORE_SCENE_BOMB_BUTTON_NAME)==0)
	{
		CCString*str = CCString::createWithFormat("%d", m_curNum);
		StatisticsLog::getInstance()->sendLogData("BuyBomb", str->getCString());
		MyGame::getInstance()->m_bombNum+=m_curNum;
	}
}
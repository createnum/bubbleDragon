#include "StageSelectScene.h"
#include "StartScene.h"
#include "publicDefine.h"
#include "gameScene.h"
#include "uiDeblockingScene.h"
#include "XMLTools.h"
#include "myGame.h"
#include "SimpleAudioEngine.h"
#include "statisticsLog.h"

#define UI_STAGE_SELECT_JSON "ui/slectStage.json"

#define UI_STAGE_SELECT_BUTTON_JSON "ui/stageSelectButton.json"
#define UI_STAGE_SELECT_BUTTON_NAME "SelectButtonName"
#define UI_STAGE_SELECT_BUTTON_NUM "stageNum"
#define UI_STAGE_SELECT_PAGEVIEW_NAME "PageView_16"
#define UI_PATH   "ui/gameScene.json"
#define UI_STAGE_SELECT_BG_IMAGE_NAME "Image_29_0"

#define UI_STAGE_SELECT_PAGE_CUR_NAME "ui/myRes/fanyeliang.png"
#define UI_STAGE_SELECT_PAGE_NAME "ui/myRes/fanyean.png"
#define UI_STAGE_SELECT_BUTTON_LOCK "ui/myRes/xuangaunan.png"
#define UI_STAGE_SELECT_STAR_LINGHT_PNG "ui/myRes/xingxingxiao.png"
#define UI_STAGE_SELECT_SMALL_LOCK_PNG "ui/myRes/suoxiao.png"

#define UI_BACK_BUTTON_NAME "goBackButton"

#define UI_STAGE_CONFIGER_FILE "stageConfig.xml"

#define UI_STAGE_BUTTON_ONESTAR_NAME "Image_1"
#define UI_STAGE_BUTTON_TWOSTAR_NAME "Image_2"
#define UI_STAGE_BUTTON_THREESTAR_NAME "Image_3"
#define UI_PAGE_STAR_COUNT "startCount"


using namespace gui;
using namespace CocosDenshion;

StageSelectScene::StageSelectScene()
{
	m_lastStage=0;
	m_curPage=0;
	m_stageNum=0;
	startPos = STAGE_BUTTON_START_POS;
	startPos.y*=CCDirector::sharedDirector()->getVisibleSize().height/960.0f;
	m_curPageImage=NULL;
	m_starAtlas=NULL;
}

StageSelectScene::~StageSelectScene()
{

}

CCScene*StageSelectScene::scene()
{
	CCScene*pScene=CCScene::create();
	pScene->addChild(StageSelectScene::create());
	return pScene;
}

bool StageSelectScene::init()
{
	if(!UILayer::init())
		return false;
	UIWidget*widget=extension::GUIReader::shareReader()->widgetFromJsonFile(UI_STAGE_SELECT_JSON);
	this->addWidget(widget);
	this->screenAdaptive(widget);

	UIButton*pBackButton=dynamic_cast<UIButton*>(widget->getChildByName(UI_BACK_BUTTON_NAME));
	pBackButton->addTouchEventListener(this,toucheventselector(StageSelectScene::buttonSelectCallBack));

	m_starAtlas=dynamic_cast<LabelAtlas*>(widget->getChildByName(UI_PAGE_STAR_COUNT));

	m_pageView = dynamic_cast<UIPageView*>(widget->getChildByName(UI_STAGE_SELECT_PAGEVIEW_NAME));
	this->initStagesButton(m_pageView);
	this->createPageLabel(widget);
	StatisticsLog::getInstance()->sendLogData("StageSelect");
	this->setKeypadEnabled(true);
	return true;
}

void StageSelectScene::screenAdaptive(cocos2d::gui::UIWidget*pWidget)
{
	CCSize size=CCDirector::sharedDirector()->getVisibleSize();
	pWidget->setSize(ccp(640,640*size.height/size.width));
}

void StageSelectScene::buttonSelectCallBack(CCObject*pSender,TouchEventType pType)
{
	CCLog("button callback");
	switch (pType)
	{
	case TOUCH_EVENT_BEGAN:
		break;
	case TOUCH_EVENT_MOVED:
		break;
	case TOUCH_EVENT_ENDED:
		{
			Widget*pWidget=(Widget*)pSender;
			this->disposeTouchEvent(pWidget);
		}
		break;
	default:
		break;
	}
}

void StageSelectScene::disposeTouchEvent(UIWidget*pWidget)
{
	if (MyGame::getInstance()->isGameEffectAbled())
		SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_BUTTON_EFFECT);
	const char*senderName=pWidget->getName();
	if(strcmp(senderName,UI_BACK_BUTTON_NAME)==0)
	{
		CCDirector::sharedDirector()->replaceScene(StartLayer::scene());
	}
	else if (strcmp(senderName,UI_STAGE_SELECT_BUTTON_NAME)==0)
	{
		LabelAtlas*stagenum=dynamic_cast<LabelAtlas*>(pWidget->getChildByName(UI_STAGE_SELECT_BUTTON_NUM));
		CCString str=stagenum->getStringValue();
		int num=str.intValue();
		CCLog("num is:%d",num);
		stageInfo*info=XMLTools::getInstance()->getStageInfoByIndex(num);
		//if(!info->m_isSpecialStage)
		//	CCDirector::sharedDirector()->replaceScene(GameScene::scene(num,false));
		//else
		//{
		//	//CCDirector::sharedDirector()->pushScene(UIDeblockingScene::scene());
		//	this->addChild(UIDeblockingScene::create());
		//}
		CCDirector::sharedDirector()->replaceScene(GameScene::scene(num, false));
	}
}

void StageSelectScene::initStagesButton(UIWidget*pWidget)
{
	std::vector<stageInfo>array=XMLTools::getInstance()->getStageInfo();
	cocos2d::CCLog("StageSelectScene::initStagesButton()---array.size:%d",array.size());
	//XMLTools::getInstance()->modifyXML("stageConfig.xml",array);
	if(array.empty())
		return;
	m_stageNum=array.size();
	m_pageNum= (m_stageNum - 1) / NUM_OF_ONE_PAGE + 1;//Ò³Êý
	int num=0;
	for (int i = 0; i < m_pageNum;i++)
	{
		UILayout*layerOut = UILayout::create();
		layerOut->setContentSize(m_pageView->getContentSize());
		layerOut->setPosition(ccp(0,40));
		for (int j = 0; j < NUM_OF_ONE_PAGE;j++)
		{
			if(i*NUM_OF_ONE_PAGE+j<array.size())
			{
				UIWidget*button = createStageButton(array[i*NUM_OF_ONE_PAGE+j],num);
				button->setPosition(calculatePosByNum(j));
				layerOut->addChild(button);
			}
		}
		m_pageStar.push_back(num);
		m_pageView->addPage(layerOut);
	}

	m_curPage=m_lastStage/NUM_OF_ONE_PAGE;
	m_pageView->scrollToPage(m_curPage);
	this->setPageStarAtlas(m_curPage);
}

void StageSelectScene::createPageLabel(cocos2d::gui::UIWidget*pWidget)
{
	m_pageNum = (m_stageNum - 1) / NUM_OF_ONE_PAGE + 1;//Ò³Êý
	CCPoint temp=STAGE_SELECT_PAGE_LABEL_STARTPOS;
	startTemp=ccp(temp.x-(m_pageNum-1)*STAGE_SELECT_PAGE_LABEL_SPACEING/2,temp.y*CCDirector::sharedDirector()->getVisibleSize().height/960.0f);
	for (int i=0;i<m_pageNum;i++)
	{
		UIImageView*imageView=UIImageView::create();
		imageView->loadTexture(UI_STAGE_SELECT_PAGE_NAME);
		pWidget->addChild(imageView);
		imageView->setPosition(ccp(startTemp.x+STAGE_SELECT_PAGE_LABEL_SPACEING*i,startTemp.y));
	}
	if(m_curPageImage)
		m_curPageImage->removeFromParentAndCleanup(true);
	m_curPageImage=UIImageView::create();
	m_curPageImage->loadTexture(UI_STAGE_SELECT_PAGE_CUR_NAME);
	m_curPageImage->setPosition(ccp(startTemp.x+STAGE_SELECT_PAGE_LABEL_SPACEING*m_curPage,startTemp.y));
	pWidget->addChild(m_curPageImage);
}

CCPoint StageSelectScene::calculatePosByNum(int num)
{
	int numOfStage = num / NUM_OF_ONE_PAGE;
	int numOfX = (num - numOfStage*NUM_OF_ONE_PAGE) % NUM_OF_Y_LINE;
	int numOfY = (num - numOfStage*NUM_OF_ONE_PAGE) / NUM_OF_X_LINE;

	float x = GAME_WIDCH*numOfStage + startPos.x + numOfX*STAGE_BUTTON_X_SPACEING;
	float y = startPos.y - numOfY*STAGE_BUTTON_Y_SPACEING*CCDirector::sharedDirector()->getVisibleSize().height/960.0f;

	return ccp(x, y);
}

UIWidget*StageSelectScene::createStageButton(stageInfo info,int &num)
{
	UIWidget*buttonWidget = cocos2d::extension::GUIReader::shareReader()->widgetFromJsonFile(UI_STAGE_SELECT_BUTTON_JSON);
	UIButton*button = dynamic_cast<UIButton*>(buttonWidget->getChildByName(UI_STAGE_SELECT_BUTTON_NAME));
	button->addTouchEventListener(this, toucheventselector(StageSelectScene::buttonSelectCallBack));
	LabelAtlas* buttonName = dynamic_cast<LabelAtlas*>(button->getChildByName(UI_STAGE_SELECT_BUTTON_NUM));
	char strNum[10] = { 0 };
	sprintf(strNum, "%d", info.m_stageNum);
	buttonName->setStringValue(strNum);

	UIImageView*oneImage=dynamic_cast<UIImageView*>(button->getChildByName(UI_STAGE_BUTTON_ONESTAR_NAME));
	UIImageView*twoImage=dynamic_cast<UIImageView*>(button->getChildByName(UI_STAGE_BUTTON_TWOSTAR_NAME));
	UIImageView*threeImage=dynamic_cast<UIImageView*>(button->getChildByName(UI_STAGE_BUTTON_THREESTAR_NAME));

	if(info.m_isLocked)
	{
		button->setBright(false);
		button->setTouchEnabled(false);
		oneImage->setVisible(false);
		twoImage->setVisible(false);
		threeImage->setVisible(false);
		buttonName->setVisible(false);
	}
	else 
	{
		if(info.m_isSpecialStage)
		{
			int curNum=info.m_stageNum/20*50;
			if(num<curNum)
			{
				UIImageView*imageView = UIImageView::create();
				imageView->loadTexture(UI_STAGE_SELECT_SMALL_LOCK_PNG);
				buttonWidget->addChild(imageView);
				oneImage->setVisible(false);
				twoImage->setVisible(false);
				threeImage->setVisible(false);
				buttonName->setVisible(false);
			}
			else
			{
				info.m_isSpecialStage = false;
				XMLTools::getInstance()->modifyStageInfoByInfo(&info);
				if (info.m_firstStar == 1)
				{
					oneImage->loadTexture(UI_STAGE_SELECT_STAR_LINGHT_PNG);
					num++;
				}
				if (info.m_secondStar == 1)
				{
					twoImage->loadTexture(UI_STAGE_SELECT_STAR_LINGHT_PNG);
					num++;
				}
				if (info.m_threeStar == 1)
				{
					threeImage->loadTexture(UI_STAGE_SELECT_STAR_LINGHT_PNG);
					num++;
				}
			}
		}
		else
		{
			if(info.m_firstStar==1)
			{
				oneImage->loadTexture(UI_STAGE_SELECT_STAR_LINGHT_PNG);
				num++;
			}
			if(info.m_secondStar==1)
			{
				twoImage->loadTexture(UI_STAGE_SELECT_STAR_LINGHT_PNG);
				num++;
			}
			if(info.m_threeStar==1)
			{
				threeImage->loadTexture(UI_STAGE_SELECT_STAR_LINGHT_PNG);
				num++;
			}
		}
		if(info.m_stageNum>m_lastStage)
			m_lastStage=info.m_stageNum;
	}
	return buttonWidget;
}

bool StageSelectScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	UILayer::ccTouchBegan(pTouch,pEvent);
	return true;
}
void StageSelectScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	UILayer::ccTouchMoved(pTouch,pEvent);
}
void StageSelectScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	UILayer::ccTouchEnded(pTouch,pEvent);
	m_curPage=m_pageView->getCurPageIndex();
	m_curPageImage->setPosition(ccp(startTemp.x+STAGE_SELECT_PAGE_LABEL_SPACEING*m_curPage,startTemp.y));
	this->setPageStarAtlas(m_curPage);
}

void StageSelectScene::keyBackClicked()
{
	CCDirector::sharedDirector()->replaceScene(StartLayer::scene());
}

void StageSelectScene::setPageStarAtlas(int curPage)
{
	if(!m_starAtlas)
		return;
	int num=m_pageStar[curPage];
	CCString*str=CCString::createWithFormat("%d/%d",num,3*NUM_OF_ONE_PAGE*(curPage+1));
	m_starAtlas->setStringValue(str->getCString());
}
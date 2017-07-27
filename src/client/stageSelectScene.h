/****************************************************************************
 FileName : stageSelectScene.h
 Creator	: chen mengjie
 Date		: 2015.6.1 9:30
 Comment : πÿø®—°‘Ò¿‡
*****************************************************************************/

#ifndef __STAGE_SELECT_SCENE_H__
#define __STAGE_SELECT_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>

USING_NS_CC;
USING_NS_CC_EXT;

struct stageInfo;

class StageSelectScene:public cocos2d::gui::UILayer
{
public:
	StageSelectScene();
	~StageSelectScene();

	virtual bool init();
	static CCScene*scene();
	CREATE_FUNC(StageSelectScene);

	void setPageStarAtlas(int num);
	CCPoint calculatePosByNum(int num);
	void initStagesButton(cocos2d::gui::UIWidget*pWidget);
	void createPageLabel(cocos2d::gui::UIWidget*pWidget);
	cocos2d::gui::UIWidget*createStageButton(stageInfo info,int &num);
	void buttonSelectCallBack(CCObject* pSender,cocos2d::gui::TouchEventType pType);
	void disposeTouchEvent(cocos2d::gui::UIWidget*pWidget);

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	void screenAdaptive(cocos2d::gui::UIWidget*pWidget);

	void keyBackClicked();
private:
	int m_stageNum;
	int m_pageNum;
	int m_lastStage;
	CCPoint startPos;
	int m_curPage;
	CCPoint startTemp;
	std::vector<int>m_pageStar;
	cocos2d::gui::UIImageView*m_curPageImage;
	cocos2d::gui::UIPageView*m_pageView;
	cocos2d::gui::LabelAtlas*m_starAtlas;
};
#endif//__STAGE_SELECT_SCENE_H__


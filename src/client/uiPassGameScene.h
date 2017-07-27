/****************************************************************************
 FileName : uiPassGameScene.h
 Creator	: chen mengjie
 Date		: 2015.6.3 15:32
 Comment : 游戏结束弹出UI，根据游戏结果弹出不同UI
*****************************************************************************/

#ifndef _UI_PASS_GAME_SCENE_H_
#define _UI_PASS_GAME_SCENE_H_

#include "cocos2d.h"
#include "cocos-ext.h"

struct stageInfo;
class UIPassGame:public cocos2d::gui::UILayer
{
public:
	UIPassGame(void);
	~UIPassGame(void);
public:
	virtual bool init(bool passSuccess,int stageNum,int score,int starNum);
	static cocos2d::CCScene*scene();
	static UIPassGame*create(bool passSuccess,int stageNum,int score,int starNum);
	void disposeTouchEvent(cocos2d::gui::UIWidget*pWidget);
	void touchCallBack(cocos2d::CCObject*pSender,cocos2d::gui::TouchEventType pType);
	stageInfo*unlockMapStage();//解锁关卡
private:
	bool m_passResult;
	int m_curScore;
	int m_curStage;
};

#endif//_UI_HELP_SCENE_H_


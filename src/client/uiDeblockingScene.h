/****************************************************************************
 FileName : uiDeblockingScene.h
 Creator	: chen mengjie
 Date		: 2015.6.4 9:20
 Comment : 关卡解锁UI，点击未解锁关卡 弹出
*****************************************************************************/

#ifndef _UI_DEBLOCKING_SCENE_H_
#define _UI_DEBLOCKING_SCENE_H_

#include "cocos2d.h"
#include "cocos-ext.h"

class UIDeblockingScene:public cocos2d::gui::UILayer
{
public:
	UIDeblockingScene(void);
	~UIDeblockingScene(void);
public:
	virtual bool init();
	static cocos2d::CCScene*scene();
	CREATE_FUNC(UIDeblockingScene);
	void touchCallBack(cocos2d::CCObject*pSender,cocos2d::gui::TouchEventType pType);
	void disposeTouchEvent(cocos2d::gui::UIWidget*pWidget);
};

class NewPlayerGuide:public cocos2d::gui::UILayer
{
public:
	NewPlayerGuide();
	~NewPlayerGuide();
public:
	virtual bool init();
	static cocos2d::CCScene*scene();
	CREATE_FUNC(NewPlayerGuide);
	void touchCallBack(cocos2d::CCObject*pSender, cocos2d::gui::TouchEventType pType);
	void disposeTouchEvent(cocos2d::gui::UIWidget*pWidget);
};

#endif//_UI_DEBLOCKING_SCENE_H_


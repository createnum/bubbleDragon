#ifndef _UI_RESURRECTION_SCENE_H_
#define _UI_RESURRECTION_SCENE_H_

#include "cocos2d.h"
#include "cocos-ext.h"

class UIResurrectionScene:public cocos2d::gui::UILayer
{
public:
	UIResurrectionScene(void);
	~UIResurrectionScene(void);
public:
	virtual bool init(int resOrHot);
	static UIResurrectionScene*create(int resOrHot);
	void touchCallBack(cocos2d::CCObject*pSender,cocos2d::gui::TouchEventType pType);
	void addTouchEventListener(cocos2d::gui::UIWidget*pWidget);
	void disposeTouchEvent(cocos2d::gui::UIWidget*pWidget);
	void screenAdaptive(cocos2d::gui::UIWidget*pWidget);
private:
	int pSceneType;
};

#endif//_UI_RESURRECTION_SCENE_H_


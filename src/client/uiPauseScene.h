#ifndef _UI_PAUSE_SCENE_H_
#define _UI_PAUSE_SCENE_H_

#include "cocos2d.h"
#include "cocos-ext.h"

class UIPauseScene:public cocos2d::gui::UILayer
{
public:
	UIPauseScene(void);
	~UIPauseScene(void);
public:
	virtual bool init();
	static cocos2d::CCScene*scene();
	CREATE_FUNC(UIPauseScene);
	void touchCallBack(cocos2d::CCObject*pSender,cocos2d::gui::TouchEventType pType);
	void disposeTouchEvent(cocos2d::gui::UIWidget*pWidget);
	void addTouchEventListener(cocos2d::gui::UIWidget*pWidget);
	void screenAdaptive(cocos2d::gui::UIWidget*pWidget);
};


#endif//_UI_PAUSE_SCENE_H_


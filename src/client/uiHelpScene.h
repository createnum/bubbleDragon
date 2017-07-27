#ifndef _UI_HELP_SCENE_H_
#define _UI_HELP_SCENE_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "HelpItemDict.h"

class UIHelpScene:public cocos2d::gui::UILayer
{
public:
	UIHelpScene(void);
	~UIHelpScene(void);
public:
	virtual bool init();
	static cocos2d::CCScene*scene();
	CREATE_FUNC(UIHelpScene);
	void touchCallBack(cocos2d::CCObject*pSender,cocos2d::gui::TouchEventType pType);
	void keyBackClicked();
	void screenAdaptive(cocos2d::gui::UIWidget*pWidget);
public:
	void addTouchEventListener(cocos2d::gui::UIWidget*pWidget);
	void disposeTouchEvent(cocos2d::gui::UIWidget*pWidget);
	void createHelpItemsOnScrollView(cocos2d::gui::UIWidget*pWidget);
	cocos2d::gui::UIWidget*createImageViewWithInfo(HelpItem info);
private:
	std::vector<HelpItem>m_helpItems;
};

#endif//_UI_HELP_SCENE_H_


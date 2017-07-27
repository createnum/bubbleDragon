#ifndef __START_SCENE_H__
#define __START_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

class StartLayer : public cocos2d::gui::UILayer
{
public:
	StartLayer();
	~StartLayer();
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
    CREATE_FUNC(StartLayer);

	void buttonCallBack(CCObject*pSender,cocos2d::gui::TouchEventType pType);
	void addTouchEventListener(cocos2d::gui::UIWidget*pWidget);
	void touchEventDesponse(const char*name);
	void keyBackClicked();
	void screenAdaptive(cocos2d::gui::UIWidget*pWidget);
	void preLoadMusicFile();
	cocos2d::CCActionInterval*getButtonAction(float delay);
private:
	cocos2d::gui::UIButton*m_musicButton;
	cocos2d::gui::UIButton*m_effectButton;
	int m_nColor;
};

#endif  // __START_SCENE_H__
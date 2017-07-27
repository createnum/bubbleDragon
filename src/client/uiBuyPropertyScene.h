#ifndef _UI_BUY_PROPERTY_SCENE_H_
#define _UI_BUY_PROPERTY_SCENE_H_

#include "cocos2d.h"
#include "cocos-ext.h"

class UIBuyPropertyScene:public cocos2d::gui::UILayer
{
public:
	UIBuyPropertyScene(void);
	~UIBuyPropertyScene(void);
public:
	virtual bool init(const char*pFileName);
	static UIBuyPropertyScene*create(const char*pFileName);
	void touchCallBack(cocos2d::CCObject*pSender,cocos2d::gui::TouchEventType pType);
	void addTouchEventListener(cocos2d::gui::UIWidget*pWidget);
	void disposeTouchEvent(cocos2d::gui::UIWidget*pWidget);
	void updateNumAtlas();
	void initData();
	void buySuccessed();
	void screenAdaptive(cocos2d::gui::UIWidget*pWidget);
private:
	cocos2d::gui::LabelAtlas*m_numAtlas;
	cocos2d::gui::LabelAtlas*m_buyCosts;
	cocos2d::CCString*m_fileName;
	int m_curNum;
	int m_curPrice;
	int m_countPrice;
	cocos2d::gui::UIImageView*m_curIconImage;
	cocos2d::gui::UIImageView*m_curTTFImage;
};

#endif//_UI_BUY_PROPERTY_SCENE_H_


#ifndef _UI_STORE_SCENE_H_
#define _UI_STORE_SCENE_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "storeBuyItenDict.h"
#include <vector>

class UIStoreScene:public cocos2d::gui::UILayer
{
public:
	UIStoreScene(void);
	~UIStoreScene(void);
public:
	static UIStoreScene*getInstance();
	virtual bool init();
	static cocos2d::CCScene*scene();
	CREATE_FUNC(UIStoreScene);
	void touchCallBack(cocos2d::CCObject*pSender,cocos2d::gui::TouchEventType pType);
	void getInfoFromXml(char*pFileName);
	void createBuyItemsOnScrollView(cocos2d::gui::UIWidget*pWidget);
	cocos2d::gui::UIWidget*createImageViewWithInfo(int gold,int rmb);
	void disposeTouchEvent(cocos2d::gui::UIWidget*pWidget);
	void addTouchEventListener(cocos2d::gui::UIWidget*pWidget);
	void initData();
	virtual void keyBackClicked();
	void screenAdaptive(cocos2d::gui::UIWidget*pWidget);
private:
	std::vector<BuyItem>buyItems;
	cocos2d::gui::LabelAtlas*m_goldAtlas;
	cocos2d::gui::LabelAtlas*m_hammerAtlas;
	cocos2d::gui::LabelAtlas*m_colorAtlas;
	cocos2d::gui::LabelAtlas*m_bombAtlas;
	cocos2d::gui::LabelAtlas*m_guideAtlas;
	static UIStoreScene*m_inst;
};


#endif//_UI_STORE_SCENE_H_


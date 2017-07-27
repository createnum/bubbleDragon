#include "uiHelpScene.h"
#include "myGame.h"
#include "publicDefine.h"
#include "SimpleAudioEngine.h"
#include "GamePay.h"

#define UI_HELP_SCENE_JSON "ui/helpUI.json"
#define UI_HELP_SCENE_BACK_BUTTON_NAME "Button_154"
#define UI_HELP_SCENE_ONE_BUTTON_NAME "Button_41"
#define UI_HELP_SCENE_TEN_BUTTON_NAME "Button_40"

#define UI_HELP_SCENE_SCROLLVIEW_NAME "ScrollView_17"
#define UI_HELP_SCENE_BG_IMAGE_NAME "Image_6"

#define UI_HELP_ITEM_JSON "ui/helpItem.json"
#define UI_HELP_ITEM_IMAGE "Image_9_3"
#define UI_HELP_ITEM_IMAGE_PATH "Image_10"
#define UI_HELP_ITEM_DECLARE_PATH "Image_40"
#define UI_HELP_ITEM_DECLARE_CONTENT "Label_42"

#define UI_HELP_ITEM_HEIGHT 175.0f
USING_NS_CC;
using namespace gui;
using namespace CocosDenshion;

UIHelpScene::UIHelpScene(void)
{
	//CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/myRes/paopao0.plist","ui/myRes/paopao0.png");
}


UIHelpScene::~UIHelpScene(void)
{
	//CCSpriteFrameCache::sharedSpriteFrameCache()->purgeSharedSpriteFrameCache();
}

CCScene*UIHelpScene::scene()
{
	CCScene*pScene=CCScene::create();
	UIHelpScene*player=UIHelpScene::create();
	pScene->addChild(player);
	return pScene;
}

bool UIHelpScene::init()
{
	if (!UILayer::init())
	{
		return false;
	}
	UIWidget*pWidget=extension::GUIReader::shareReader()->widgetFromJsonFile(UI_HELP_SCENE_JSON);
	this->addWidget(pWidget);
	this->screenAdaptive(pWidget);

	this->addTouchEventListener(pWidget);

	UIScrollView*pScrollView=dynamic_cast<UIScrollView*>(pWidget->getChildByName(UI_HELP_SCENE_SCROLLVIEW_NAME));
	this->createHelpItemsOnScrollView(pScrollView);

	this->setKeypadEnabled(true);

	return true;
}

void UIHelpScene::screenAdaptive(cocos2d::gui::UIWidget*pWidget)
{
	CCSize size=CCDirector::sharedDirector()->getVisibleSize();
	pWidget->setSize(ccp(640,640*size.height/size.width));
}

void UIHelpScene::touchCallBack(CCObject*pSender,TouchEventType pType)
{
	switch(pType)
	{
	case TOUCH_EVENT_BEGAN:
		break;
	case TOUCH_EVENT_MOVED:
		break;
	case TOUCH_EVENT_ENDED:
		{
			UIWidget*pWidget=(UIWidget*)pSender;
			this->disposeTouchEvent(pWidget);
		}
		break;
	}
}

void UIHelpScene::addTouchEventListener(UIWidget*pWidget)
{
	UIButton*pButton = dynamic_cast<UIButton*>(pWidget->getChildByName(UI_HELP_SCENE_BACK_BUTTON_NAME));
	pButton->addTouchEventListener(this, toucheventselector(UIHelpScene::touchCallBack));
	CCPoint p1 = pButton->getPosition();

	UIButton*pOnePenny = dynamic_cast<UIButton*>(pWidget->getChildByName(UI_HELP_SCENE_ONE_BUTTON_NAME));
	pOnePenny->addTouchEventListener(this, toucheventselector(UIHelpScene::touchCallBack));
	CCPoint p2 = pOnePenny->getPosition();

	UIButton*pTenPenny = dynamic_cast<UIButton*>(pWidget->getChildByName(UI_HELP_SCENE_TEN_BUTTON_NAME));
	pTenPenny->addTouchEventListener(this, toucheventselector(UIHelpScene::touchCallBack));
	CCPoint p3 = pTenPenny->getPosition();
}

void UIHelpScene::disposeTouchEvent(cocos2d::gui::UIWidget*pWidget)
{
	if (MyGame::getInstance()->isGameEffectAbled())
		SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_BUTTON_EFFECT);
	const char*name=pWidget->getName();
	if (strcmp(name,UI_HELP_SCENE_BACK_BUTTON_NAME)==0)
	{
		CCDirector::sharedDirector()->popScene();
	}
	else if (strcmp(name, UI_HELP_SCENE_ONE_BUTTON_NAME) == 0)
	{
		GamePay::getInstance()->payWithRmb(GamePay::getInstance()->rmbTransitionTargetID(1, PAY_OWNUSE));
	}
	else if (strcmp(name, UI_HELP_SCENE_TEN_BUTTON_NAME) == 0)
	{
		GamePay::getInstance()->payWithRmb(GamePay::getInstance()->rmbTransitionTargetID(10, PAY_OWNUSE));
	}
}

void UIHelpScene::createHelpItemsOnScrollView(UIWidget*pWidget)
{
	UIScrollView*pScrollView=(UIScrollView*)pWidget;
	m_helpItems=HelpItemDict::getInstance()->getHelpItems();
	CCLog("UIHelpScene::createHelpItemsOnScrollView helpItems.size:%d",m_helpItems.size());
	pScrollView->setInnerContainerSize(CCSizeMake(pScrollView->getContentSize().width, UI_HELP_ITEM_HEIGHT*m_helpItems.size()));
	for (int i=0;i<m_helpItems.size();i++)
	{
		UIWidget*imageView=createImageViewWithInfo(m_helpItems[i]);
		pScrollView->addChild(imageView);
	}
}

UIWidget*UIHelpScene::createImageViewWithInfo(HelpItem info)
{
	UIWidget*pWidget=extension::GUIReader::shareReader()->widgetFromJsonFile(UI_HELP_ITEM_JSON);

	UIImageView*pImage=dynamic_cast<UIImageView*>(pWidget->getChildByName(UI_HELP_ITEM_IMAGE));

	UIImageView*pImagePath=dynamic_cast<UIImageView*>(pImage->getChildByName(UI_HELP_ITEM_IMAGE_PATH));
	UIImageView*pDeclearPath=dynamic_cast<UIImageView*>(pImage->getChildByName(UI_HELP_ITEM_DECLARE_PATH));
	UILabel*pDeclearContent=dynamic_cast<UILabel*>(pImage->getChildByName(UI_HELP_ITEM_DECLARE_CONTENT));

	//pImagePath->loadTexture(info.imagePath.c_str(),UI_TEX_TYPE_PLIST);
	pImagePath->loadTexture(info.imagePath.c_str());
	pDeclearPath->loadTexture(info.declarePath.c_str());
	pDeclearContent->setText(info.declareContent);

	return pWidget;
}

void UIHelpScene::keyBackClicked()
{
	CCDirector::sharedDirector()->popScene();
}
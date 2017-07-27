#include "uiPauseScene.h"
#include "stageSelectScene.h"
#include "gameScene.h"
#include "myGame.h"
#include "publicDefine.h"
#include "SimpleAudioEngine.h"
#include "statisticsLog.h"

#define UI_PAUSE_SCENE_JSON "ui/PauseScene.json"
#define UI_PAUSE_SCENE_IMAGE "Image_1"
#define UI_PAUSE_SCENE_EFFECT_BUTTON "EffectButton"
#define UI_PAUSE_SCENE_MUSIC_BUTTON "MusicButton"
#define UI_PAUSE_SCENE_GOBACK_BUTTON "GoBack"
#define UI_PAUSE_SCENE_RETRESH_BUTTON "Refresh"
#define UI_PAUSE_SCENE_GOON_BUTTON "GoOn"

#define UI_MAIN_SCENE_MUSIC_ON "ui/myRes/yinyue.png"
#define UI_MAIN_SCENE_MUSIC_OFF "ui/myRes/yinyuean.png"
#define UI_MAIN_SCENE_EFFECT_ON "ui/myRes/yinxiao.png"
#define UI_MAIN_SCENE_EFFECT_OFF "ui/myRes/yinxiaoan.png"

USING_NS_CC;
using namespace gui;
using namespace CocosDenshion;


UIPauseScene::UIPauseScene(void)
{
	
}


UIPauseScene::~UIPauseScene(void)
{

}

CCScene*UIPauseScene::scene()
{
	CCScene*pScene=CCScene::create();
	UIPauseScene*player=UIPauseScene::create();
	pScene->addChild(player);
	return pScene;
}

bool UIPauseScene::init()
{
	if (!UILayer::init())
	{
		return false;
	}

	UIWidget*pWidget=extension::GUIReader::shareReader()->widgetFromJsonFile(UI_PAUSE_SCENE_JSON);
	this->addWidget(pWidget);
	this->screenAdaptive(pWidget);

	this->addTouchEventListener(pWidget);

	StatisticsLog::getInstance()->sendLogData("PauseGame");

	return true;
}

void UIPauseScene::screenAdaptive(cocos2d::gui::UIWidget*pWidget)
{
	CCSize size=CCDirector::sharedDirector()->getVisibleSize();
	pWidget->setSize(ccp(640,640*size.height/size.width));
}

void UIPauseScene::addTouchEventListener(UIWidget*pWidget)
{
	UIImageView*pImageView = dynamic_cast<UIImageView*>(pWidget->getChildByName(UI_PAUSE_SCENE_IMAGE));

	UIButton*pMusicButton = dynamic_cast<UIButton*>(pImageView->getChildByName(UI_PAUSE_SCENE_MUSIC_BUTTON));
	pMusicButton->addTouchEventListener(this,toucheventselector(UIPauseScene::touchCallBack));

	if(!MyGame::getInstance()->isGameMusicAbled())
		pMusicButton->loadTextureNormal(UI_MAIN_SCENE_MUSIC_OFF);

	UIButton*pEffectButton = dynamic_cast<UIButton*>(pImageView->getChildByName(UI_PAUSE_SCENE_EFFECT_BUTTON));
	pEffectButton->addTouchEventListener(this,toucheventselector(UIPauseScene::touchCallBack));

	if(!MyGame::getInstance()->isGameEffectAbled())
		pEffectButton->loadTextureNormal(UI_MAIN_SCENE_EFFECT_OFF);

	UIButton*pGoBackButton = dynamic_cast<UIButton*>(pImageView->getChildByName(UI_PAUSE_SCENE_GOBACK_BUTTON));
	pGoBackButton->addTouchEventListener(this,toucheventselector(UIPauseScene::touchCallBack));
	UIButton*pGoOnButton = dynamic_cast<UIButton*>(pImageView->getChildByName(UI_PAUSE_SCENE_GOON_BUTTON));
	pGoOnButton->addTouchEventListener(this,toucheventselector(UIPauseScene::touchCallBack));
	UIButton*pRefreshButton = dynamic_cast<UIButton*>(pImageView->getChildByName(UI_PAUSE_SCENE_RETRESH_BUTTON));
	pRefreshButton->addTouchEventListener(this,toucheventselector(UIPauseScene::touchCallBack));
}


void UIPauseScene::touchCallBack(CCObject*pSender,TouchEventType pType)
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

void UIPauseScene::disposeTouchEvent(UIWidget*pWidget)
{
	const char*name=pWidget->getName();
	UIButton*pButton=(UIButton*)pWidget;
	if (strcmp(name,UI_PAUSE_SCENE_MUSIC_BUTTON)==0)
	{
		if(MyGame::getInstance()->isGameMusicAbled())
		{
			MyGame::getInstance()->setGameMusic(false);
			SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			pButton->loadTextureNormal(UI_MAIN_SCENE_MUSIC_OFF);
		}
		else
		{
			MyGame::getInstance()->setGameMusic(true);
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_GAMESCENE_BG_MUSIC, true);
			pButton->loadTextureNormal(UI_MAIN_SCENE_MUSIC_ON);
		}
	} 
	else if(strcmp(name,UI_PAUSE_SCENE_EFFECT_BUTTON)==0)
	{
		if(MyGame::getInstance()->isGameEffectAbled())
		{
			MyGame::getInstance()->setGameEffect(false);
			pButton->loadTextureNormal(UI_MAIN_SCENE_EFFECT_OFF);
		}
		else
		{
			MyGame::getInstance()->setGameEffect(true);
			pButton->loadTextureNormal(UI_MAIN_SCENE_EFFECT_ON);
		}
	}
	else if(strcmp(name,UI_PAUSE_SCENE_GOBACK_BUTTON)==0)
	{
		StatisticsLog::getInstance()->sendLogData("PauseGameGoBack");

		if (MyGame::getInstance()->isGameMusicAbled())
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_STARTSCENE_BG_MUSIC, true);
		CCDirector::sharedDirector()->replaceScene(StageSelectScene::scene());
	}
	else if(strcmp(name,UI_PAUSE_SCENE_RETRESH_BUTTON)==0)
	{
		StatisticsLog::getInstance()->sendLogData("PauseGameRefresh");
		CCDirector::sharedDirector()->replaceScene(GameScene::scene(MyGame::getInstance()->getCurStage(),false));
	}
	else if(strcmp(name,UI_PAUSE_SCENE_GOON_BUTTON)==0)
	{
		StatisticsLog::getInstance()->sendLogData("PauseGameGoOn");
		this->removeFromParentAndCleanup(true);
	}
	if (MyGame::getInstance()->isGameEffectAbled())
		SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_BUTTON_EFFECT);
}
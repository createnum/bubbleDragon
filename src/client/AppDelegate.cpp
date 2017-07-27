#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "StartScene.h"
#include "SimpleAudioEngine.h"
#include "myGame.h"
#include "statisticsLog.h"
//#include "SimulatorTwoGameScene.h"

using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
	CCEGLView *pEGLView = CCEGLView::sharedOpenGLView();
    pDirector->setOpenGLView(pEGLView);

	pEGLView->setDesignResolutionSize(MyGame::CUR_STANDARD_WIDTH, MyGame::CUR_STANDARD_HEIGHT, kResolutionFixedWidth);
    
    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    // create a scene. it's an autorelease object
    CCScene *pScene = StartLayer::scene();		//进入游戏主界面
	//CCScene *pScene = SimulatorTwoGameScene::scene(0, 0);		//进入游戏主界面
    // run
    pDirector->runWithScene(pScene);
    CCLog("application Did Finish Launching finish");
	StatisticsLog::getInstance()->sendLogData("FinishLaunching");
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	StatisticsLog::getInstance()->sendLogData("EnterBackground");
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	StatisticsLog::getInstance()->sendLogData("EnterForeground");
}

#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
#include "myGame.h"
#include "newbeeUtils/logging.h"
#include "SimpleAudioEngine.h"
#include "statisticsLog.h"

USING_NS_CC;

// uncomment below line, open debug console
#define USE_WIN32_CONSOLE
#define COCOS2D_DEBUG 1

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	StatisticsLog log;
	std::string serverPath("http://ddz.geiliyou.com:10017/fishLog/newLog");//("http://192.168.254.76:8080/gm/newLog");
	std::string baseInfo("&appId=skt.board.billiardsEliminate&channelId=&version=2.3&phoneBrand=win32&phoneType=xp&systemVersion=xp&imsi=&key=1&operatorId=-1");
	std::string verifyInfo("skt.board.billiardsEliminate1");//appid+key
	log.init(serverPath, baseInfo, verifyInfo);
	log.sendLogData("winB","","");
	CCLog("main");
	// create the application instance

//#ifdef USE_WIN32_CONSOLE
//	AllocConsole();
//	freopen("CONIN$", "r", stdin);
//	freopen("CONOUT$", "w", stdout);
//	freopen("CONOUT$", "w", stderr);
//#endif

	// create the application instance
	AppDelegate app;
	CCEGLView* eglView = CCEGLView::sharedOpenGLView();
	eglView->setFrameSize(640,960);
	eglView->setFrameZoomFactor(0.8f);
	int ret = CCApplication::sharedApplication()->run();
	CocosDenshion::SimpleAudioEngine::end();

//#ifdef USE_WIN32_CONSOLE
//	FreeConsole();
//#endif

	return ret;
}

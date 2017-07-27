#include "uiPassGameScene.h"
#include "stageSelectScene.h"
#include "gameScene.h"
#include "uiStoreScene.h"
#include "XMLTools.h"
#include "myGame.h"
#include "AnimationTools.h"
#include "publicDefine.h"
#include "SimpleAudioEngine.h"
#include "statisticsLog.h"

#define UI_PASS_SUCCESS_SCENE_JSON "ui/passSuccess.json"
#define UI_PASS_FAILED_SCENE_JSON "ui/passFailed.json"

#define UI_PASS_ONE_STAR "Image_1"
#define UI_PASS_TWO_STAR "Image_2"
#define UI_PASS_THREE_STAR "Image_3"

#define UI_PASS_SCORE "AtlasLabel_7"
#define UI_PASS_HIGE_SCORE "AtlasLabel_7_0"

#define UI_PASS_BUTTON_BACK "Button_10"
#define UI_PASS_BUTTON_RETRY "Button_10_0"
#define UI_PASS_BUTTON_UNKNOWN "Button_10_1"

#define UI_PASS_STAR_PNG "ui/myRes/xingxing.png"

USING_NS_CC;
using namespace gui;
using namespace CocosDenshion;

UIPassGame::UIPassGame(void)
{
	m_curScore=0;
	m_passResult=false;
}


UIPassGame::~UIPassGame(void)
{
}

CCScene*UIPassGame::scene()
{
	CCScene*pScene=CCScene::create();
	//UIPassGame*player=UIPassGame::create();
	//pScene->addChild(player);
	return pScene;
}

UIPassGame*UIPassGame::create(bool passSuccess,int stageNum,int score,int starNum)
{
	UIPassGame *pRet = new UIPassGame(); 
	if (pRet && pRet->init(passSuccess,stageNum,score,starNum)) 
	{ 
		pRet->autorelease(); 
	} 
	else 
	{ 
		delete pRet; 
		pRet = NULL; 
	} 
	return pRet;
}
bool UIPassGame::init(bool passSuccess,int stageNum,int score,int starNum)
{
	if (!UILayer::init())
	{
		return false;
	}

	if (passSuccess){
		StatisticsLog::getInstance()->sendLogData("PassGameSuccess");
	}else{
		StatisticsLog::getInstance()->sendLogData("PassGameFailed");
	}

	int lines=MyGame::getInstance()->max_rows-6;
	m_passResult=passSuccess;
	m_curStage=stageNum;
	UIWidget*pWidget=NULL;
	{
		if(passSuccess)
		{
			pWidget=extension::GUIReader::shareReader()->widgetFromJsonFile(UI_PASS_SUCCESS_SCENE_JSON);
			UILabelAtlas*highScore=dynamic_cast<UILabelAtlas*>(pWidget->getChildByName(UI_PASS_HIGE_SCORE));
			stageInfo*pInfo=XMLTools::getInstance()->getStageInfoByIndex(stageNum);

			int pMax=pInfo->m_maxScore>score?pInfo->m_maxScore:score;
			highScore->setStringValue(CCString::createWithFormat("%d",pMax)->getCString());
		}
		else
		{
			pWidget=extension::GUIReader::shareReader()->widgetFromJsonFile(UI_PASS_FAILED_SCENE_JSON);
			//score=score/(15*MyGame::getInstance()->max_rows);
			//if(score>=100)
			//	score=97;
			float countNum=XMLTools::getInstance()->getStageCount();
			float unlockNum=XMLTools::getInstance()->getUnlockStageNum()-1;
			score=unlockNum/countNum*100.0f;
			std::cout << "score:" << score << std::endl;
			std::cout << "unlockNum:" << unlockNum << std::endl;
			std::cout << "countNum:" << countNum << std::endl;
		}
	}
	this->addWidget(pWidget);
	CCSize size=CCDirector::sharedDirector()->getVisibleSize();
	pWidget->setSize(ccp(640,640*size.height/size.width));

	UIButton*pBackButton=dynamic_cast<UIButton*>(pWidget->getChildByName(UI_PASS_BUTTON_BACK));
	pBackButton->addTouchEventListener(this,toucheventselector(UIPassGame::touchCallBack));
	UIButton*pRetryButton=dynamic_cast<UIButton*>(pWidget->getChildByName(UI_PASS_BUTTON_RETRY));
	pRetryButton->addTouchEventListener(this,toucheventselector(UIPassGame::touchCallBack));
	UIButton*pButton=dynamic_cast<UIButton*>(pWidget->getChildByName(UI_PASS_BUTTON_UNKNOWN));
	pButton->addTouchEventListener(this,toucheventselector(UIPassGame::touchCallBack));

	char str[100]={0};
	sprintf(str,"%d",score);

	UILabelAtlas*scoreAtlas=dynamic_cast<UILabelAtlas*>(pWidget->getChildByName(UI_PASS_SCORE));
	scoreAtlas->setStringValue(str);

	if(passSuccess)
	{
		for (int i=0;i<starNum;i++)
		{
			sprintf(str,"Image_%d",i+1);
			UIImageView*pStarImage=dynamic_cast<UIImageView*>(pWidget->getChildByName(str));
			UIImageView*pTempImage=UIImageView::create();
			pTempImage->loadTexture(UI_PASS_STAR_PNG);
			pTempImage->setScale(0.0f);
			pTempImage->setPosition(pStarImage->getPosition());
			pTempImage->setRotation(pStarImage->getRotation());
			pWidget->addChild(pTempImage);
			CCSequence*seq=CCSequence::create(CCDelayTime::create(0.3f*i),CCScaleTo::create(0.6f,1.2f),CCScaleTo::create(0.2f,1.0f),CCScaleTo::create(0.1f,1.1f),CCScaleTo::create(0.05f,1.0f),NULL);
			pTempImage->runAction(seq);
			CCParticleSystem*particleAmation=AnimationTools::getInstance()->createParticleAnimation("ui/animations/passGameArmature/smallStar.plist",pStarImage->getPosition());
			//particleAmation->setAutoRemoveOnFinish(true);
			pWidget->addNode(particleAmation,-1);
			
		}
		XMLTools::getInstance()->updateMapStageInfo(stageNum, starNum, score);

		{
			CCPoint tempPoint = ccp(328, 657);
			tempPoint.y = tempPoint.y*size.height / 960.0f;
			CCArmature*pArmature = AnimationTools::getInstance()->createAnimationWithExportJson("ui/animations/passGameArmature/guoguanXzTX.ExportJson", "guoguanXzTX", tempPoint);
			pArmature->setScale(1.5f);
			pWidget->addNode(pArmature, -1);
		}
	}
	
	//UIImageView*pStarImage=dynamic_cast<UIImageView*>(pWidget->getChildByName("Image_2"));
	

	return true;
}

void UIPassGame::touchCallBack(CCObject*pSender,TouchEventType pType)
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

void UIPassGame::disposeTouchEvent(UIWidget*pWidget)
{
	if (MyGame::getInstance()->isGameEffectAbled())
		SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_BUTTON_EFFECT);
	const char*name=pWidget->getName();
	if (strcmp(name,UI_PASS_BUTTON_BACK)==0)
	{
		if (m_passResult){
			StatisticsLog::getInstance()->sendLogData("PassGameSuccessGoBack");
		}
		else{
			StatisticsLog::getInstance()->sendLogData("PassGameFailedGoBack");
		}
		if (MyGame::getInstance()->isGameMusicAbled())
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_STARTSCENE_BG_MUSIC);
		CCDirector::sharedDirector()->replaceScene(StageSelectScene::scene());
	} 
	else if(strcmp(name,UI_PASS_BUTTON_RETRY)==0)
	{
		if (m_passResult){
			StatisticsLog::getInstance()->sendLogData("PassGameSuccessRetry");
		}
		else{
			StatisticsLog::getInstance()->sendLogData("PassGameFailedRetry");
		}
		this->removeFromParentAndCleanup(true);
		int curNum=GameScene::getInstance()->m_stage;
		CCDirector::sharedDirector()->replaceScene(GameScene::scene(curNum,false));
	}
	else if(strcmp(name,UI_PASS_BUTTON_UNKNOWN)==0)
	{
		if (m_passResult){
			StatisticsLog::getInstance()->sendLogData("PassGameSuccessNext");
		}
		else{
			StatisticsLog::getInstance()->sendLogData("PassGameFailedStore");
		}

		CCNode*pFarther=this->getParent();
		if(m_passResult)
		{
			this->removeFromParentAndCleanup(true);
			m_curStage=GameScene::getInstance()->m_stage+1;
			if (m_curStage > XMLTools::getInstance()->getStageCount())
				CCDirector::sharedDirector()->replaceScene(StageSelectScene::scene());
			else
				CCDirector::sharedDirector()->replaceScene(GameScene::scene(m_curStage, false));
		}
		else
		{
			CCDirector::sharedDirector()->pushScene(UIStoreScene::scene());
		}
	}
}

stageInfo*UIPassGame::unlockMapStage()
{
	stageInfo*info=new stageInfo();
	info->m_stageNum=m_curStage;
	info->m_isLocked=false;
	return info;
}
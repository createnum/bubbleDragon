#include "GameScene.h"
#include "MyGame.h"
#include "Utility.h"
#include <algorithm>
#include "StartScene.h"
#include "stageMapDict.h"
#include "stageSelectScene.h"
#include "uiPassGameScene.h"
#include "publicDefine.h"
#include "sprite_nodes\CCSprite.h"
#include "CommonFunc.h"
#include "XMLTools.h"
#include "AnimationTools.h"
#include<cstdlib>
#include "ToolsGuideLine.h"
#include "uiCombo.h"
#include "uiPauseScene.h"
#include "SimpleAudioEngine.h"
#include "publicDefine.h"
#include "statisticsLog.h"
#include "uiStoreScene.h"
#include "uiResurrectionScene.h"
#include "uiDeblockingScene.h"
#include "uiBuyPropertyScene.h"


using namespace cocos2d;
using namespace gui;
using namespace CocosDenshion;

static const BUBBLE_COLOR ableShootBubbles[ABLE_SHOOT_BUBBLE_COUNT] = { BUBBLE_YELLOW, BUBBLE_PURPLE, BUBBLE_BULE, BUBBLE_RED, BUBBLE_GREEN, BUBBLE_ORANGE, BUBBLE_PINK, BUBBLE_PROPERTY_COLOR, BUBBLE_PROPERTY_BOMB };
static const float PROPERTY_BUBBLE_SCALE_VALUE = 0.8f;
static const float BOARD_DOWN_DURATION = 0.01f;
static const float BOARD_DOWN_DIS = 2 * BUBBLE_RADIUS*sin(PI / 3);

#define UI_PATH   "ui/gameScene.json"
#define UI_RES_ROOT_PATH      "ui/myRes/"

#define UI_NAME "Panel_14"
#define BG_BOTTOM_VIEW_TAG_NAME    "Image_1"
#define BG_ABOVE_VIEW_TAG_NAME    "Image_8"
#define PAUSE_BUTTON_TAG_NAME    "Button_9"
#define PROPERTY_HAMMER_BUTTON_NAME    "Button_2"
#define PROPERTY_BOMB_BUTTON_NAME    "Button_3"
#define PROPERTY_COLOR_BUTTON_NAME    "Button_4"
#define PROPERTY_GUIDE_BUTTON_NAME    "Button_5"
#define PROPERTY_HAMMER_IMAGE_NAME    "Image_12"
#define PROPERTY_BOMB_IMAGE_NAME    "Image_12_0"
#define PROPERTY_COLOR_IMAGE_NAME    "Image_12_1"
#define PROPERTY_GUILD_IMAGE_NAME    "Image_12_2"

#define UI_STORE_SCENE_HAMMER_BUTTON_NAME "Button_37"
#define UI_STORE_SCENE_BOMB_BUTTON_NAME "Button_47"
#define UI_STORE_SCENE_GUIDE_BUTTON_NAME "Button_38"
#define UI_STORE_SCENE_COLOR_BUTTON_NAME "Button_50"

#define STEPS_COUNT_DOWN_BG "ui/myRes/shuzidaojiquan.png"
#define STEPS_COUNT_DOWN_NUM "ui/myRes/shuzidaoji.png"
#define SCORES_COUNT_PNG "ui/myRes/shuzi.png"
#define SCORE_PNG "ui/myRes/shuzijiafen.png"
#define BUBBLE_DIED_LINE_PICTURE_NAME "ui/myRes/jingjiexian.png"
#define BUBBLE_DOWN_BOARD "ui/myRes/xiahuagan.png"
#define PROGRESS_TIMER_BG "ui/myRes/jindutiaodi.png"
#define PROGRESS_TIMER_INIT "ui/myRes/jindutiao.png"
#define PROGRESS_STAR_LIGHT_PNG "ui/myRes/xingxingliang.png"
#define PROGRESS_STAR_DARK_PNG "ui/myRes/xingxingan.png"
#define BUBBLE_BEGIN_DISTANCE 6

#define THREE_STAR_SCORE_PER 400.0f
#define GOLD_ADD_PER 5
#define UI_PAUSE_LAYER_TAG 222

#define WHITE_ARROWS_PNG "ui/myRes/jiantoubai.png"

#define ANIMATION_GOLD_PLIST "ui/animations/coin/jinbi0.plist"

const static int READY_ARROW = 10001;
const static int GUIDE_LINE = 10002;
const static int COUNT_NUM = 10003;
const static float SHOOT_NORMALIZE_MIN_Y = 0.305f;
const static float oneStarTag = 20000;
const static float twoStarTag = 21000;
const static float twoStarTag_one = 21001;
const static float threeStarTag = 22000;
const static float threeStarTag_one = 22001;
const static float threeStarTag_two = 22002;

#define BUBBLE_JUMP_XRANGE 3
#define BUBBLE_JUMP_YRANGE 10
#define BUBBLE_DROPDOWM_DESTANCE 8
#define BUTTON_SCALE_COEFFICIENT 1.3f
#define GUIDE_POINT_DIS 30.0f

GameScene* GameScene::m_inst = 0;

static const char* HAMMER_NUM_LABELATLAS_NAME = "AtlasLabel_22";
static const char* BOMB_NUM_LABELATLAS_NAME = "AtlasLabel_22_0";
static const char* COLOR_NUM_LABELATLAS_NAME = "AtlasLabel_22_1";
static const char* GUILD_NUM_LABELATLAS_NAME = "AtlasLabel_22_2";
static const char* GOLD_NUM_LABELATLAS_NAME = "AtlasLabel_7";
static const char* CUR_STAGE_NUM_LABELATLAS_NAME = "AtlasLabel_11";

#define BUBBLE_TOTLE_PLIST "ui/myRes/paopao0.plist"
#define BUBBLE_TOTLE_IMAGE "ui/myRes/paopao0.png"

GameScene::GameScene() :
m_state(GS_START),
m_nGoldenBubbleCount(0),
m_nSliveryBubbleCount(0),
m_nScore(0),
m_steps(0),
m_showStartRow(0),
m_bubbleCountNum(0),
m_starNum(0),
m_comboNum(-1)
{
	//SimpleAudioEngine::sharedEngine()->playBackgroundMusic("game_scene_bg.mp3",true);
	//SimpleAudioEngine::sharedEngine()->playEffect("sound/android_sound/ready_go.mp3");
	srand(time(NULL));
	isCollisonWithBlackHole = false;
	CCPoint readyPos = READY_BUBBLE_POS;
	m_diedLinePos = ccp(readyPos.x, readyPos.y + 80);
	m_stage = 0;
	m_isHammerSelect = false;
	m_dropDowndistance = 0;
	m_clearAction = 0;
	isUsingGuide = false;
	m_guideLineIsMoving = false;
	score_tag = 20000;
	score_tag_temp = 20000;
	partical_tag = 100;
	partical_tag_temp = 100;
	m_downBubble = NULL;
	m_isClearBubble = false;
	m_clearTime = 0.0f;
	m_clickIslocking = false;
	isUItouched = false;
	m_isGameOver = false;
	m_isHitGoldOrAir = false;
	m_isAddSurScene = false;
	isDisponse = false;
	isCollisionWithDiedBubble = false;
	isChangeWaitToReady = false;
	m_upDistance = BUBBLE_BEGIN_DISTANCE*BOARD_DOWN_DIS;
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(BUBBLE_TOTLE_PLIST, BUBBLE_TOTLE_IMAGE);
}

GameScene::~GameScene()
{
	MyGame::getInstance()->saveUserData();
	m_collisionToBubble = NULL;
	m_bubbleBatchNode = NULL;
	m_inst = NULL;
}

void GameScene::onEnterTransitionDidFinish()
{
	if (!m_inst)
	{
		m_inst = this;
		this->loadMapData(MyGame::getInstance()->getCurStage(), false);
		stageInfo*info = XMLTools::getInstance()->getStageInfoByIndex(MyGame::getInstance()->getCurStage());
		if (info->m_isSpecialStage)
			this->addChild(UIDeblockingScene::create(),2);
		if (CCUserDefault::sharedUserDefault()->getBoolForKey("first_game", true))
		{
			this->addChild(NewPlayerGuide::create(), 2);
			CCUserDefault::sharedUserDefault()->setBoolForKey("first_game", false);
		}
	}
}

CCScene* GameScene::scene(int level, bool isRandom)
{
	//CCScene* scene = NULL;
	//do
	//{
	//	scene = CCScene::create();
	//	CC_BREAK_IF(!scene);
	//	GameScene* layer = GameScene::create();
	//	layer->loadMapData(level, isRandom);
	//	CC_BREAK_IF(!layer);
	//	scene->addChild(layer);
	//} while (0);
	MyGame::getInstance()->setCurStage(level);
	CCScene* scene = CCScene::create();
	GameScene* layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!TouchGroup::init());
		Widget *widget = extension::GUIReader::shareReader()->widgetFromJsonFile(UI_PATH);
		CC_BREAK_IF(!widget);
		widget->setZOrder(1);
		this->addWidget(widget);
		this->screenAdaptive(widget);

		//底层UI
		ImageView *bottomImageView = dynamic_cast<ImageView*>(widget->getChildByName(BG_BOTTOM_VIEW_TAG_NAME));
		CC_BREAK_IF(!bottomImageView);
		//顶层UI
		ImageView *aboveImageView = dynamic_cast<ImageView*>(widget->getChildByName(BG_ABOVE_VIEW_TAG_NAME));
		CC_BREAK_IF(!aboveImageView);
		m_curSatgeAtlas = dynamic_cast<UILabelAtlas*>(aboveImageView->getChildByName(CUR_STAGE_NUM_LABELATLAS_NAME));

		CCSize size = CCDirector::sharedDirector()->getVisibleSize();
		CCSize sizeTemp = aboveImageView->getContentSize();
		m_upLine = size.height - sizeTemp.height;

		Button* pauseButton = dynamic_cast<Button*>(aboveImageView->getChildByName(PAUSE_BUTTON_TAG_NAME));
		CC_BREAK_IF(!pauseButton);
		pauseButton->addTouchEventListener((CCObject*)this, toucheventselector(GameScene::touchedCallBack));

		pHammerButton = dynamic_cast<Button*>(bottomImageView->getChildByName(PROPERTY_HAMMER_BUTTON_NAME));
		CC_BREAK_IF(!pHammerButton);
		pHammerButton->addTouchEventListener((CCObject*)this, toucheventselector(GameScene::touchedCallBack));
		Button* pBombButton = dynamic_cast<Button*>(bottomImageView->getChildByName(PROPERTY_BOMB_BUTTON_NAME));
		CC_BREAK_IF(!pBombButton);

		pBombButton->addTouchEventListener((CCObject*)this, toucheventselector(GameScene::touchedCallBack));
		Button* pColorButton = dynamic_cast<Button*>(bottomImageView->getChildByName(PROPERTY_COLOR_BUTTON_NAME));
		CC_BREAK_IF(!pColorButton);
		pColorButton->addTouchEventListener((CCObject*)this, toucheventselector(GameScene::touchedCallBack));
		Button* pGuideButton = dynamic_cast<Button*>(bottomImageView->getChildByName(PROPERTY_GUIDE_BUTTON_NAME));
		CC_BREAK_IF(!pGuideButton);
		pGuideButton->addTouchEventListener((CCObject*)this, toucheventselector(GameScene::touchedCallBack));


		//m_inst = this;
		setPropertyNumTTF();


		CC_BREAK_IF(!GameScene::initScheduler());//初始化定时器
		//CC_BREAK_IF(!GameScene::initBoard());//初始化泡泡
		//CC_BREAK_IF(!GameScene::initReadyBubble());//初始化准备泡泡(下一个发射的泡泡)
		//CC_BREAK_IF(!GameScene::initWaitBubble());//初始化等待泡泡(要发射的泡泡)

		pDiedLine = CCSprite::create(BUBBLE_DIED_LINE_PICTURE_NAME);
		pDiedLine->setPosition(m_diedLinePos);
		pDiedLine->setVisible(false);
		this->addChild(pDiedLine);

		m_curUsePropertyType = PROPERTY_TYPE_NONE;
		m_guideArrow = NULL;
		m_guideLine = NULL;
		m_collisionToBubble = NULL;

		m_countDown = CCSprite::create(STEPS_COUNT_DOWN_BG);
		m_countDown->setPosition(COUNT_STEPS_POS);

		CCLabelAtlas*num = CCLabelAtlas::create("8", STEPS_COUNT_DOWN_NUM, 22, 28, '0');
		num->setAnchorPoint(ccp(0.5, 0.5));
		num->setPosition(ccp(25, 25));
		m_countDown->addChild(num, 0, COUNT_NUM);
		this->addChild(m_countDown, 0);

		m_scoreAtlas = CCLabelAtlas::create("0", SCORES_COUNT_PNG, 30, 50, '.');
		m_scoreAtlas->setScale(0.8f);
		m_scoreAtlas->setAnchorPoint(ccp(0.5, 0.5));
		m_scoreAtlas->setPosition(ccp(size.width*0.55, size.height*0.975));
		this->addChild(m_scoreAtlas, 1);

		this->createProgressTimer();

		this->setKeypadEnabled(true);

		cc_timeval psv;
		CCTime::gettimeofdayCocos2d(&psv, NULL);
		unsigned long int rand_seed = psv.tv_sec * 1000 + psv.tv_usec / 1000;
		srand(rand_seed);

		bRet = true;

		if (MyGame::getInstance()->isGameMusicAbled())
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_GAMESCENE_BG_MUSIC, true);

	} while (0);

	return bRet;
}

void GameScene::screenAdaptive(cocos2d::gui::UIWidget*pWidget)
{
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	pWidget->setSize(ccp(640, 640 * size.height / size.width));
}

void GameScene::setPropertyNumTTF()
{
	Widget*widget = this->getWidgetByName(UI_NAME);
	ImageView *bottomImageView = dynamic_cast<ImageView*>(widget->getChildByName(BG_BOTTOM_VIEW_TAG_NAME));

	ImageView *hammerImageView = dynamic_cast<ImageView*>(bottomImageView->getChildByName(PROPERTY_HAMMER_IMAGE_NAME));

	//锤子技能数
	LabelAtlas* hammerNumLabelAtlas = dynamic_cast<LabelAtlas*>(hammerImageView->getChildByName(HAMMER_NUM_LABELATLAS_NAME));
	ostringstream hammerNumOss;
	hammerNumOss << MyGame::getInstance()->m_HammerNum;
	hammerNumLabelAtlas->setStringValue(hammerNumOss.str());

	ImageView *bombImageView = dynamic_cast<ImageView*>(bottomImageView->getChildByName(PROPERTY_BOMB_IMAGE_NAME));
	//炸弹技能数
	LabelAtlas* bomdNumLabelAtlas = dynamic_cast<LabelAtlas*>(bombImageView->getChildByName(BOMB_NUM_LABELATLAS_NAME));
	ostringstream bomdNumOss;
	bomdNumOss << MyGame::getInstance()->m_bombNum;
	bomdNumLabelAtlas->setStringValue(bomdNumOss.str());

	ImageView *colorImageView = dynamic_cast<ImageView*>(bottomImageView->getChildByName(PROPERTY_COLOR_IMAGE_NAME));

	//彩球技能数
	LabelAtlas* colorNumLabelAtlas = dynamic_cast<LabelAtlas*>(colorImageView->getChildByName(COLOR_NUM_LABELATLAS_NAME));
	ostringstream colorNumOss;
	colorNumOss << MyGame::getInstance()->m_colorBubbleNum;
	colorNumLabelAtlas->setStringValue(colorNumOss.str());

	ImageView *guildImageView = dynamic_cast<ImageView*>(bottomImageView->getChildByName(PROPERTY_GUILD_IMAGE_NAME));
	//引导线技能数
	LabelAtlas* guildNumLabelAtlas = dynamic_cast<LabelAtlas*>(guildImageView->getChildByName(GUILD_NUM_LABELATLAS_NAME));
	ostringstream guildNumOss;
	guildNumOss << MyGame::getInstance()->m_guidLineNum;
	guildNumLabelAtlas->setStringValue(guildNumOss.str());

	//金币数
	LabelAtlas* goldNumLabelAtlas = dynamic_cast<LabelAtlas*>(bottomImageView->getChildByName(GOLD_NUM_LABELATLAS_NAME));
	ostringstream goldNumOss;
	goldNumOss << MyGame::getInstance()->m_gold;
	goldNumLabelAtlas->setStringValue(goldNumOss.str());
	m_goldPoint = goldNumLabelAtlas->getParent()->convertToWorldSpace(goldNumLabelAtlas->getPosition());
}

bool GameScene::loadMapData(int level, bool isRandom)
{
	bool bRet = false;
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();

	m_stage = level;
	CCString*str = CCString::createWithFormat("%d", m_stage);
	m_curSatgeAtlas->setStringValue(str->getCString());

	MyGame::getInstance()->setCurStage(m_stage);

	if (!isRandom)
	{
		stageMapInfo = StageMapDict::getInstance()->getStageMapInfos(level);
		vector<vector<std::string> > mapData = stageMapInfo->m_data;
		int lines = stageMapInfo->m_lines;

		MyGame::getInstance()->max_rows = lines + BUBBLE_BEGIN_DISTANCE + 1;

		//小球出生位置
		m_boardStartPos = ccp(size.width / 53, m_diedLinePos.y + (lines + BUBBLE_BEGIN_DISTANCE)*BOARD_DOWN_DIS + 8);

		//下落板
		m_downBoard = CCSprite::create(BUBBLE_DOWN_BOARD);
		m_downBoard->setPosition(ccp(size.width / 2, m_boardStartPos.y + 10));

		{
			RowCol tempRC = GetRowColByPos(size.width / 2, m_upLine);
			if (tempRC.m_nRow < 0)
				m_showStartRow = 0;
			else
				m_showStartRow = tempRC.m_nRow;

			RowCol tempRC1 = GetRowColByPos(size.width / 2, m_diedLinePos.y);
			m_checkLine = tempRC1.m_nRow - m_showStartRow+1;
		}
		CCLog("GameScene::LoadMapData--m_showStartRow init num is:%d", m_showStartRow);

		Widget*widget = this->getWidgetByName(UI_NAME);
		widget->addNode(m_downBoard, 2);

		m_bubbleBatchNode = CCSpriteBatchNode::create(BUBBLE_TOTLE_IMAGE);
		widget->addNode(m_bubbleBatchNode, 2);

		for (int i = 0; i < lines; i++)
		{
			vector<std::string>temp = mapData[i];
			vector<Bubble*>bubbleTemp;
			for (int j = 0; j < temp.size(); j++)
			{
				std::vector<int> bubbleDictStr;
				CommonFunc::split(bubbleDictStr, temp[j], ",");
				int color = bubbleDictStr[0];
				bool isIceBubble;
				if (bubbleDictStr[1] == 1)
					isIceBubble = true;
				else
					isIceBubble = false;

				Bubble*pBubble = NULL;

				if (color != -1)
				{
					BUBBLE_COLOR tempColor = (BUBBLE_COLOR)color;
					if (isIceBubble)
						pBubble = this->createIceBubble(tempColor);
					else
						pBubble = this->createBubble(tempColor);
					pBubble->setPosition(getPosByRowAndCol(i, j));

					m_bubbleBatchNode->addChild(pBubble);

					pBubble->setRowColIndex(i, j);
					m_listBubble.push_back(pBubble);
					m_bubbleCountNum++;

					this->addBubbleToColorArr(pBubble);
					this->adjustDownBubble(pBubble);

					//病毒泡泡队列
					if (pBubble->getBubbleColor() == BUBBLE_VIRUS)
						m_virusBubble.push_back(pBubble);
				}
				bubbleTemp.push_back(pBubble);
			}
			m_board.push_back(bubbleTemp);
		}

		for (int i = lines; i < MyGame::getInstance()->max_rows; i++)
		{
			vector<Bubble*>bubbleTemp;
			for (int j = 0; j < MAX_COLS; j++)
			{
				Bubble*pBubble = NULL;
				bubbleTemp.push_back(pBubble);
			}
			m_board.push_back(bubbleTemp);
		}
	}

	GameScene::initReadyBubble();
	GameScene::initWaitBubble();

	if (MyGame::getInstance()->isGameEffectAbled())
		SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_READYGO_EFFECT);

	CCString*strtemp = CCString::createWithFormat("%d", level);
	StatisticsLog::getInstance()->sendLogData("PlayGame", strtemp->getCString());

	return bRet;
}

void GameScene::touchedCallBack(CCObject* pSender, TouchEventType pType){
	switch (pType){
	case TOUCH_EVENT_BEGAN:
		isUItouched = true;
		break;
	case TOUCH_EVENT_MOVED:
		break;
	case TOUCH_EVENT_ENDED:
	{
		Widget *pWidget = (Widget*)(pSender);
		this->disposeTouchEvent(pWidget);
	}
		break;
	case TOUCH_EVENT_CANCELED:
		break;
	default:
		break;
	}
}

void GameScene::disposeTouchEvent(UIWidget*pWidget)
{
	if (MyGame::getInstance()->isGameEffectAbled())
		SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_BUTTON_EFFECT);
	if (0 == strcmp(pWidget->getName(), PAUSE_BUTTON_TAG_NAME))
	{
		CCNode*pNode = this->getChildByTag(UI_PAUSE_LAYER_TAG);
		if (!pNode)
			this->addChild(UIPauseScene::create(), 2, UI_PAUSE_LAYER_TAG);
	}
	else if (0 == strcmp(pWidget->getName(), PROPERTY_HAMMER_BUTTON_NAME))
	{
		if (MyGame::getInstance()->m_HammerNum >= 1)
		{
			if (m_listBubble.empty())
				return;
			if (!m_isHammerSelect)
			{
				m_isHammerSelect = true;
				pWidget->setScale(BUTTON_SCALE_COEFFICIENT);
			}
			else
			{
				m_isHammerSelect = false;
				pWidget->setScale(1.0f);
			}
			StatisticsLog::getInstance()->sendLogData("UseHammer");
			clickedProperty(PROPERTY_TYPE_HAMMER);
		}
		else{
			(MyGame::getInstance()->m_gold < 100) ?
			CCDirector::sharedDirector()->pushScene(UIStoreScene::scene()):
			this->addChild(UIBuyPropertyScene::create("Button_37"));
		}
	}
	else if (0 == strcmp(pWidget->getName(), PROPERTY_BOMB_BUTTON_NAME))
	{
		if (MyGame::getInstance()->m_bombNum >= 1)
		{
			MyGame::getInstance()->addMyProperty(0, 0, -1, 0, 0);
			setPropertyNumTTF();
			StatisticsLog::getInstance()->sendLogData("UseBomb");
			clickedProperty(PROPERTY_TYPE_BOMB);
		}
		else{
			(MyGame::getInstance()->m_gold < 100) ?
			CCDirector::sharedDirector()->pushScene(UIStoreScene::scene()):
			this->addChild(UIBuyPropertyScene::create("Button_47"));
		}
	}
	else if (0 == strcmp(pWidget->getName(), PROPERTY_COLOR_BUTTON_NAME))
	{
		if (MyGame::getInstance()->m_colorBubbleNum >= 1)
		{
			MyGame::getInstance()->addMyProperty(0, 0, 0, -1, 0);
			setPropertyNumTTF();
			StatisticsLog::getInstance()->sendLogData("UseColorBubble");
			clickedProperty(PROPERTY_TYPE_COLOR);
		}
		else{
			(MyGame::getInstance()->m_gold < 100) ?
			CCDirector::sharedDirector()->pushScene(UIStoreScene::scene()):
			this->addChild(UIBuyPropertyScene::create("Button_50"));
		}
	}
	else if (0 == strcmp(pWidget->getName(), PROPERTY_GUIDE_BUTTON_NAME))
	{
		if (MyGame::getInstance()->m_guidLineNum >= 1)
		{
			MyGame::getInstance()->addMyProperty(0, 0, 0, 0, -1);
			setPropertyNumTTF();
			StatisticsLog::getInstance()->sendLogData("UseGuideLine");
			clickedProperty(PROPERTY_TYPE_GUIDE);
			pWidget->setTouchEnabled(false);
			isUsingGuide = true;
			m_guideLineIsMoving = true;
		}
		else{
			(MyGame::getInstance()->m_gold < 80)?
			CCDirector::sharedDirector()->pushScene(UIStoreScene::scene()):
			this->addChild(UIBuyPropertyScene::create("Button_38"));
		}
	}
}

//道具处理
void GameScene::clickedProperty(PROPERTY_TYPE pType)
{
	m_curUsePropertyType = pType;
	if (PROPERTY_TYPE_COLOR == pType)
	{
		int bubbleColor = BUBBLE_PROPERTY_COLOR;
		CCSprite*arrows = (CCSprite*)this->getWidgetByName(UI_NAME)->getNodeByTag(READY_ARROW);
		arrows->setTexture(CCTextureCache::sharedTextureCache()->addImage(WHITE_ARROWS_PNG));
		m_curReady->setBubbleColor(BUBBLE_PROPERTY_COLOR);
		CCSpriteFrame*frameTemp = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(g_bubbleName[bubbleColor].c_str());
		m_curReady->setDisplayFrame(frameTemp);
		//m_curReady->setScale(PROPERTY_BUBBLE_SCALE_VALUE);
		m_curUsePropertyType = PROPERTY_TYPE_NONE;
	}
	else if (PROPERTY_TYPE_BOMB == pType)
	{
		int bubbleColor = BUBBLE_PROPERTY_BOMB;
		CCSprite*arrows = (CCSprite*)this->getWidgetByName(UI_NAME)->getNodeByTag(READY_ARROW);
		arrows->setTexture(CCTextureCache::sharedTextureCache()->addImage(WHITE_ARROWS_PNG));
		m_curReady->setBubbleColor(BUBBLE_PROPERTY_BOMB);
		CCSpriteFrame*frameTemp = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(g_bubbleName[bubbleColor].c_str());
		m_curReady->setDisplayFrame(frameTemp);
		//m_curReady->setScale(PROPERTY_BUBBLE_SCALE_VALUE);
		m_curUsePropertyType = PROPERTY_TYPE_NONE;
	}
	else if (PROPERTY_TYPE_HAMMER == pType)
	{
		m_curUsePropertyType = PROPERTY_TYPE_HAMMER;
		setDisableEnable();
	}
	else if (PROPERTY_TYPE_GUIDE == pType)
	{
		isUsingGuide = true;
		m_guideLineIsMoving = true;
		CCSize size = CCDirector::sharedDirector()->getVisibleSize();
		m_guideLineMy->setBubbleSpriteFrameName(m_curReady->getBubbleColor());

		float angle = m_curReady->getRotation() / 180.0f*PI;
		CCPoint tempPoint = ccp(m_curReady->getPositionX() + 100 * sin(angle), m_curReady->getPositionY() + 100 * cos(angle));
		m_guideLineMy->displayGuideLine(tempPoint, true);
	}
}

bool GameScene::updateGuide(CCPoint touchPos)
{
	float touchToArrowDisX = (touchPos.x - m_curReady->getPositionX());
	float touchToArrowDisY = (touchPos.y - m_curReady->getPositionY());

	float angleValue = PI / 2 - atan2(touchToArrowDisY, touchToArrowDisX);
	float rotation = angleValue * 180 / PI;

	if (rotation > 70.0f || rotation < -70.0f)
	{
		return false;
	}
	m_curReady->setRotation(rotation);//旋转
	this->getWidgetByName(UI_NAME)->getNodeByTag(READY_ARROW)->setRotation(rotation);

	if (!isUsingGuide)
	{
		return true;
	}
	if (NULL == this->getChildByTag(GUIDE_LINE))
	{
		m_guideLine = CCNode::create();
		m_guideLine->setPosition(m_curReady->getPosition());
		m_guideLine->setTag(GUIDE_LINE);
		this->addChild(m_guideLine);
	}
	m_guideLine->removeAllChildren();
	bool isNotLastPoint = true;
	float pointX = m_curReady->getPositionX();
	float pointY = m_curReady->getPositionY();

	float disX = sin(angleValue)*GUIDE_POINT_DIS;
	float disY = cos(angleValue)*GUIDE_POINT_DIS;

	float borderX = 0;
	float borderY = 0;

	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	float realWidth = size.width / 2 - getBoardStartPos().x - BUBBLE_RADIUS;

	CCSpriteBatchNode*bathNode = CCSpriteBatchNode::create((UI_RES_ROOT_PATH + string("point.png")).c_str(), 200);
	bathNode->setPosition(CCPointZero);
	m_guideLine->addChild(bathNode, 0, 1);

	while (isNotLastPoint)
	{
		if (borderX< -realWidth && 0 > disX)
		{
			disX = -disX;
		}
		else if (borderX > realWidth && 0 < disX)
		{
			disX = -disX;
		}
		borderX += disX;
		borderY += disY;

		if (borderY > m_boardStartPos.y - READY_BUBBLE_POS.y)
		{
			isNotLastPoint = false;
			break;
		}
		if (borderX*borderX + borderY*borderY < (m_curReady->getContentSize().height / 2 + this->getWidgetByName(UI_NAME)->getNodeByTag(READY_ARROW)->getContentSize().height)*(m_curReady->getContentSize().height / 2 + this->getWidgetByName(UI_NAME)->getNodeByTag(READY_ARROW)->getContentSize().height))
		{
			continue;
		}

		CCSprite*guidePoint = CCSprite::createWithTexture(bathNode->getTexture());

		//guidePoint->setColor(getGuideColor());

		guidePoint->setPositionX(borderX);
		guidePoint->setPositionY(borderY);
		bathNode->addChild(guidePoint);

		for (BUBBLE_LIST::reverse_iterator iterBubble = m_listBubble.rbegin(); iterBubble != m_listBubble.rend(); ++iterBubble)
		{
			Bubble *pBubble = *iterBubble;
			if (pBubble&& isCollisionWithBubble(pBubble->getPosition(), BUBBLE_RADIUS, bathNode->convertToWorldSpace(guidePoint->getPosition()), BUBBLE_RADIUS))
			{
				isNotLastPoint = false;
				break;
			}
		}
	}
	return true;
}

ccColor3B GameScene::getGuideColor()
{
	BUBBLE_COLOR color = m_curReady->getBubbleColor();
	switch (color)
	{
	case BUBBLE_YELLOW:
		return ccc3(253, 212, 64);
	case BUBBLE_PURPLE:
		return ccc3(143, 75, 255);
	case BUBBLE_BULE:
		return ccc3(0, 166, 255);
	case BUBBLE_RED:
		return ccc3(219, 41, 17);
	case BUBBLE_GREEN:
		return ccc3(62, 219, 46);
	case BUBBLE_ORANGE:
		return ccc3(255, 108, 11);
	case BUBBLE_PINK:
		return ccc3(255, 123, 188);
	default:
		return ccc3(255, 255, 255);
	}
}

bool GameScene::initScheduler()
{
	return true;
}

void GameScene::updateBoardPos()
{
	//m_clickIslocking=false;

	if (IsDistanceOverStandard())
	{
		wholeDropDowm();
	}
	if (m_steps == 0 || m_steps % 8 != 0)
	{
		return;
	}
	for (BUBBLE_LIST::reverse_iterator iterBubble = m_listBubble.rbegin(); iterBubble != m_listBubble.rend(); ++iterBubble)
	{
		Bubble *pBubble = *iterBubble;
		if (!pBubble)
			continue;
		CCMoveBy*moveby = CCMoveBy::create(0.5f, ccp(0, -BOARD_DOWN_DIS));//修改by黄泽昊(/2)
		pBubble->runAction(moveby);
	}
	m_downBoard->runAction(CCMoveBy::create(0.5f, ccp(0, -BOARD_DOWN_DIS)));//修改by黄泽昊(/2)
	m_boardStartPos.y -= BOARD_DOWN_DIS;//修改by黄泽昊(/2)
	m_showStartRow -= 1;
	if (m_showStartRow < 0)
		m_showStartRow = 0;
	CCSequence*seq = CCSequence::create(CCDelayTime::create(0.5f), CCCallFunc::create(this, callfunc_selector(GameScene::delayCallBack)), NULL);
	this->runAction(seq);
}

void GameScene::delayCallBack()
{
	if (!m_downBubble)
		return;
	//if (m_downBubble->getPositionY() - BUBBLE_RADIUS < m_diedLinePos.y)
	//	gameIsOver();
	if (m_downBubble->getPositionY() - BUBBLE_RADIUS < m_diedLinePos.y){
		if (!m_isGameOver)
		{
			m_isGameOver = true;
			this->gameResult();
		}
	}
}

//初始化泡泡队列 初始队列为init_line 4 行,其余均初始化为空
bool GameScene::initBoard()
{
	bool bRet = false;
	do
	{
		CCSize size = CCDirector::sharedDirector()->getVisibleSize();
		m_boardStartPos = ccp(size.width / 53, size.height*0.88f);
		for (int row = 0; row < MAX_ROWS; row++)
		{
			for (int col = 0; col < MAX_COLS - row % 2; col++)
			{
				//初始化前INIT_LINE行， 其他行为NULL
				if (row >= INIT_LINE)
				{
					m_board[row][col] = NULL;
					if (row % 2 != 0 && col == MAX_COLS - row % 2 - 1)
					{
						m_board[row][col + 1] = NULL;
					}
					continue;
				}
				if (row % 2 != 0 && col == MAX_COLS - row % 2 - 1)
				{
					m_board[row][col + 1] = NULL;
				}
				Bubble* pBubble = randomBubble(BUBBLE_TYPE_ALL);
				if (pBubble == NULL)
				{
					CC_BREAK_IF(!pBubble);
				}

				// 
				pBubble->setPosition(getPosByRowAndCol(row, col));
				m_bubbleBatchNode->addChild(pBubble, 0);

				//this->addChild(pBubble);

				m_board[row][col] = pBubble;//将二维数组与泡泡褡裢
				m_board[row][col]->setRowColIndex(row, col);
				m_listBubble.push_back(pBubble);
				this->addBubbleToColorArr(pBubble);

				//病毒泡泡队列
				if (pBubble->getBubbleColor() == BUBBLE_VIRUS)
					m_virusBubble.push_back(pBubble);

				bRet = true;
			}
		}
	} while (0);

	return bRet;
}

//初始化泡泡发射器
bool GameScene::initReadyBubble()
{
	bool bRet = false;
	do
	{
		m_curReady = randomBubble(m_colorCount);
		CC_BREAK_IF(!m_curReady);
		m_curReady->setPosition(READY_BUBBLE_POS);
		m_bubbleBatchNode->addChild(m_curReady, 0);
		int curColor = m_curReady->getBubbleColor();
		std::stringstream ss;
		ss << UI_RES_ROOT_PATH << "arrow_" << curColor << ".png";
		CCSprite * guideArrow = CCSprite::create(ss.str().c_str());

		guideArrow->setPosition(ccp(READY_BUBBLE_POS.x, READY_BUBBLE_POS.y));

		guideArrow->setTag(READY_ARROW);
		guideArrow->setAnchorPoint(ccp(0.5f, -0.1f));
		this->getWidgetByName(UI_NAME)->addNode(guideArrow, 1);

		m_guideLineMy = GuideLine::create(m_curReady, "");
		//this->addChild(m_guideLineMy,2);
		this->getWidgetByName(UI_NAME)->addNode(m_guideLineMy, 2);
		m_guideLineMy->m_pBoard = &m_board;

		//this->addChild(m_curReady);

		bRet = true;
	} while (0);

	return bRet;
}

bool GameScene::initWaitBubble()
{
	bool bRet = false;
	do
	{
		for (int i = 0; i < MAX_WAIT_BUBBLE; i++)
		{
			Bubble *pBubble = randomBubble(m_colorCount);
			CC_BREAK_IF(!pBubble);

			CCSize size = CCDirector::sharedDirector()->getVisibleSize();
			pBubble->setPosition(WAITE_BUBBLE_POS);
			m_wait[i] = pBubble;

			m_bubbleBatchNode->addChild(pBubble, 0);

			pHammerButton->addTouchEventListener((CCObject*)this, toucheventselector(GameScene::touchedCallBack));

			bRet = true;
		}
	} while (0);

	return bRet;
}

Bubble*GameScene::randomBubble(vector<ColorAndNum> colorVectoe)
{
	int size = colorVectoe.size();
	int color = 0;
	if (size == 0)
		color = 1;
	else
		color = rand() % size;
	Bubble*pBubble = NULL;
	if (size)
	{
		BUBBLE_COLOR bubbleColor = colorVectoe[color].m_color;
		pBubble = createBubble(bubbleColor);
	}
	else
	{
		pBubble = createBubble(BUBBLE_BULE);
	}
	return pBubble;
}

//产生一个随机泡泡
Bubble* GameScene::randomBubble(BUBBLE_TYPE bubbleType)
{
	int colorCount = 0;
	BUBBLE_COLOR color;
	if (BUBBLE_TYPE_NORMAL == bubbleType)
	{
		colorCount = NORMAL_BUBBLE_COUNT;
		color = static_cast<BUBBLE_COLOR>(rand() % colorCount);
	}
	else if (BUBBLE_TYPE_ALL == bubbleType)
	{
		colorCount = ALL_BUBBLE_COUNT - PROPERTY_BUBBLE_COUNT;
		color = static_cast<BUBBLE_COLOR>(rand() % colorCount);
	}

	Bubble *pBubble = NULL;
	if (color != BUBBLE_ICE)
		pBubble = createBubble(color);
	else
	{
		colorCount = NORMAL_BUBBLE_COUNT;
		color = static_cast<BUBBLE_COLOR>(rand() % colorCount);
		pBubble = createIceBubble(color);
	}
	return pBubble;
}

//根据颜色创建一个泡泡
Bubble* GameScene::createBubble(BUBBLE_COLOR color)
{
	Bubble *pBubble = NULL;
	do
	{
		pBubble = Bubble::create();
		if (pBubble && pBubble->initWithSpriteFrameName(g_bubbleName[color].c_str()))
		{
			pBubble->setBubbleColor(color);
		}
		else
		{
			CC_SAFE_DELETE(pBubble);
		}
	} while (0);

	return pBubble;
}

Bubble* GameScene::createIceBubble(BUBBLE_COLOR color)
{
	Bubble *pBubble = createBubble(BUBBLE_ICE);
	if (!pBubble){
		CC_SAFE_DELETE(pBubble);
		return NULL;
	}
	else{
	}
	Bubble*pAddBubble = createBubble(color);
	if (!pBubble){
		CC_SAFE_DELETE(pBubble);
	}
	else{
		pAddBubble->setPosition(ccp(BUBBLE_RADIUS, BUBBLE_RADIUS));
	}
	pBubble->addChild(pAddBubble, -1, ICE_BUBBLE_CHILD);
	return pBubble;
}

//清空泡泡数组
void GameScene::clear()
{
	for (int nRow = 0; nRow < MyGame::getInstance()->max_rows; nRow++)
	{
		for (int nCol = 0; nCol < MAX_COLS - nRow % 2; nCol++)
		{
			CC_SAFE_DELETE(m_board[nRow][nCol]);
		}
	}
	m_listBubble.clear();
}

bool GameScene::isCollisionWithBorder(float x)
{
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	float disX = m_boardStartPos.x;
	//if (x < disX+ BUBBLE_RADIUS || x > size.width - BUBBLE_RADIUS-disX)
	if (x < BUBBLE_RADIUS || x > size.width - BUBBLE_RADIUS)
	{
		return true;
	}

	return false;
}

bool GameScene::isCollisionWithTopBorder(Bubble *pBubble)//Y方向是否出界
{
	if (pBubble == NULL)
	{
		return false;
	}

	CCPoint pos = pBubble->getPosition();
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	if (pos.y > m_boardStartPos.y - BUBBLE_RADIUS)
	{
		return true;
	}

	return false;
}

bool GameScene::isCollisionWithBubble(CCPoint pos1, float radius1, CCPoint pos2, float radius2)	//是否和其他的球碰撞
{
	return pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2) < pow(radius1 + radius2, 2);	//判断两圆是否相交, 公式：（x1-x2)^2 + (y1-y2)^2 < (r1 + r2)^2
}

void GameScene::setEnable()
{
	m_clickIslocking = false;
	//if (!m_listBubble.empty())
	//	m_clickIslocking = false;
	//if (isUsingGuide)
	//{
	//	m_curUsePropertyType = PROPERTY_TYPE_NONE;
	//	//m_guideLine->removeAllChildrenWithCleanup(true);
	//	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	//	//updateGuide(ccp(size.width/2,size.height/2));
	//	//this->removeChildByTag(GUIDE_LINE);
	//}
}

void GameScene::setDisableEnable()
{
	m_clickIslocking = true;
}

bool GameScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if (m_clickIslocking){
		if (m_curUsePropertyType == PROPERTY_TYPE_HAMMER)
		{
			destoryBubbleWithHammer(pTouch->getLocation());
		}
		return false;
	}
	TouchGroup::ccTouchBegan(pTouch, pEvent);
	if (isUItouched)
		return true;
	CCNode*pNode = this->getWidgetByName(UI_NAME)->getNodeByTag(READY_ARROW);
	if (pNode)
	{
		float tempX = (pTouch->getLocation().x - m_curReady->getPosition().x);
		float tempY = (pTouch->getLocation().y - m_curReady->getPosition().y);
		float angleValue = PI / 2 - atan2(tempY, tempX);
		float rotation = angleValue * 180 / PI;
		if (rotation<MAX_ANGLE&&rotation>-MAX_ANGLE)
			pNode->setRotation(rotation);
	}

	if (isUsingGuide)
	{
		m_guideLineIsMoving = true;
		m_guideLineMy->rotateGuideLine(pTouch->getLocation());
	}
	return true;
}

void GameScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	if (m_clickIslocking/*||!m_guideLineIsMoving*/)
	{
		return;
	}
	TouchGroup::ccTouchMoved(pTouch, pEvent);

	//updateGuide(pTouch->getLocation());
	CCNode*pNode = this->getWidgetByName(UI_NAME)->getNodeByTag(READY_ARROW);
	if (pNode)
	{
		float tempX = (pTouch->getLocation().x - m_curReady->getPosition().x);
		float tempY = (pTouch->getLocation().y - m_curReady->getPosition().y);
		float angleValue = PI / 2 - atan2(tempY, tempX);
		float rotation = angleValue * 180 / PI;
		if (rotation<MAX_ANGLE&&rotation>-MAX_ANGLE){
			pNode->setRotation(rotation);
			if (!isUsingGuide)
				m_curReady->setRotation(rotation);
		}
	}

	if (isUsingGuide)
	{
		m_guideLineMy->rotateGuideLine(pTouch->getLocation());
	}
}

void GameScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	if (m_clickIslocking)
	{
		//if (m_curUsePropertyType == PROPERTY_TYPE_HAMMER)
		//{
		//	destoryBubbleWithHammer(pTouch->getLocation());
		//}
		return;
	}
	TouchGroup::ccTouchEnded(pTouch, pEvent);
	CCPoint pos = pTouch->getLocation();
	if (isUItouched)
	{
		isUItouched = false;
		return;
	}
	//点击等待区域小球
	if (m_wait[0]->boundingBox().containsPoint(pos) || m_curReady->boundingBox().containsPoint(pos))
	{
		setDisableEnable();
		changeBetweenWaitWithReady();
		return;
	}

	m_real = ccpNormalize(ccpSub(pos, m_curReady->getPosition()));//得到一个方向向量

	float tempAngle = calculateTheAngle(m_curReady->getPosition(), pTouch->getLocation());
	if (tempAngle > MAX_ANGLE || tempAngle < -MAX_ANGLE)
	{
		return;
	}

	this->getWidgetByName(UI_NAME)->removeNodeByTag(READY_ARROW);
	this->setDisableEnable();

	if (isUsingGuide&&m_guideLineIsMoving)
	{
		//m_guideLineMy->setGuideLineVisible(false);
		//m_guideLine->removeAllChildrenWithCleanup(true);
		m_guideLineMy->unDisplayGuideLine();
		m_guideLineIsMoving = false;
	}
	{
		CCParticleSystem* pEmitter = CCParticleSystemQuad::create(g_animationsName[1].c_str());
		pEmitter->setPosition(m_curReady->getPosition());
		//if (this->getWidgetByName(UI_NAME)->getNodeByTag(partical_tag + 1))
		//	CCLog("");
		this->getWidgetByName(UI_NAME)->addNode(pEmitter, 0, partical_tag++);
	}
	this->scheduleUpdate();
	//修改by黄泽昊
	this->addBubbleToColorArr(m_curReady);
	{
		m_steps++;
		this->setStepsNum(m_steps % 8);
	}
	//end
}

float GameScene::calculateTheAngle(CCPoint firstPoint, CCPoint secondPoint)
{
	float dy = secondPoint.y - firstPoint.y;
	float dx = secondPoint.x - firstPoint.x;

	float angleValue = PI / 2 - atan2(dy, dx);
	float rotation = angleValue * 180 / PI;

	return rotation;
}
void GameScene::loop(float dt)
{

}

void GameScene::destoryBubbleWithHammer(CCPoint location)
{
	RowCol rowCol = GetRowColByPos(location.x, location.y);
	if (!IsValidPos(rowCol) || (IsValidPos(rowCol) && m_board[rowCol.m_nRow][rowCol.m_nCol] == NULL))
	{
		setEnable();
		pHammerButton->setScale(1.0f);
		m_isHammerSelect = false;
		return;
	}
	Bubble*pBubble = m_board[rowCol.m_nRow][rowCol.m_nCol];
	//m_showStartRow=rowCol.m_nRow;
	if (pBubble)
	{
		if (MyGame::getInstance()->isGameEffectAbled())
			SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_HAMMER_EFFECT);
		/*修改by黄泽昊*/
		if (pBubble->getBubbleColor() == BUBBLE_LIGHTNING)
		{
			clearBubble(doLightningBubble(pBubble));
		}
		//end
		MyGame::getInstance()->addMyProperty(0, -1, 0, 0, 0);
		setPropertyNumTTF();

		this->playSequenceFrameAction("cuizi", location);
		m_listBubble.remove(pBubble);
		m_virusBubble.remove(pBubble);
		this->removeBubbleFromColorArr(pBubble);

		pBubble->removeFromParentAndCleanup(true);
		m_board[rowCol.m_nRow][rowCol.m_nCol] = NULL;
		setEnable();
		m_isHammerSelect = false;
		m_curUsePropertyType = PROPERTY_TYPE_NONE;
		pHammerButton->setScale(1.0f);
		pHammerButton->setTouchEnabled(true);
		//ROWCOL_LIST fallList = checkFallBubble(m_showStartRow);
		ROWCOL_LIST fallList = checkFallBubble(0);
		{
			float delayTime = FallBubble(fallList);
			CCCallFunc*callFunc = CCCallFunc::create(this, callfunc_selector(GameScene::passGameSuccess));
			this->runAction(CCSequence::create(CCDelayTime::create(delayTime), callFunc, NULL));
		}

	}
}

void GameScene::bubbleCollisonDispose()
{
	//CCLog("child cout : %d",this->getChildrenCount());
	//CCLog("UI child count :%d",this->getWidgetByName(UI_NAME)->getChildrenCount());
	//CCLog("Begin clear bubble m_showStartRow is:%d", m_showStartRow);
	this->unscheduleUpdate();
	isDisponse = true;

	if (MyGame::getInstance()->isGameEffectAbled())
		SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_BUBBLEHIT_EFFECT);

	BUBBLE_LIST aroundList = GetAroundBubble(m_curReady);
	if (!aroundList.empty())
		bubbleCollisionAction(findTheCollisionBubble(*(aroundList.begin())));//碰撞动画
	{
		CCSize size = CCDirector::sharedDirector()->getVisibleSize();
		CCNode*pNode = this->getWidgetByName(UI_NAME)->getNodeByTag(partical_tag - 1);
		if (pNode)
		{
			pNode->setPosition(ccp(size.width*1.5f, size.height*1.5f));
			pNode->runAction(CCSequence::create(CCDelayTime::create(1.0f), CCCallFuncN::create(this, callfuncN_selector(GameScene::removeObjFromParent)), NULL));
		}
	}
	ROWCOL_LIST clearBubbleList = collisionWithBubble(aroundList);
	clearBubble(clearBubbleList);

	//this->FallBubble();
	//this->unscheduleUpdate();

	if (!isCollisonWithBlackHole)
		this->changeWaitToReady();
	m_real = CCPointZero;
	m_collisionToBubble = NULL;

	////如果没有清空且病毒泡泡列表不为空
	if (!m_isClearBubble&&!m_virusBubble.empty())
	{
		virtualInfectionWithBubble();
	}
	if (!m_isClearBubble)
		updateBoardPos();

	this->FallBubble();
	

	//CCLog("child cout : %d",this->getChildrenCount());
	//CCLog("UI child count :%d",this->getWidgetByName(UI_NAME)->getChildrenCount());
}

ROWCOL_LIST GameScene::collisionWithBubble(BUBBLE_LIST bubbleList)
{
	ROWCOL_LIST clearRowCollist;
	int curColor = m_curReady->getBubbleColor();
	switch (curColor)
	{
	case BUBBLE_PROPERTY_COLOR:
		clearRowCollist = doColorBubble(m_curReady);
		break;
	case BUBBLE_PROPERTY_BOMB:
		clearRowCollist = doBombLine(m_curReady);
		break;
	case ALL_BUBBLE_COUNT:
		break;
	default:
		clearRowCollist = collisionWithTheSpecialBubble(bubbleList);
		break;
	}
	clearBarrierIce(clearRowCollist);
	return clearRowCollist;
}

ROWCOL_LIST GameScene::collisionWithTheSpecialBubble(BUBBLE_LIST bubbleList)
{
	ROWCOL_LIST clearRowCollist;
	if (bubbleList.empty())
		return clearRowCollist;
	bool haveBlackHole = false;
	for (BUBBLE_LIST::iterator it = bubbleList.begin(); it != bubbleList.end(); it++)
	{
		Bubble*pBubble = *it;
		if (!pBubble)
			continue;
		if (pBubble->getBubbleColor() == BUBBLE_DIED)
		{
			//m_isGameOver = true;
			//pBubble->setVisible(false);
			//playliziAni(g_animationsName[3].c_str(), pBubble->getPosition().x, pBubble->getPosition().y);
			//this->runAction(CCSequence::create(CCDelayTime::create(1.5f), CCCallFunc::create(this, callfunc_selector(GameScene::collisionWithDiedBubble)), NULL));
			//CCScaleTo*pScale = CCScaleTo::create(1.0f, 2.0f);
			isCollisionWithDiedBubble = true;
			CCBlink*pBlink = CCBlink::create(1.0f, 2);
			pBubble->runAction(CCSequence::create(pBlink,CCCallFuncN::create(this, callfuncN_selector(GameScene::collisionWithDiedBubble)), NULL));
			return clearRowCollist;
		}
	}

	for (BUBBLE_LIST::iterator it = bubbleList.begin(); it != bubbleList.end(); it++)
	{
		Bubble*pBubble = *it;
		if (!pBubble)
			continue;
		int color = pBubble->getBubbleColor();
		switch (color)
		{
		case BUBBLE_BLACK_HOLE://撞到了黑洞
		{
			if (!haveBlackHole)
			{
				dragByBlackHole(pBubble);
			}
		}
			break;
		case BUBBLE_ICE://冰块泡泡破裂
			bubbleIceDestroy(pBubble);
			break;
		case BUBBLE_DIED://撞到了死亡泡泡
			break;
		case BUBBLE_AIR://碰到空气
		case BUBBLE_GOLD://碰到金币
			break;
		case BUBBLE_DYE_BULE:
		case BUBBLE_DYE_GREEN:
		case BUBBLE_DYE_ORANGE:
		case BUBBLE_DYE_RED:
		case BUBBLE_DYE_YELLOW:
		case BUBBLE_DYE_PINK:
		case BUBBLE_DYE_PURPLE:
			/*装到染色小球只染色不消除*/
			doDyeBubble(pBubble);
			clearRowCollist.clear();
			return clearRowCollist;
			//end
		case BUBBLE_LIGHTNING:
			clearRowCollist = doLightningBubble(pBubble);
			break;
		default:
		{
			//ROWCOL_LIST clearRowCollistTemp;
			if ((color == BUBBLE_PROPERTY_COLOR || color == m_curReady->getBubbleColor()) && clearRowCollist.empty())
			{
				/*clearRowCollistTemp = findSameBubble(m_curReady);
				//clearRowCollist = findSameBubble(m_curReady);
				//if (clearRowCollist.size() < REMOVE_COUNT)
				//clearRowCollist.clear();
				/*修改by黄泽昊*/
				/*if (clearRowCollistTemp.size() < REMOVE_COUNT)
				{
					clearRowCollist.clear();
					return clearRowCollist;
				}
				clearRowCollist = findTheVirusBubble(m_curReady);*/
				clearRowCollist = findSameBubble(m_curReady);
				//end

			}
		}
			break;
		}
	}
	return clearRowCollist;
}


void GameScene::collisionWithDiedBubble(CCNode*pNode)
{
	Bubble*pBubble = (Bubble*)pNode;
	m_board[pBubble->getRowIndex()][pBubble->getColumnIndex()] = NULL;
	m_listBubble.remove(pBubble);
	pBubble->removeFromParentAndCleanup(true);
	m_isGameOver = true;
	//this->FallBubble();
	this->runAction(CCSequence::create(CCDelayTime::create(0.5f), CCCallFunc::create(this, callfunc_selector(GameScene::FallBubble)), NULL));
	//this->gameResult();
}

void GameScene::update(float delta)
{
	if (isCollisionWithBorder(m_curReady->getPosition().x))//x方向反弹
		m_real.x = -m_real.x;

	Bubble*pBubble = m_curReady;

	CCPoint pos = m_curReady->getPosition();
	RowCol nowCol = GetRowColByPos(pos.x, pos.y);
	CCPoint posNew = ccp(pos.x + m_real.x * BUBBLE_SPEED, pos.y + m_real.y * BUBBLE_SPEED);
	RowCol rowcol_index = GetRowColByPos(posNew.x, posNew.y);

	if (posNew.y > CCDirector::sharedDirector()->getVisibleSize().height - 2.5*BUBBLE_RADIUS)
	{
		removeCurBubble();
		/*修改by黄泽昊*/
		updateBoardPos();
		//end
		return;
	}

	if (IsValidPos(rowcol_index))
	{
		if (m_board[rowcol_index.m_nRow][rowcol_index.m_nCol] == NULL)
		{
			if (judgeCollision(rowcol_index.m_nRow, rowcol_index.m_nCol))
				adjustBubblePosition(m_collisionToBubble);
			else
				m_curReady->setPosition(posNew);
		}
		else
		{
			if (judgeCollision(rowcol_index.m_nRow, rowcol_index.m_nCol))
				adjustBubblePosition(m_collisionToBubble);
			else
				adjustBubblePosition(m_board[rowcol_index.m_nRow][rowcol_index.m_nCol]);
		}
	}
	else if (posNew.y > m_boardStartPos.y - BUBBLE_RADIUS)
	{
		m_curReady->setPosition(ccp(posNew.x, m_boardStartPos.y - BUBBLE_RADIUS));
		this->adjustBubblePos();
	}
	else
	{
		m_curReady->setPosition(posNew);
	}

	{
		CCNode*pNode = this->getWidgetByName(UI_NAME)->getNodeByTag(partical_tag - 1);
		if (pNode&&pNode->getPositionX() < CCDirector::sharedDirector()->getVisibleSize().width)
			pNode->setPosition(pBubble->getPosition());
	}

}

bool GameScene::judgeCollision(int nRow, int nCol)
{
	std::vector<RowCol> vecRowCol;
	GetAround(nRow, nCol, vecRowCol);
	for (size_t i = 0; i < vecRowCol.size(); i++)
	{
		Bubble* pCurBubble = m_board[vecRowCol[i].m_nRow][vecRowCol[i].m_nCol];
		//if(pCurBubble&&pCurBubble!=m_curReady&&pCurBubble->getBoundingBox().intersectsRect(m_curReady->getBoundingBox()))
		if (pCurBubble&&pCurBubble != m_curReady&&isCollisionWithBubble(pCurBubble->getPosition(), BUBBLE_COLLISION_RADIUS, m_curReady->getPosition(), BUBBLE_COLLISION_RADIUS))
		{
			m_collisionToBubble = pCurBubble;
			return true;
		}
	}
	return false;
}

void GameScene::bubbleIceDestroy(Bubble*pBubble)
{
	if (pBubble->getBubbleColor() != BUBBLE_ICE)
		return;
	Bubble*pChildBubble = (Bubble*)pBubble->getChildByTag(ICE_BUBBLE_CHILD);
	pBubble->setBubbleColor(pChildBubble->getBubbleColor());
	CCSpriteFrame*frameTemp = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(g_bubbleName[pChildBubble->getBubbleColor()].c_str());
	pBubble->setDisplayFrame(frameTemp);
	pBubble->removeAllChildrenWithCleanup(true);
	this->addBubbleToColorArr(pBubble);
}

void GameScene::dragByBlackHole(Bubble*pBlackHoleBubble)
{
	isCollisonWithBlackHole = true;
	//this->unscheduleUpdate();
	m_board[m_curReady->getRowIndex()][m_curReady->getColumnIndex()] = NULL;
	m_listBubble.remove(m_curReady);
	CCScaleTo*scaleTo = CCScaleTo::create(0.3f, 0, 0);
	CCMoveTo*moveTo = CCMoveTo::create(0.3f, pBlackHoleBubble->getPosition());
	CCSpawn*spawn = CCSpawn::createWithTwoActions(scaleTo, moveTo);
	CCSequence*seq = CCSequence::create(spawn, CCCallFunc::create(this, callfunc_selector(GameScene::removeCurBubble)), NULL);
	m_curReady->runAction(seq);
}

void GameScene::removeCurBubble()
{
	this->unscheduleUpdate();
	this->removeBubbleFromColorArr(m_curReady);
	if (m_downBubble==m_curReady)
	{
		m_downBubble = NULL;
	}
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	CCNode*pNode = this->getWidgetByName(UI_NAME)->getNodeByTag(partical_tag - 1);
	if (pNode)
	{
		pNode->setPosition(ccp(size.width*1.5f, size.height*1.5f));
		pNode->runAction(CCSequence::create(CCDelayTime::create(1.0f), CCCallFuncN::create(this, callfuncN_selector(GameScene::removeObjFromParent)), NULL));
	}

	m_curReady->removeFromParentAndCleanup(true);
	changeWaitToReady();
	isCollisonWithBlackHole = false;
	this->FallBubble();
	//this->setEnable();
}

void GameScene::removeBubbleWithNoAction(Bubble*pBubble)
{
	if (m_downBubble == pBubble)
		m_downBubble = NULL;
	m_board[pBubble->getRowIndex()][pBubble->getColumnIndex()] = NULL;
	m_listBubble.remove(pBubble);
	m_virusBubble.remove(pBubble);
}

//调整小球位置
void GameScene::adjustBubblePosition()
{
	CCPoint curPos = m_curReady->getPosition();
	RowCol rowcol_index = GetRowColByPos(curPos.x, curPos.y);
	if (1 == rowcol_index.m_nRow % 2 && (MAX_COLS - 1) == rowcol_index.m_nCol)
	{
		rowcol_index.m_nCol -= 1;
	}
	if (m_board[rowcol_index.m_nRow][rowcol_index.m_nCol])
		rowcol_index.m_nRow += 1;

	CCPoint adjustPos = getPosByRowAndCol(rowcol_index.m_nRow, rowcol_index.m_nCol);
	m_curReady->setPosition(adjustPos);
	m_curReady->setRowColIndex(rowcol_index.m_nRow, rowcol_index.m_nCol);

	m_board[m_curReady->getRowIndex()][m_curReady->getColumnIndex()] = m_curReady;
	m_listBubble.push_back(m_curReady);
	this->addBubbleToColorArr(m_curReady);

	this->adjustDownBubble(m_curReady);

	if (m_curReady->getPositionY() < m_diedLinePos.y)
	{
		//this->gameIsOver();
		m_isGameOver = true;
	}
}

void GameScene::adjustBubblePos()
{
	CCPoint curPos = m_curReady->getPosition();
	RowCol pRowCol = GetRowColByPos(curPos.x, curPos.y);
	while (!IsValidPos(pRowCol))
	{
		if (pRowCol.m_nCol < 0)
			pRowCol.m_nCol = 0;
		if (pRowCol.m_nCol >= MAX_COLS - 1)
			pRowCol.m_nCol -= 1;
	}
	if (!m_board[pRowCol.m_nRow][pRowCol.m_nCol])
	{
		m_curReady->setPosition(getPosByRowAndCol(pRowCol.m_nRow, pRowCol.m_nCol));
	}
	else
	{
		Bubble*pBubble = m_board[pRowCol.m_nRow][pRowCol.m_nCol];
		if (pBubble->getPositionX() > m_curReady->getPositionX())
			m_curReady->setPosition(getPosByRowAndCol(pRowCol.m_nRow, --pRowCol.m_nCol));
		else if (pBubble->getPositionX() <= m_curReady->getPositionX())
			m_curReady->setPosition(getPosByRowAndCol(pRowCol.m_nRow, ++pRowCol.m_nCol));
	}
	m_curReady->setRowColIndex(pRowCol.m_nRow, pRowCol.m_nCol);
	m_board[pRowCol.m_nRow][pRowCol.m_nCol] = m_curReady;
	m_listBubble.push_back(m_curReady);
	this->addBubbleToColorArr(m_curReady);

	this->bubbleCollisonDispose();
}

void GameScene::adjustBubblePosition(Bubble*pBubble)
{
	CCPoint curPoint = m_curReady->getPosition();
	CCPoint collisionPoint = pBubble->getPosition();

	float angleValue = PI / 2 - atan2(curPoint.y - collisionPoint.y, curPoint.x - collisionPoint.x);
	float rotation = angleValue * 180 / PI;
	if (rotation < 0)
		rotation = 360 + rotation;
	RowCol temp = calculateByPosition(pBubble->getBubbleRowCol(), rotation);

	if (pBubble->getBubbleColor() != BUBBLE_AIR&&pBubble->getBubbleColor() != BUBBLE_GOLD)
	{
		m_curReady->setPosition(getPosByRowAndCol(temp.m_nRow, temp.m_nCol));
		m_curReady->setRowColIndex(temp.m_nRow, temp.m_nCol);

		m_board[temp.m_nRow][temp.m_nCol] = m_curReady;
		m_listBubble.push_back(m_curReady);

		//this->addBubbleToColorArr(m_curReady);
		//修改by黄泽昊

		this->adjustDownBubble(m_curReady);

		if (m_curReady->getBoundingBox().getMinY() < m_diedLinePos.y)
		{
			//this->gameIsOver();
			m_isGameOver = true;
		}
		this->bubbleCollisonDispose();
	}
	else
	{
		CCPoint posNew = ccp(curPoint.x + m_real.x * BUBBLE_SPEED, curPoint.y + m_real.y * BUBBLE_SPEED);
		m_curReady->setPosition(posNew);
		//this->removeBubbleWithNoAction(pBubble);
		m_board[pBubble->getRowIndex()][pBubble->getColumnIndex()] = NULL;
		m_listBubble.remove(pBubble);

		this->dealWithAirAndGoldBubble(pBubble);
	}
}

void GameScene::dealWithAirAndGoldBubble(Bubble*pBubble)
{
	float pDelay = 0.0f;
	if (pBubble->getBubbleColor() == BUBBLE_GOLD)
	{
		if (MyGame::getInstance()->isGameEffectAbled())
			SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_GOLD_EFFECT);
		Bubble*pTemp = Bubble::create();
		pTemp->setBubbleColor(BUBBLE_GOLD);
		pTemp->setPosition(pBubble->getPosition());
		this->addChild(pTemp);

		CCAnimation*animation = AnimationTools::getInstance()->createAnimationWithPlist(ANIMATION_GOLD_PLIST, "coin%d.png", 1, 6, 0.5f);
		CCAnimate*animate = CCAnimate::create(animation);
		CCMoveTo*moveTo = CCMoveTo::create(1.0f, m_goldPoint);
		CCSpawn*spawn = CCSpawn::create(CCRepeat::create(animate, 2), moveTo, NULL);
		CCSequence*seq = CCSequence::create(spawn, CCCallFuncN::create(this, callfuncN_selector(GameScene::callbackRemoveBubble)), NULL);
		pTemp->runAction(seq);
		pDelay = 1.0f + 0.5f;
	}
	//m_showStartRow=pBubble->getRowIndex();
	this->removeBubbleWithNoAction(pBubble);
	this->removeBubbleAction(pBubble);
	m_isHitGoldOrAir = true;
	//CCSequence*seq = CCSequence::create(CCDelayTime::create(pDelay), CCCallFunc::create(this, callfunc_selector(GameScene::FallBubble)), NULL);
	//this->runAction(seq);
}

void GameScene::FallBubble()
{
	CCLog("GameScene::FallBubble--End clear bubble m_showStartRow is:%d", m_showStartRow);

	//FallBubble(fallList);
	float delayTime = 0.0f;
	if (m_isClearBubble||m_isHitGoldOrAir||isCollisionWithDiedBubble)
	{
		//ROWCOL_LIST fallList = checkFallBubble(m_showStartRow);
		ROWCOL_LIST fallList = checkFallBubble(0);//修改by黄泽昊
		delayTime = FallBubble(fallList);
	}
	delayTime += m_clearTime;
	CCLog("GameScene::FallBubble--delayTime is:%f", delayTime);
	CCLog("GameScene::FallBubble--CurrentTime--%ld", getCurrentTime());
	if (!m_listBubble.empty()&&abs(delayTime-0.0f)>0.001f)
		delayTime = 0.0f;
	CCCallFunc*callFunc = CCCallFunc::create(this, callfunc_selector(GameScene::gameResult));
	CCLog("GameScene::FallBubble--the num of runningAction--%d", this->numberOfRunningActions());
	this->runAction(CCSequence::create(CCDelayTime::create(delayTime), callFunc, NULL));
}

long long GameScene::getCurrentTime()
{
	struct cc_timeval tv;
	CCTime::gettimeofdayCocos2d(&tv, NULL);
	long long  time = ((long long)tv.tv_sec) * 1000 + tv.tv_usec / 1000;
	return time;
}

void GameScene::GetTime()
{
	struct tm *tm;
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct cc_timeval now;
	CCTime::gettimeofdayCocos2d(&now, NULL);
	timep = now.tv_sec;
#endif
	tm = localtime(&timep);
	int year = tm->tm_year + 1900;
	int month = tm->tm_mon + 1;
	int day = tm->tm_mday;
	int hour = tm->tm_hour;
	int min = tm->tm_min;
	int second = tm->tm_sec;
}

void GameScene::gameResult()
{
	CCLog("GameScene::CurrentTime--%ld", getCurrentTime());
	if (m_listBubble.empty())
		this->passGameSuccess();
	else if (m_isGameOver&&!m_isAddSurScene){
		m_isAddSurScene = true;
		//this->addChild(UIResurrectionScene::create(0),2);// 打开复活礼包
		//CCDirector::sharedDirector()->pushScene(UIStoreScene::scene());// 打开商城
		this->gameIsOver();
	}
	else if(!isChangeWaitToReady&&!isCollisionWithDiedBubble){
		this->setEnable();
	}
	m_isClearBubble = false;
	m_isHitGoldOrAir = false;
	isDisponse = false;
	/*修改by黄泽昊*/
	//checkFallBubble(0);//从整个地图开始检索最底下一个球
	//updateBoardPos();//更新小球位置
	//end
}

void GameScene::gameResurrection()
{
	m_isGameOver = false;
	m_isAddSurScene = false;
	isCollisionWithDiedBubble = false;
	this->setEnable();

	float dis = m_diedLinePos.y + BUBBLE_BEGIN_DISTANCE*BOARD_DOWN_DIS - m_downBubble->getPositionY();
	if (dis <= 0)
		return;
	m_upDistance = dis;

	this->schedule(schedule_selector(GameScene::goToReadyPos));
	int size = m_board.size();
	CCPoint pos = getPosByRowAndCol(size, 0);
	//if (pos.y + BUBBLE_BEGIN_DISTANCE*BOARD_DOWN_DIS > m_diedLinePos.y)
	//{
	//	MyGame::getInstance()->max_rows += BUBBLE_BEGIN_DISTANCE;
	//	CCLog("GameScene::gameResurrection--m_board add to :%d", MyGame::getInstance()->max_rows);
	//	m_showStartRow += BUBBLE_BEGIN_DISTANCE-1;
	//	for (int i = 0; i < BUBBLE_BEGIN_DISTANCE; i++)
	//	{
	//		vector<Bubble*>bubbleTemp;
	//		for (int j = 0; j < MAX_COLS; j++)
	//		{
	//			Bubble*pBubble = NULL;
	//			bubbleTemp.push_back(pBubble);
	//		}
	//		m_board.push_back(bubbleTemp);
	//	}
	//}

	int addNum = dis / BOARD_DOWN_DIS + 0.5;
	if (pos.y + dis > m_diedLinePos.y)
	{
		MyGame::getInstance()->max_rows += addNum;
		CCLog("GameScene::gameResurrection--m_board add to :%d", MyGame::getInstance()->max_rows);
		m_showStartRow += addNum - 1;
		for (int i = 0; i < addNum; i++)
		{
			vector<Bubble*>bubbleTemp;
			for (int j = 0; j < MAX_COLS; j++)
			{
				Bubble*pBubble = NULL;
				bubbleTemp.push_back(pBubble);
			}
			m_board.push_back(bubbleTemp);
		}
	}
}

void GameScene::goToReadyPos(float dt)
{
	m_boardStartPos.y += 2;
	BUBBLE_LIST::iterator ite = m_listBubble.begin();
	while (ite != m_listBubble.end())
	{
		Bubble*pBubble = *ite;
		pBubble->setPositionY(pBubble->getPositionY() + 2);
		ite++;
	}
	m_downBoard->setPositionY(m_downBoard->getPositionY() + 2);
	m_upDistance -= 2;
	if (m_upDistance <= 0)
	{
		this->unschedule(schedule_selector(GameScene::goToReadyPos));
		m_upDistance = BUBBLE_BEGIN_DISTANCE*BOARD_DOWN_DIS;
	}
}

void GameScene::adjustDownBubble(Bubble*pBubble)
{
	//if (m_downBubble)
	//	;
	if (!pBubble)
		return;
	if (m_downBubble&&m_downBubble->getRowIndex() > 0)
	{
		if (pBubble->getPositionY() < m_downBubble->getPositionY())
			m_downBubble = pBubble;
	}
	else
		m_downBubble = pBubble;

	if (m_downBubble->getPositionY() - m_diedLinePos.y <= 3 * BOARD_DOWN_DIS)
	{
		if (!pDiedLine->isVisible())
		{
			pDiedLine->setVisible(true);
			CCSequence*seq = CCSequence::create(CCScaleTo::create(0.7f, 1.0f, 0.3f), CCScaleTo::create(0.7f, 1.0f, 1.0f), NULL);
			CCSpawn*spa = CCSpawn::create(CCBlink::create(1.5f, 2), seq, NULL);
			pDiedLine->runAction(CCRepeatForever::create(seq));
		}
	}
	else
	{
		if (pDiedLine->isVisible())
		{
			pDiedLine->stopAllActions();
			pDiedLine->setVisible(false);
		}
	}
}

RowCol GameScene::calculatePosition(RowCol mindle, float angle)
{
	RowCol temp;
	if (angle >= 0 && angle < 60)
	{
		temp.m_nRow = mindle.m_nRow - 1;
		if (mindle.m_nRow % 2 == 0)
			temp.m_nCol = mindle.m_nCol;
		else
			temp.m_nCol = mindle.m_nCol + 1;
	}
	else if (angle >= 60 && angle < 120)
	{
		temp.m_nRow = mindle.m_nRow;
		temp.m_nCol = mindle.m_nCol + 1;
	}
	else if (angle >= 120 && angle < 180)
	{
		temp.m_nRow = mindle.m_nRow + 1;
		if (mindle.m_nRow % 2 == 0)
			temp.m_nCol = mindle.m_nCol;
		else
			temp.m_nCol = mindle.m_nCol + 1;
	}
	else if (angle >= 180 && angle < 240)
	{
		temp.m_nRow = mindle.m_nRow + 1;
		if (mindle.m_nRow % 2 == 0)
			temp.m_nCol = mindle.m_nCol - 1;
		else
			temp.m_nCol = mindle.m_nCol;
	}
	else if (angle >= 240 && angle < 300)
	{
		temp.m_nRow = mindle.m_nRow;
		temp.m_nCol = mindle.m_nCol - 1;
	}
	else if (angle >= 300 && angle<360)
	{
		temp.m_nRow = mindle.m_nRow - 1;
		if (mindle.m_nRow % 2 == 0)
			temp.m_nCol = mindle.m_nCol - 1;
		else
			temp.m_nCol = mindle.m_nCol;
	}
	if (!isAnAvailablePos(temp))
	{
		if (angle <= 60.0f&&angle>0.0f)
		{
			temp = calculatePosition(mindle, 90);
			//assert(isAnAvailablePos(temp));
		}
		else if (angle >= 60.0f&&angle <= 120.0f)
		{
			temp = calculatePosition(mindle, 150);
			//assert(isAnAvailablePos(temp));
		}
		else if (angle >= 120.0f&&angle <= 180.0f)
		{
			temp.m_nCol -= 1;
			//assert(isAnAvailablePos(temp));
		}
		else if (angle >= 180.0f&&angle <= 240.0f)
		{
			temp.m_nCol += 1;
			//assert(isAnAvailablePos(temp));
		}
		else if (angle >= 240.0f&&angle <= 300.0f)
		{
			temp.m_nRow += 1;
			temp.m_nCol += 1;
			//assert(isAnAvailablePos(temp));
		}
		else if (angle <= 360.0f&&angle > 300.0f)
		{
			temp = calculatePosition(mindle, 270);
			//assert(isAnAvailablePos(temp));
		}
		else
		{
			CCLog("GameScene::calculatePosition--the angle is:%f", angle, "the row is :%f", temp.m_nRow, "the col is :%f", temp.m_nCol);
			assert(isAnAvailablePos(temp));
		}
	}
	return temp;
}

RowCol GameScene::calculateByPosition(RowCol mindle, float angle)
{
	RowCol temp = calculatePosition(mindle, angle);
	if (m_board[temp.m_nRow][temp.m_nCol])
	{
		temp = calculatePositionByTwo(mindle, temp);
	}
	return temp;
}


RowCol GameScene::calculatePositionByTwo(RowCol A, RowCol B)
{
	CCPoint pointA = getPosByRowAndCol(A.m_nRow, A.m_nCol);
	CCPoint pointB = getPosByRowAndCol(B.m_nRow, B.m_nCol);
	CCPoint pointC = m_curReady->getPosition();

	RowCol temp;

	float ABPI = PI / 2 - atan2(pointB.y - pointA.y, pointB.x - pointA.x);
	float ABangle = ABPI * 180 / PI;
	if (ABangle < 0)
		ABangle = 360 + ABangle;

	float ACPI = PI / 2 - atan2(pointC.y - pointA.y, pointC.x - pointA.x);
	float ACangle = ACPI * 180 / PI;
	if (ACangle < 0)
		ACangle = 360 + ACangle;

	float BCPI = PI / 2 - atan2(pointC.y - pointB.y, pointC.x - pointB.x);
	float BCangle = BCPI * 180 / PI;
	if (BCangle < 0)
		BCangle = 360 + BCangle;
	BCangle += 60;

	if (ACangle >= ABangle)
		ACangle += 60;
	else
		ACangle -= 60;

	if (ACangle>360)
		ACangle -= 360;

	temp = this->calculatePosition(A, ACangle);
	if (!isAnAvailablePos(temp))
		temp = this->calculateByPosition(B, BCangle);
	return temp;
}


bool GameScene::isAnAvailablePos(RowCol temp)
{
	if (!IsValidPos(temp.m_nRow, temp.m_nCol))
		return false;
	if (m_board[temp.m_nRow][temp.m_nCol])
		return false;
	return true;
}

//将wait状态的球换成ready状态
void GameScene::changeWaitToReady()
{
	//if (isCollisonWithBlackHole)
	//	isCollisonWithBlackHole=false;
	//else

	/*修改by黄泽昊
	{
		m_steps++;
		this->setStepsNum(m_steps % 8);
	}
	*/
	this->setDisableEnable();
	isChangeWaitToReady = true;

	m_curReady = m_wait[0];
	float scaleNum = m_curReady->getScale();

	CCSequence*pSeq = CCSequence::create(CCJumpTo::create(0.2f, READY_BUBBLE_POS, 50, 1), CCCallFunc::create(this, callfunc_selector(GameScene::setGuideLineMovingTrue)), NULL);
	m_curReady->runAction(pSeq);


	m_curReady->setRotation(m_guideLineMy->getInitialAngle());

	{
		m_guideLineMy->setTargetNode(m_curReady);
		m_guideLineMy->setBubbleSpriteFrameName(m_curReady->getBubbleColor());
		m_guideLineMy->changeBathNodeColor();
	}

	int curColor = m_curReady->getBubbleColor();
	std::stringstream ss;
	if (m_curReady->getBubbleColor() != BUBBLE_PROPERTY_COLOR&&m_curReady->getBubbleColor() != BUBBLE_PROPERTY_BOMB)
	{
		ss << UI_RES_ROOT_PATH << "arrow_" << curColor << ".png";
	}
	else
	{
		ss << WHITE_ARROWS_PNG;
	}
	CCSprite * guideArrow = CCSprite::create(ss.str().c_str());
	guideArrow->setPosition(ccp(READY_BUBBLE_POS.x, READY_BUBBLE_POS.y));
	guideArrow->setTag(READY_ARROW);
	guideArrow->setAnchorPoint(ccp(0.5f, -0.1f));
	guideArrow->setScale(0.0f);
	guideArrow->setRotation(m_curReady->getRotation());

	CCSequence*seqTemp = CCSequence::create(CCDelayTime::create(0.2f), CCScaleTo::create(0.1f, 1.0f), NULL);
	guideArrow->runAction(seqTemp);

	this->getWidgetByName(UI_NAME)->addNode(guideArrow, 1);

	CCSize size = CCDirector::sharedDirector()->getVisibleSize();

	for (int index = 0; index < MAX_WAIT_BUBBLE - 1; index++)
	{
		m_wait[index] = m_wait[index + 1];
		m_wait[index]->setPosition(WAITE_BUBBLE_POS);
		m_wait[index]->setScale(0.0f);
		m_wait[index]->runAction(CCScaleTo::create(0.2f, 1.0f));
	}

	m_wait[MAX_WAIT_BUBBLE - 1] = randomBubble(m_colorCount);
	m_wait[MAX_WAIT_BUBBLE - 1]->setPosition(WAITE_BUBBLE_POS);
	m_wait[MAX_WAIT_BUBBLE - 1]->setScale(0.0f);
	m_wait[MAX_WAIT_BUBBLE - 1]->runAction(CCScaleTo::create(0.4f, 1.0f));

	//Widget*widget=this->getWidgetByName(UI_NAME);
	//widget->addNode(m_wait[MAX_WAIT_BUBBLE - 1],2);
	m_bubbleBatchNode->addChild(m_wait[MAX_WAIT_BUBBLE - 1]);

	//if (!m_isClearBubble)
	//{
	//	CCSequence*seq = CCSequence::create(CCDelayTime::create(0.2f), CCCallFunc::create(this, callfunc_selector(GameScene::setEnable)), NULL);
	//	this->runAction(seq);
	//}
}

void GameScene::setGuideLineMovingTrue()
{
	//this->setEnable();
	if (!isDisponse&&!isCollisionWithDiedBubble){
		isChangeWaitToReady = false;
		this->setEnable();
	}
	else
		CCLog("");
	if (isUsingGuide){
		m_guideLineMy->setGuideLineVisible(true);
		float angle = m_curReady->getRotation() / 180.0f*PI;
		CCPoint tempPoint = ccp(m_curReady->getPositionX() + 100 * sin(angle), m_curReady->getPositionY() + 100 * cos(angle));
		m_guideLineMy->displayGuideLine(tempPoint, true);
	}
}
void GameScene::addBubbleToColorArr(Bubble*pBubble)
{
	if (!pBubble)
		return;
	BUBBLE_COLOR color = pBubble->getBubbleColor();
	if (color >= NORMAL_BUBBLE_COUNT)
		return;
	ColorAndNum tempCAN(color);

	if (m_colorCount.empty())
		m_colorCount.push_back(tempCAN);
	else
	{
		int temp = m_colorCount.size();
		bool isHave = false;
		for (int i = 0; i < temp; i++)
		{
			if (color == m_colorCount[i].m_color)
			{
				m_colorCount[i].m_num++;
				isHave = true;
				return;
			}
		}
		if (!isHave)
			m_colorCount.push_back(tempCAN);
	}

}

void GameScene::removeBubbleFromColorArr(Bubble*pBubble)
{
	if (!pBubble)
		return;
	if (m_colorCount.empty())
		return;
	BUBBLE_COLOR color = pBubble->getBubbleColor();
	vector<ColorAndNum>::iterator ite = m_colorCount.begin();
	for (ite; ite != m_colorCount.end(); ite++)
	{
		ColorAndNum &temp = *ite;
		if (color == temp.m_color)
		{
			temp.m_num--;
		}
		if (temp.m_num <= 0)
		{
			m_colorCount.erase(ite);
			return;
		}
	}
}

void GameScene::setStepsNum(int num)
{
	int backNum = 8 - num;
	CCLabelAtlas*atlas = (CCLabelAtlas*)m_countDown->getChildByTag(COUNT_NUM);
	char str[10] = { 0 };
	sprintf(str, "%d", backNum);
	atlas->setString(str);
}

void GameScene::setScoreCountNum()
{
	char str[10] = { 0 };
	sprintf(str, "%d", m_nScore);
	m_scoreAtlas->setString(str);
}


//将wait状态的球换成ready状态
void GameScene::changeBetweenWaitWithReady()
{
	this->getWidgetByName(UI_NAME)->removeNodeByTag(READY_ARROW);

	m_curReady->runAction(
		CCSequence::create(
		CCJumpTo::create(0.2f, m_wait[0]->getPosition(), 50, 1),
		CCCallFuncN::create(this, callfuncN_selector(GameScene::ReadyChangeWaitEnd)),
		NULL
		)
		);

	m_wait[0]->runAction(
		CCSequence::create(
		CCJumpTo::create(0.2f, m_curReady->getPosition(), 50, 1),
		CCCallFuncN::create(this, callfuncN_selector(GameScene::WaitChangeReadyEnd)),
		NULL
		)
		);
}


void GameScene::clearBarrierIce(ROWCOL_LIST listRowCol)
{
	if (listRowCol.empty())
	{
		return;
	}

	ROWCOL_LIST barrierIcelist;

	ROWCOL_LIST::iterator itCur = listRowCol.begin();

	//循环查找冰块
	do
	{
		std::vector<RowCol> vecRowCol;

		GetAround(itCur->m_nRow, itCur->m_nCol, vecRowCol);

		for (size_t i = 0; i < vecRowCol.size(); i++)
		{
			Bubble* pCurBubble = m_board[vecRowCol[i].m_nRow][vecRowCol[i].m_nCol];
			if (pCurBubble && pCurBubble->getBubbleColor() == BUBBLE_BARRIER_ICE)
			{
				RowCol rc(vecRowCol[i].m_nRow, vecRowCol[i].m_nCol);
				ROWCOL_LIST::iterator itFind = std::find(barrierIcelist.begin(), barrierIcelist.end(), rc);
				//查找是否已经在列表中
				if (itFind == barrierIcelist.end())
				{
					barrierIcelist.push_back(vecRowCol[i]);
				}
			}
		}
		itCur++;
	} while (itCur != listRowCol.end());

	//clearBubble(barrierIcelist);
}

ROWCOL_LIST GameScene::findSameBubble(Bubble *pReadyBubble)
{
	ROWCOL_LIST samelist;
	BUBBLE_COLOR nColor = pReadyBubble->getBubbleColor();
	int nRow = pReadyBubble->getRowIndex();
	int nCol = pReadyBubble->getColumnIndex();
	samelist.push_back(RowCol(nRow, nCol));

	ROWCOL_LIST::iterator itCur = samelist.begin();

	//循环查找同样的球
	int count = 0;
	do
	{
		std::vector<RowCol> vecRowCol;
		Bubble*pTempBubble = m_board[itCur->m_nRow][itCur->m_nCol];//发射的小球碰到的那个球
		if (pTempBubble && this->isObstacle(pTempBubble)){
			itCur++;
			continue;
		}
		GetAround(itCur->m_nRow, itCur->m_nCol, vecRowCol);
		for (size_t i = 0; i < vecRowCol.size(); i++)
		{
			Bubble* pCurBubble = m_board[vecRowCol[i].m_nRow][vecRowCol[i].m_nCol];//pTempBubble旁边的那个球
			if (!pCurBubble)
				continue;
			//if (pCurBubble->getBubbleColor() == nColor || pCurBubble->getBubbleColor() == BUBBLE_PROPERTY_COLOR||pCurBubble->getBubbleColor()==BUBBLE_VIRUS)
			if (!this->isObstacle(pCurBubble) &&
				(pCurBubble->getBubbleColor() == nColor
				|| pCurBubble->getBubbleColor() == BUBBLE_PROPERTY_COLOR
				|| (pTempBubble->getBubbleColor() == BUBBLE_PROPERTY_COLOR && nColor == BUBBLE_PROPERTY_COLOR)
				|| (pTempBubble->getBubbleColor() == BUBBLE_PROPERTY_COLOR && nColor != BUBBLE_PROPERTY_COLOR && pCurBubble->getBubbleColor() == nColor)))//修改by黄泽昊
			{
				RowCol rc(vecRowCol[i].m_nRow, vecRowCol[i].m_nCol);
				ROWCOL_LIST::iterator itFind = std::find(samelist.begin(), samelist.end(), rc);
				//查找是否已经在列表中
				if (itFind == samelist.end())
				{
					samelist.push_back(vecRowCol[i]);
				}
			}
			count++;
		}
		itCur++;
	} while (itCur != samelist.end());

	if (samelist.size() > 3&&samelist.size()<6)
	{
		if (MyGame::getInstance()->isGameEffectAbled())
			SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_CLEAR4_EFFECT);
	}
	else if (samelist.size()>=6)
	{
		if (MyGame::getInstance()->isGameEffectAbled())
			SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_CLEAR6_EFFECT);
	}
	else if (samelist.size() == 3)
	{
		if (MyGame::getInstance()->isGameEffectAbled())
			SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_CLEAR1_EFFECT);
	}

	if (!(samelist.size() < REMOVE_COUNT))
	{
		//samelist = this->findTheVirusBubble(pReadyBubble);
		this->addTheVirusBubble(samelist);
	}
	else
	{
		samelist.clear();
	}
	return samelist;
}
void GameScene::addTheVirusBubble(ROWCOL_LIST &bubbleList)
{//寻找相同颜色超过3个(包括彩球)的泡泡相邻的病毒泡泡
	ROWCOL_LIST bubbleListTemp = bubbleList;
	for (RowCol bubbleListIter : bubbleListTemp)
	{
		std::vector<RowCol> vecRowCol;
		GetAround(bubbleListIter.m_nRow, bubbleListIter.m_nCol, vecRowCol);
		for (auto vecRowColIter : vecRowCol)
		{
			Bubble* pCurBubble = m_board[vecRowColIter.m_nRow][vecRowColIter.m_nCol];
			auto isHaveFlag = [&](Bubble *tempBubble)->bool{
				for (auto _iter : bubbleListTemp)
				{
					Bubble* _bubble = m_board[_iter.m_nRow][_iter.m_nCol];
					if (*tempBubble == *_bubble)
					{
						return true;
					}
				}
				return false; 
			};
			if (pCurBubble && pCurBubble->getBubbleColor() == BUBBLE_VIRUS && !isHaveFlag(pCurBubble))
			{
				bubbleList.push_back(vecRowColIter);
			}
		}
	}
}

/*ROWCOL_LIST GameScene::findTheVirusBubble(Bubble *pReadyBubble)
{
	ROWCOL_LIST samelist;
	BUBBLE_COLOR nColor = pReadyBubble->getBubbleColor();
	int nRow = pReadyBubble->getRowIndex();
	int nCol = pReadyBubble->getColumnIndex();
	samelist.push_back(RowCol(nRow, nCol));

	ROWCOL_LIST::iterator itCur = samelist.begin();

	//循环查找同样的球
	int count = 0;
	do
	{
		std::vector<RowCol> vecRowCol;
		Bubble*pTempBubble = m_board[itCur->m_nRow][itCur->m_nCol];
		if (pTempBubble && this->isObstacle(pTempBubble)){
			itCur++;
			continue;
		}
		GetAround(itCur->m_nRow, itCur->m_nCol, vecRowCol);

		for (size_t i = 0; i < vecRowCol.size(); i++)
		{
			Bubble* pCurBubble = m_board[vecRowCol[i].m_nRow][vecRowCol[i].m_nCol];
			if (!pCurBubble)
				continue;
			if (!this->isObstacle(pCurBubble)&&
				(pCurBubble->getBubbleColor() == nColor
				|| pCurBubble->getBubbleColor() == BUBBLE_PROPERTY_COLOR
				|| (pTempBubble->getBubbleColor() == BUBBLE_PROPERTY_COLOR && nColor == BUBBLE_PROPERTY_COLOR)
				|| pCurBubble->getBubbleColor() == BUBBLE_VIRUS
				|| (pTempBubble->getBubbleColor() == BUBBLE_PROPERTY_COLOR && nColor != BUBBLE_PROPERTY_COLOR && pCurBubble->getBubbleColor() == nColor)))
			//if (pCurBubble->getBubbleColor() == nColor || pCurBubble->getBubbleColor() == BUBBLE_PROPERTY_COLOR)//修改by黄泽昊
			{
				RowCol rc(vecRowCol[i].m_nRow, vecRowCol[i].m_nCol);
				ROWCOL_LIST::iterator itFind = std::find(samelist.begin(), samelist.end(), rc);
				//查找是否已经在列表中
				if (itFind == samelist.end())
				{
					samelist.push_back(vecRowCol[i]);
				}
			}
			count++;
		}
		itCur++;
	} while (itCur != samelist.end());
	return samelist;
}*/
bool GameScene::isObstacle(Bubble*pBubble)
{
	if (!pBubble || pBubble->getBubbleColor() == BUBBLE_VIRUS
		|| pBubble->getBubbleColor() == BUBBLE_BARRIER_BLOCK
		|| pBubble->getBubbleColor() == BUBBLE_BARRIER_STONE
		|| pBubble->getBubbleColor() == BUBBLE_MYSTICAL)
	{
		return true;
	}
	return false;
}
/*void GameScene::findSameBubble(Bubble *pReadyBubble, ROWCOL_LIST &samelist)
{
	BUBBLE_COLOR nColor = pReadyBubble->getBubbleColor();
	int nRow = pReadyBubble->getRowIndex();
	int nCol = pReadyBubble->getColumnIndex();
	samelist.push_back(RowCol(nRow, nCol));

	ROWCOL_LIST::iterator itCur = samelist.begin();

	//循环查找同样的球
	int count = 0;
	do
	{
		std::vector<RowCol> vecRowCol;

		GetAround(itCur->m_nRow, itCur->m_nCol, vecRowCol);

		for (size_t i = 0; i < vecRowCol.size(); i++)
		{
			Bubble* pCurBubble = m_board[vecRowCol[i].m_nRow][vecRowCol[i].m_nCol];
			if (pCurBubble && (pCurBubble->getBubbleColor() == nColor || pCurBubble->getBubbleColor() == BUBBLE_PROPERTY_COLOR))
			{
				RowCol rc(vecRowCol[i].m_nRow, vecRowCol[i].m_nCol);
				ROWCOL_LIST::iterator itFind = std::find(samelist.begin(), samelist.end(), rc);
				//查找是否已经在列表中
				if (itFind == samelist.end())
				{
					samelist.push_back(vecRowCol[i]);
				}
			}
			count++;
		}
		itCur++;
	} while (itCur != samelist.end());
}
*/

//处理彩球道具泡泡
ROWCOL_LIST GameScene::doColorBubble(Bubble* pReadyBubble)
{
	int nRow = pReadyBubble->getRowIndex();
	int nCol = pReadyBubble->getColumnIndex();

	ROWCOL_LIST clearColorList;
	bool isOver = false;
	std::list<RowCol> tempList = GetAroundList(nRow, nCol);
	std::list<RowCol>::iterator ite = tempList.begin();

	BUBBLE_LIST bubbleTemp;

	//增加彩球装到染色小球的判定
	Bubble * temp = nullptr;
	while (ite != tempList.end())
	{
		RowCol test = *ite;
		Bubble*pBubble = m_board[test.m_nRow][test.m_nCol];
		if (pBubble)
		{
			bubbleTemp.push_back(pBubble);
		}
		if (pBubble && pBubble->getBubbleColor() >= 7 && pBubble->getBubbleColor() <= 13)
		{
			temp = pBubble;
		}
		ite++;
	}
	if (temp)
	{
		this->doDyeBubble(temp);
	}


	BUBBLE_LIST::iterator myite = bubbleTemp.begin();
	while (myite != bubbleTemp.end())
	{
		Bubble*pBubble = *myite;
		ROWCOL_LIST temp = findSameBubble(pBubble);
		//增加闪电泡泡判定 修改by黄泽昊
		if (pBubble->getBubbleColor() == BUBBLE_LIGHTNING)
		{
			return doLightningBubble(pBubble);
		}
		//if (pBubble->getBubbleColor() >= 7 && pBubble->getBubbleColor() <= 13)
		//{
		//	this->doDyeBubble(pBubble);
		//}
		if (temp.size() <= 2)
		{
			myite++;
			continue;
		}

		//temp = findTheVirusBubble(pBubble);//修改by黄泽昊  为了消除旁边的病毒小球
		//temp = collisionWithTheSpecialBubble
		ROWCOL_LIST::iterator iteTest = temp.begin();
		while (iteTest != temp.end())
		{
			RowCol rc = *iteTest;
			ROWCOL_LIST::iterator itFind = std::find(clearColorList.begin(), clearColorList.end(), rc);
			if (itFind == clearColorList.end())
			{
				clearColorList.push_back(rc);
			}
			iteTest++;
		}
		myite++;
	}
	return clearColorList;
}

//处理炸弹道具泡泡
ROWCOL_LIST GameScene::doBombLine(Bubble* pReadyBubble)
{
	int nRow = pReadyBubble->getRowIndex();
	int nCol = pReadyBubble->getColumnIndex();

	if (MyGame::getInstance()->isGameEffectAbled())
		SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_BOMB_EFFECT);

	ROWCOL_LIST clearBombList;
	GetBombLine(nRow, nCol, clearBombList);
	if (!clearBombList.empty())
	{
		//std::list<RowCol>::iterator ite = clearBombList.begin();
		//while (ite != clearBombList.end())
		//{
		//	RowCol tep = *ite;
		//	if (m_board[tep.m_nRow][tep.m_nCol])
		//	{
		//		this->playSequenceFrameAction("baoTX", m_board[tep.m_nRow][tep.m_nCol]->getPosition());
		//	}
		//	ite++;
		//}
		playliziAni(g_animationsName[2].c_str(), pReadyBubble->getPosition().x, pReadyBubble->getPosition().y);
	}
	return clearBombList;
}

//处理闪电泡泡
ROWCOL_LIST GameScene::doLightningBubble(Bubble* pReadyBubble)
{
	int nRow = pReadyBubble->getRowIndex();
	int nCol = pReadyBubble->getColumnIndex();

	ROWCOL_LIST clearLightningList;
	GetLightningLine(nRow, nCol, clearLightningList);
	{
		if (!clearLightningList.empty())
		{
			this->playSequenceFrameAction("shandianTX", ccp(0.0f, pReadyBubble->getPositionY()));
			if (MyGame::getInstance()->isGameEffectAbled())
				SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_LIGHTING_EFFECT);
		}

	}
	return clearLightningList;
}

//处理染色泡泡
bool GameScene::doDyeBubble(Bubble *pBubble)
{
	int collisionToBubblenRow = pBubble->getRowIndex();
	int collisionToBubblenCol = pBubble->getColumnIndex();
	ROWCOL_LIST bubbleList = GetAroundList(collisionToBubblenRow, collisionToBubblenCol);
	bubbleList.push_back(RowCol(collisionToBubblenRow, collisionToBubblenCol));
	int nRow, nCol;
	for (ROWCOL_LIST::const_iterator iterBubble = bubbleList.begin(); iterBubble != bubbleList.end(); iterBubble++)
	{
		nRow = iterBubble->m_nRow;
		nCol = iterBubble->m_nCol;

		Bubble *obj = m_board[nRow][nCol];
		BUBBLE_COLOR changeedColor = pBubble->getBubbleColor();
		switch (pBubble->getBubbleColor())
		{
		case BUBBLE_DYE_BULE:
			changeedColor = BUBBLE_BULE;
			break;
		case BUBBLE_DYE_GREEN:
			changeedColor = BUBBLE_GREEN;
			break;
		case BUBBLE_DYE_ORANGE:
			changeedColor = BUBBLE_ORANGE;
			break;
		case BUBBLE_DYE_PINK:
			changeedColor = BUBBLE_PINK;
			break;
		case BUBBLE_DYE_PURPLE:
			changeedColor = BUBBLE_PURPLE;
			break;
		case BUBBLE_DYE_RED:
			changeedColor = BUBBLE_RED;
			break;
		case BUBBLE_DYE_YELLOW:
			changeedColor = BUBBLE_YELLOW;
			break;
		}

		if (obj)
		{
			int curColor = (int)obj->getBubbleColor();
			if (curColor > 13)
				continue;
			this->removeBubbleFromColorArr(obj);
			obj->setBubbleColor(changeedColor);
			CCSpriteFrame*frameTemp = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(g_bubbleName[changeedColor].c_str());
			obj->setDisplayFrame(frameTemp);
			this->addBubbleToColorArr(obj);
		}
	}
	return true;

}

//消除bubbleList里所有的球
void GameScene::clearBubble(const ROWCOL_LIST &bubbleList)
{
	CCLog("GameScene::clearBubble--Begin clear bubble m_showStartRow is:%d", m_showStartRow);
	//playSequenceFrameAction("");
	int nRow, nCol;
	int clearNum = 0;
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	float minY = size.height;
	float minX = size.width;
	float maxX = 0;
	for (ROWCOL_LIST::const_iterator iterBubble = bubbleList.begin(); iterBubble != bubbleList.end(); iterBubble++)
	{
		nRow = iterBubble->m_nRow; nCol = iterBubble->m_nCol;
		Bubble *obj = m_board[nRow][nCol];
		if (obj)
		{
			clearNum++;
			if (minY > obj->getPositionY())
				minY = obj->getPositionY();
			if (obj->getPositionX() > maxX)
				maxX = obj->getPositionX();
			if (obj->getPositionX() < minX)
				minX = obj->getPositionX();
		}
	}
	CCLog("GameScene::clearBubble--Clear Num is:%d", clearNum);
	int curNum = 0;
	//m_showStartRow=0;
	if (clearNum != 0)
	{
		//if (MyGame::getInstance()->isGameEffectAbled())
		//	SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_CLEAR4_EFFECT);
		//this->runAction(CCFallOffShake::create(0.8f+0.03f*clearNum,10.0f));
		setDisableEnable();
		m_isClearBubble = true;
		m_clearTime = 0.8f + 0.03f*clearNum;

		m_comboNum++;
		if (m_comboNum > 0)
		{
			m_nScore += m_comboNum * 300;
			this->setScoreCountNum();

			UICombo*pCombo = UICombo::create(m_comboNum);
			pCombo->setScale(0.8f);
			//minY-=120;
			CCPoint tempPoint = ccp((minX + maxX) / 2.0f - 120.0f, minY - 150.0f);
			pCombo->setPosition(tempPoint);
			this->addChild(pCombo);
			this->playCombEffect();
			CCSequence*seq = CCSequence::create(CCMoveBy::create(0.8f, ccp(0, 50)), CCCallFuncN::create(this, callfuncN_selector(GameScene::removeObjFromParent)), NULL);
			pCombo->runAction(seq);
		}
		else if (clearNum > 7)
		{
			this->playCombEffect();
			int num = getAnRandomNum(0, 4);
			CCSprite*sp = CCSprite::create(m_comboPngName[num].c_str());
			sp->setPosition(ccp((minX + maxX) / 2.0f, minY));
			this->addChild(sp);
			CCSequence*seq = CCSequence::create(CCMoveBy::create(0.8f, ccp(0, 50)), CCCallFuncN::create(this, callfuncN_selector(GameScene::removeObjFromParent)), NULL);
			sp->runAction(seq);
		}
	}
	else
	{
		m_comboNum = -1;
		m_isClearBubble = false;
		m_clearTime = 0.0f;
	}
	CCLog("GameScene::clearBubble--m_clearTime is:%f", m_clearTime);
	for (ROWCOL_LIST::const_iterator iterBubble = bubbleList.begin(); iterBubble != bubbleList.end(); iterBubble++)
	{
		nRow = iterBubble->m_nRow;
		nCol = iterBubble->m_nCol;

		Bubble *obj = m_board[nRow][nCol];
		if (obj)
		{
			if (BUBBLE_GOLD == obj->getBubbleColor())
			{
				MyGame::getInstance()->addGold(MyGame::BUBBLE_GOLD_REWARD_NUM);
			}

			curNum++;
			if (obj->getBubbleColor() != BUBBLE_GOLD)
			{
				CCCallFuncND*call = CCCallFuncND::create(this, callfuncND_selector(GameScene::createBubbleScore), (void*)curNum);
				CCCallFuncN*callRemove = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeBubbleAction));
				CCSequence*seq = CCSequence::create(CCDelayTime::create(0.03f*curNum), call, callRemove, NULL);
				//CCSequence*seq=CCSequence::create(call,callRemove,NULL);
				obj->runAction(seq);
			}
			else
			{
				this->dealWithAirAndGoldBubble(obj);
			}
			//removeBubbleAction(obj);
			m_board[nRow][nCol] = NULL;
			m_listBubble.remove(obj);
			m_virusBubble.remove(obj);
			this->removeBubbleFromColorArr(obj);
		}
	}
}

void GameScene::playCombEffect()
{
	if (!MyGame::getInstance()->isGameEffectAbled())
	{
		return;
	}
	int num = getAnRandomNum(0, 4);
	switch (num)
	{
	case 0:
		SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_COMBO1_EFFECT);
		break;
	case 1:
		SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_COMBO2_EFFECT);
		break;
	case 2:
		SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_COMBO3_EFFECT);
		break;
	case 3:
		SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_COMBO4_EFFECT);
		break;
	default:
		break;
	}
}
void GameScene::removeBubbleAction(CCNode*pSender)
{
	Bubble*pBubble = (Bubble*)pSender;
	if (!pBubble)
		return;
	if (m_downBubble == pBubble)
		m_downBubble = NULL;
	playliziAni(g_animationsName[0].c_str(), pBubble->getPosition().x, pBubble->getPosition().y);
	pBubble->removeFromParentAndCleanup(true);
}

void GameScene::createBubbleScore(CCNode*pSender, void*pData)

{
	int num = static_cast<int>((int)pData);

	this->createBubbleScore(num, pSender->getPosition());
	this->setProgressPercentage();
	this->setScoreCountNum();
}

void GameScene::createBubbleScore(int curNum, CCPoint pos)
{

	int num = 100 * curNum - 200;

	if (num <= 0)
		num = 100;
	m_nScore += num;

	char str[100] = { 0 };
	sprintf(str, "%d", num);
	CCLabelAtlas*labelAtlas = CCLabelAtlas::create(str, SCORE_PNG, 12, 26, '0');
	labelAtlas->setAnchorPoint(ccp(0.5, 0.5));
	float dy = getAnRandomNum(pos.y - BUBBLE_RADIUS, pos.y + BUBBLE_RADIUS);
	labelAtlas->setPosition(ccp(pos.x, dy));
	this->addChild(labelAtlas, 0, score_tag++);

	CCSequence*seqScale = CCSequence::create(CCScaleTo::create(0.4f, 1.3f), CCScaleTo::create(0.4f, 0.8f), NULL);
	CCSpawn*spaw = CCSpawn::create(CCMoveBy::create(0.8f, ccp(0, 100)), seqScale, CCFadeOut::create(0.8f), NULL);
	CCSequence*seq = CCSequence::create(spaw, CCCallFunc::create(this, callfunc_selector(GameScene::scoreCallFunc)), NULL);
	labelAtlas->runAction(seq);
}

void GameScene::scoreCallFunc()
{
	CCLabelAtlas*labelAtlas = (CCLabelAtlas*)this->getChildByTag(score_tag_temp++);
	labelAtlas->removeFromParentAndCleanup(true);
	if (!this->getChildByTag(score_tag_temp))
	{
		if (m_listBubble.empty())
		{
			passGameSuccess();
			return;
		}
		//m_isClearBubble = false;
		this->updateBoardPos();
	}
}

void GameScene::callbackRemoveBubble(CCNode *obj)
{
	Bubble*pBubble = (Bubble*)obj;
	if (pBubble->getBubbleColor() == BUBBLE_GOLD)
	{
		MyGame::getInstance()->addGold(GOLD_ADD_PER);
		this->setPropertyNumTTF();
	}
	if (m_downBubble == pBubble)
		m_downBubble = NULL;
	if (obj != NULL)
	{
		pBubble->removeFromParentAndCleanup(true);
	}
}

void GameScene::removeObjFromParent(CCNode *obj)
{
	if (!obj)
		return;
	obj->removeFromParentAndCleanup(true);
}

void GameScene::ReadyChangeWaitEnd(CCNode *obj)
{
	if (obj != NULL)
	{
		Bubble* bubble = (Bubble*)obj;
		CCSize size = CCDirector::sharedDirector()->getVisibleSize();
		BUBBLE_COLOR oldColor = bubble->getBubbleColor();
		this->getWidgetByName(UI_NAME)->removeNodeByTag(READY_ARROW);
		m_wait[MAX_WAIT_BUBBLE - 1] = bubble;
		m_wait[MAX_WAIT_BUBBLE - 1]->setPosition(WAITE_BUBBLE_POS);
	}
	setEnable();
}
void GameScene::WaitChangeReadyEnd(CCNode *obj)
{
	if (obj != NULL)
	{
		m_curReady = (Bubble*)obj;
		{
			m_curReady->setPosition(READY_BUBBLE_POS);
			m_curReady->setRotation(m_guideLineMy->getInitialAngle());
		}

		//m_guideLineMy->clearBathNodeChildren();
		{
			m_guideLineMy->setTargetNode(m_curReady);
			m_guideLineMy->setBubbleSpriteFrameName(m_curReady->getBubbleColor());
			m_guideLineMy->changeBathNodeColor();
		}

		int curColor = m_curReady->getBubbleColor();
		std::stringstream ss;
		if (m_curReady->getBubbleColor() != BUBBLE_PROPERTY_COLOR&&m_curReady->getBubbleColor() != BUBBLE_PROPERTY_BOMB)
		{
			ss << UI_RES_ROOT_PATH << "arrow_" << curColor << ".png";
		}
		else
		{
			ss << WHITE_ARROWS_PNG;
		}
		CCSprite * guideArrow = CCSprite::create(ss.str().c_str());
		guideArrow->setPosition(ccp(READY_BUBBLE_POS.x, READY_BUBBLE_POS.y));
		guideArrow->setTag(READY_ARROW);
		guideArrow->setAnchorPoint(ccp(0.5f, -0.1f));
		guideArrow->setRotation(m_curReady->getRotation());
		this->getWidgetByName(UI_NAME)->addNode(guideArrow, 1);
	}
}


typedef Bubble* BUBBLE_POINT;
BUBBLE_LIST GameScene::GetAroundBubble(Bubble*pBubble)
{
	BUBBLE_LIST aroundBubble;
	if (!pBubble)
		return aroundBubble;
	int nRow = pBubble->getRowIndex();
	int nCol = pBubble->getColumnIndex();

	if (!IsValidPos(nRow, nCol))
		return aroundBubble;

	//同一行
	if (IsValidPos(nRow, nCol - 1) && m_board[nRow][nCol - 1])
		aroundBubble.push_back(m_board[nRow][nCol - 1]);
	if (IsValidPos(nRow, nCol + 1) && m_board[nRow][nCol + 1])
		aroundBubble.push_back(m_board[nRow][nCol + 1]);

	if (nRow % 2 == 0) //双行
	{
		if (IsValidPos(nRow - 1, nCol - 1) && m_board[nRow - 1][nCol - 1])//左上角
			aroundBubble.push_back(m_board[nRow - 1][nCol - 1]);
		if (IsValidPos(nRow + 1, nCol) && m_board[nRow + 1][nCol])//右下角
			aroundBubble.push_back(m_board[nRow + 1][nCol]);
		if (IsValidPos(nRow + 1, nCol - 1) && m_board[nRow + 1][nCol - 1])//左下角
			aroundBubble.push_back(m_board[nRow + 1][nCol - 1]);
		if (IsValidPos(nRow - 1, nCol) && m_board[nRow - 1][nCol])//右上角
			aroundBubble.push_back(m_board[nRow - 1][nCol]);
	}
	else
	{
		if (IsValidPos(nRow - 1, nCol) && m_board[nRow - 1][nCol])//左上角
			aroundBubble.push_back(m_board[nRow - 1][nCol]);
		if (IsValidPos(nRow + 1, nCol + 1) && m_board[nRow + 1][nCol + 1])//右下角
			aroundBubble.push_back(m_board[nRow + 1][nCol + 1]);
		if (IsValidPos(nRow + 1, nCol) && m_board[nRow + 1][nCol])//左下角
			aroundBubble.push_back(m_board[nRow + 1][nCol]);
		if (IsValidPos(nRow - 1, nCol + 1) && m_board[nRow - 1][nCol + 1])//右上角
			aroundBubble.push_back(m_board[nRow - 1][nCol + 1]);
	}
	return aroundBubble;
}

ROWCOL_LIST GameScene::checkFallBubble(int startIndex)
{
	CCLog("GameScene::checkFallBubble--check fallbubble index is:%d", startIndex);
	ROWCOL_LIST LinkBubbleList;	//所有连接在一起的球
	m_downBubble = NULL;

	for (int i = 0; i < MAX_COLS - startIndex % 2; i++)
	{
		if (m_board[startIndex][i] != NULL)
		{
			LinkBubbleList.push_back(RowCol(startIndex, i));
		}
	}

	int checkMax = (m_showStartRow + m_checkLine) < MyGame::getInstance()->max_rows ? (m_showStartRow + m_checkLine) : MyGame::getInstance()->max_rows;
	if (LinkBubbleList.empty())//第一行空了
	{
		for (int i = startIndex; i < checkMax; i++)
		{
			for (int j = 0; j < MAX_COLS - i % 2; j++)
			{
				if (m_board[i][j] != NULL)
				{
					RowCol findRowCol(i, j);
					LinkBubbleList.push_back(findRowCol);
					this->adjustDownBubble(m_board[i][j]);
				}
			}
		}
		return LinkBubbleList;
	}

	//从上到下查找所有相连的球，则剩下没有找到的就是不相连的
	ROWCOL_LIST::iterator itCur = LinkBubbleList.begin();
	do
	{
		std::vector<RowCol> vecRowCol;
		GetAround(itCur->m_nRow, itCur->m_nCol, vecRowCol);

		Bubble *pTemp = m_board[itCur->m_nRow][itCur->m_nCol];

		//最下的小球集合更新
		this->adjustDownBubble(pTemp);

		for (size_t i = 0; i < vecRowCol.size(); i++)
		{
			Bubble *pBubble = m_board[vecRowCol[i].m_nRow][vecRowCol[i].m_nCol];
			if (startIndex <= vecRowCol[i].m_nRow && pBubble)
			{
				RowCol pos(vecRowCol[i].m_nRow, vecRowCol[i].m_nCol);
				ROWCOL_LIST::iterator itFind = std::find(LinkBubbleList.begin(), LinkBubbleList.end(), pos);

				//查找是否已经在列表中
				if (itFind == LinkBubbleList.end())
				{
					LinkBubbleList.push_back(vecRowCol[i]);
					//最下的小球集合更新
					this->adjustDownBubble(pBubble);
				}
			}
		}
		itCur++;
	} while (itCur != LinkBubbleList.end());

	ROWCOL_LIST NoLinkBubblelist;	//找出剩下的所有没有连接的球，就是要下落的球
	for (int i = startIndex; i < checkMax; i++)
	{
		for (int j = 0; j < MAX_COLS - i % 2; j++)
		{
			if (m_board[i][j] != NULL)
			{
				RowCol findRowCol(i, j);
				ROWCOL_LIST::iterator itFind = std::find(LinkBubbleList.begin(), LinkBubbleList.end(), findRowCol);
				if (itFind == LinkBubbleList.end())
				{
					NoLinkBubblelist.push_back(findRowCol);
				}
			}
		}
	}

	return NoLinkBubblelist;
}
/*
BUBBLE_LIST GameScene::findFallDownBubble(Bubble*pBubble)
{
	BUBBLE_LIST list;
	return list;
}
*/
//执行可以掉落的泡泡
float GameScene::FallBubble(const ROWCOL_LIST &fallBubbleList)
{
	int index = fallBubbleList.size();
	int curNum = 0;
	for (ROWCOL_LIST::const_iterator iter = fallBubbleList.begin(); iter != fallBubbleList.end(); iter++)
	{
		Bubble *pBubble = m_board[iter->m_nRow][iter->m_nCol];
		if (pBubble != NULL)
		{
			curNum++;
			if (pBubble->getBubbleColor() != BUBBLE_GOLD)
			{
				CCCallFuncND*call = CCCallFuncND::create(this, callfuncND_selector(GameScene::createBubbleScore), (void*)curNum);
				CCCallFuncND*callRemove = CCCallFuncND::create(this, callfuncND_selector(GameScene::downBubbleAction), (void*)index--);
				CCSequence*seq = CCSequence::create(CCDelayTime::create(0.03f*curNum), call, callRemove, NULL);
				pBubble->runAction(seq);
				this->removeBubbleFromColorArr(pBubble);
			}
			else
			{
				this->dealWithAirAndGoldBubble(pBubble);
			}
			m_board[iter->m_nRow][iter->m_nCol] = NULL;
			m_listBubble.remove(pBubble);
			/*修改by黄泽昊*/
			if (pBubble->getBubbleColor() == BUBBLE_VIRUS)
			{
				for (BUBBLE_LIST::iterator iter = m_virusBubble.begin(); iter != m_virusBubble.end(); ++iter)
				{
					if (pBubble == *iter)
					{
						m_virusBubble.erase(iter);
						break;
					}
				}
			}
			//end
		}
	}
	//CCSequence*seq = CCSequence::create(CCDelayTime::create(curNum*0.03f + 1.0f), CCCallFunc::create(this, callfunc_selector(GameScene::setEnable)), NULL);
	//this->runAction(seq);

	if (curNum == 0)
		return 0.0f;
	float tempTime =curNum*0.03f + 0.8f + 0.5f;
	/*修改by黄泽昊*/
	//checkFallBubble(0);//从整个地图开始检索最底下一个球
	//updateBoardPos();//更新小球位置
	//end
	return tempTime;
}

//掉落泡泡
void GameScene::downBubbleAction(CCNode*pSender, void*pData)
{
	float offY = -100;
	Bubble*pBubble = (Bubble*)pSender;
	if (!pSender)
		return;
	int delayValue = static_cast<int>((int)pData);

	CCPoint pos = pBubble->getPosition();
	int dis = getAnRandomNum(20, 100);
	CCSpawn*spaw = CCSpawn::create(CCFadeOut::create(0.5f), CCJumpBy::create(0.5f, ccp(0, -dis), dis, 1), NULL);
	CCSequence*seq = CCSequence::create(spaw, CCCallFuncN::create(this, callfuncN_selector(GameScene::callbackRemoveBubble)), NULL);
	pBubble->runAction(seq);

}

int GameScene::getAnRandomNum(int min, int max)
{
	int randNum = rand() % (max - min) + min;
	return randNum;
}
void GameScene::playliziAni(std::string path, float x, float y)
{
	//读取粒子效果 
	CCParticleSystem* m_emitter1 = CCParticleSystemQuad::create(path.c_str());
	m_emitter1->setPosition(x, y);
	this->addChild(m_emitter1, 10);
	m_emitter1->setAutoRemoveOnFinish(true);//粒子自动释放函数
}

void GameScene::playSequenceFrameAction(char*name, CCPoint pos)
{
	char str[100] = { 0 };
	sprintf(str, "ui/animations/%s/%s.ExportJson", name, name);
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(str);
	//根据动画名称创建动画精灵
	armature = CCArmature::create(name);
	//播放指定动作
	armature->getAnimation()->playWithIndex(0);
	armature->setPosition(pos);
	armature->getAnimation()->setMovementEventCallFunc(this, SEL_MovementEventCallFunc(&GameScene::armatureCallBack));
	this->addChild(armature);
}

void GameScene::armatureCallBack(CCArmature * armature, MovementEventType eventType, const char *name)
{
	if (eventType == COMPLETE)
	{
		armature->removeFromParentAndCleanup(true);
		armature = NULL;
	}
}

//执行病毒感染
void GameScene::virtualInfectionWithBubble()
{
	std::vector<RowCol> virtualAroundBubble;
	for (BUBBLE_LIST::iterator iterVirtualBubble = m_virusBubble.begin(); iterVirtualBubble != m_virusBubble.end(); ++iterVirtualBubble)
	{
		Bubble*pBubble = *iterVirtualBubble;
		GetAround(pBubble->getRowIndex(), pBubble->getColumnIndex(), virtualAroundBubble);
		if (virtualAroundBubble.empty())
			continue;
		for (int i = 0; i < virtualAroundBubble.size(); i++)
		{
			Bubble*pTemp = m_board[virtualAroundBubble[i].m_nRow][virtualAroundBubble[i].m_nCol];
			if (pTemp&&pTemp->getBubbleColor() < 7 && pTemp->getBubbleColor() != BUBBLE_VIRUS&&pTemp->getPositionY()<m_upLine)
			{
				this->removeBubbleFromColorArr(pTemp);
				pTemp->setBubbleColor(BUBBLE_VIRUS);
				CCSpriteFrame*frameTemp = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(g_bubbleName[BUBBLE_VIRUS].c_str());
				pTemp->setDisplayFrame(frameTemp);
				m_virusBubble.push_back(pTemp);
				return;
			}
		}
		virtualAroundBubble.clear();
	}
}

void GameScene::bubbleCollisionAction(BUBBLE_LIST pBubbleList)
{
	if (pBubbleList.empty())
		return;
	Bubble*pBubble = *(pBubbleList.begin());
	for (BUBBLE_LIST::iterator it = pBubbleList.begin(); it != pBubbleList.end(); it++)
	{
		Bubble*pBubble = *it;
		if (!pBubble)
			continue;
		CCPoint point = pBubble->getJumpDirection();
		int jumpRange = pBubble->getJumpRange();
		CCJumpBy*jumpBy = CCJumpBy::create(0.15f, ccp(jumpRange*point.x, jumpRange*point.y), jumpRange, 1);
		CCSequence*seq = CCSequence::create(jumpBy, jumpBy->reverse(), NULL);
		pBubble->runAction(seq);
		//this->shakeActionByDierection(pBubble,point,jumpRange);
		//this->bubbleVibate(pBubble);
	}
	m_collisionBubble.clear();
}


BUBBLE_LIST GameScene::findTheCollisionBubble(Bubble*pBubble)
{
	CCPoint point = ccpNormalize(ccpSub(pBubble->getPosition(), m_curReady->getPosition()));
	if (!m_collisionBubble.empty())
		m_collisionBubble.clear();
	BUBBLE_LIST aroundCur = GetAroundBubble(m_curReady);
	BUBBLE_LIST::iterator iteCur = aroundCur.begin();
	for (iteCur; iteCur != aroundCur.end(); iteCur++)
	{
		Bubble*curAround = *iteCur;
		curAround->setJumpRange(4);
		curAround->setJumpDirection(ccpNormalize(ccpSub(curAround->getPosition(), m_curReady->getPosition())));
		if (curAround->getBubbleColor() == BUBBLE_BARRIER_BLOCK || curAround->getBubbleColor() == BUBBLE_BARRIER_STONE)
			continue;
		m_collisionBubble.push_back(curAround);
		BUBBLE_LIST aroAround = GetAroundBubble(curAround);
		BUBBLE_LIST::iterator iteAround = aroAround.begin();
		for (iteAround; iteAround != aroAround.end(); iteAround++)
		{
			Bubble*pTemp = *iteAround;
			BUBBLE_LIST::iterator iteTemp = find(m_collisionBubble.begin(), m_collisionBubble.end(), pTemp);
			if (iteTemp == m_collisionBubble.end())
			{
				pTemp->setJumpRange(3);
				pTemp->setJumpDirection(ccpNormalize(ccpSub(pTemp->getPosition(), m_curReady->getPosition())));
				if (pTemp->getBubbleColor() == BUBBLE_BARRIER_BLOCK || pTemp->getBubbleColor() == BUBBLE_BARRIER_STONE)
					continue;
				m_collisionBubble.push_back(pTemp);
				BUBBLE_LIST aroThree = GetAroundBubble(pTemp);
				BUBBLE_LIST::iterator iteThree = aroThree.begin();
				for (iteThree; iteThree != aroThree.end(); iteThree++)
				{
					Bubble*pThree = *iteThree;
					BUBBLE_LIST::iterator iteThreeTemp = find(m_collisionBubble.begin(), m_collisionBubble.end(), pThree);
					if (iteThreeTemp == m_collisionBubble.end())
					{
						pThree->setJumpRange(2);
						pThree->setJumpDirection(ccpNormalize(ccpSub(pThree->getPosition(), m_curReady->getPosition())));
						if (pThree->getBubbleColor() == BUBBLE_BARRIER_BLOCK || pThree->getBubbleColor() == BUBBLE_BARRIER_STONE)
							continue;
						m_collisionBubble.push_back(pThree);
					}
				}
			}
		}
	}
	return m_collisionBubble;
}

void GameScene::gameIsOver()
{
	this->setDisableEnable();
	if (m_guideLineMy){
		isUsingGuide = false;
		m_guideLineMy->unDisplayGuideLine();
	}
	//CCDirector::sharedDirector()->getActionManager()->pauseAllRunningActions();
	CCDirector::sharedDirector()->getActionManager()->removeAllActions();

	if (MyGame::getInstance()->isGameEffectAbled())
		SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_PASSFAILED_EFFECT);
	this->fallDownAction();
}

void GameScene::fallDownAction()
{
	BUBBLE_LIST::reverse_iterator ite = m_listBubble.rbegin();
	int curNum = 0;
	float curMax = 0;
	float maxTime = 3.0f;//失败动画最多播放这么长时间
	while (ite != m_listBubble.rend())
	{
		Bubble*pBubble = *ite;
		float dy = 0.04*(pBubble->getPositionY() - 250) / BUBBLE_RADIUS;
		if (dy < 0.000001f)
			dy = 0.0f;
		else
		{
			dy = getAnRandomNum(3, 6) / 6.0f*dy;
		}
		CCPoint tempPoint = ccp(pBubble->getPositionX(), 150 + abs(pBubble->getColumnIndex() - 5) * 10);
		CCActionInterval * easeSineIn = CCEaseSineIn::create(CCMoveTo::create(dy, tempPoint));
		CCSequence*seq = CCSequence::create(CCDelayTime::create(0.03f*curNum + 0.05*(pBubble->getColumnIndex() % 2)), easeSineIn, CCCallFuncN::create(this, callfuncN_selector(GameScene::removeBubbleAction)), NULL);
		curMax = dy + 0.03f*curNum + 0.05*(pBubble->getColumnIndex() % 2);

		pBubble->runAction(seq);

		ite++;
		curNum++;
	}
	if (curMax < maxTime)
		maxTime = curMax;
	CCSequence*sequence = CCSequence::create(CCDelayTime::create(maxTime), CCCallFunc::create(this, callfunc_selector(GameScene::passGameFailed)), NULL);
	this->runAction(sequence);
}

void GameScene::passGameFailed()
{
	//this->unscheduleUpdate();
	this->stopAllActions();
	CCDirector::sharedDirector()->getActionManager()->pauseAllRunningActions();
	this->addChild(UIPassGame::create(false, m_stage, m_nScore, m_starNum), 1);
}

void GameScene::passGameSuccess()
{
	if (m_listBubble.empty())
	{
		if (m_guideLineMy)
			m_guideLineMy->unDisplayGuideLine();
		//this->unscheduleUpdate();
		this->stopAllActions();

		if (MyGame::getInstance()->isGameEffectAbled())
			SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_PASSGAME_EFFECT);

		CCDirector::sharedDirector()->getActionManager()->pauseAllRunningActions();
		XMLTools::getInstance()->unlockMapStageByIndex(m_stage + 1);
		this->addChild(UIPassGame::create(true, m_stage, m_nScore, m_starNum), 1);
	}
}

BUBBLE_COLOR GameScene::getBubbleColorByIndex(int index){
	switch (index)
	{
	case 0:
		return BUBBLE_YELLOW;
	case 1:
		return BUBBLE_PURPLE;
	case 2:
		return BUBBLE_BULE;
	case 3:
		return BUBBLE_RED;
	case 4:
		return BUBBLE_GREEN;
	case 5:
		return BUBBLE_ORANGE;
	case 6:
		return BUBBLE_PINK;
	case 7:
		return BUBBLE_DYE_YELLOW;
	case 8:
		return BUBBLE_DYE_PURPLE;
	case 9:
		return BUBBLE_DYE_BULE;
	case 10:
		return BUBBLE_DYE_RED;
	case 11:
		return BUBBLE_DYE_GREEN;
	case 12:
		return BUBBLE_DYE_ORANGE;
	case 13:
		return BUBBLE_DYE_PINK;
	case 14:
		return BUBBLE_DIED;
	case 15:
		return BUBBLE_ICE;
	case 16:
		return BUBBLE_GOLD;
	case 17:
		return BUBBLE_AIR;
	case 18:
		return BUBBLE_MYSTICAL;
	case 19:
		return BUBBLE_VIRUS;
	case 20:
		return BUBBLE_LIGHTNING;
	case 21:
		return BUBBLE_BLACK_HOLE;
	case 22:
		return BUBBLE_BARRIER_BLOCK;
	case 23:
		return BUBBLE_BARRIER_STONE;
	case 24:
		return BUBBLE_BARRIER_ICE;
	default:
		break;
	}
	return ALL_BUBBLE_COUNT;
}

bool GameScene::IsDistanceOverStandard()
{
	if (!m_downBubble)
		return false;
	float minY = m_downBubble->getPositionY();
	//if(minY>CCDirector::sharedDirector()->getVisibleSize().height-BUBBLE_DROPDOWM_DESTANCE*BOARD_DOWN_DIS)
	//{
	//	m_dropDowndistance=minY-m_diedLinePos.y-BOARD_DOWN_DIS*BUBBLE_BEGIN_DISTANCE;
	//	m_dropDowndistance=m_dropDowndistance<(m_downBoard->getPositionY()-m_upLine+15)?m_dropDowndistance:(m_downBoard->getPositionY()-m_upLine+15);
	//	m_showStartRow-=m_dropDowndistance/BOARD_DOWN_DIS+0.5f;
	//	if(m_showStartRow<0)
	//		m_showStartRow=0;
	//	return true;
	//}
	float temp = m_diedLinePos.y + BOARD_DOWN_DIS*BUBBLE_DROPDOWM_DESTANCE;
	if (minY > temp&&m_showStartRow >= 0)
	{
		m_dropDowndistance = minY - temp;
		m_dropDowndistance = m_dropDowndistance < (m_downBoard->getPositionY() - m_upLine + 15) ? m_dropDowndistance : (m_downBoard->getPositionY() - m_upLine + 15);
		if (m_dropDowndistance <= 0)
			return false;
		int pTemp = (int)(m_dropDowndistance / BOARD_DOWN_DIS + 0.5f);
		m_showStartRow -= pTemp;
		if (m_showStartRow < 0)
			m_showStartRow = 0;
		return true;
	}
	return false;
}

void GameScene::wholeDropDowm()
{
	this->schedule(schedule_selector(GameScene::updateBubblePos));
}

void GameScene::createProgressTimer()
{
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	CCSprite *progressBG = CCSprite::create(PROGRESS_TIMER_BG);
	progressBG->setPosition(ccp(size.width*0.55, size.height*0.936));
	this->addChild(progressBG, 1);
	CCSize bgSize = progressBG->getContentSize();

	CCSprite *progressSprite = CCSprite::create(PROGRESS_TIMER_INIT);

	m_progressTimer = CCProgressTimer::create(progressSprite);
	m_progressTimer->setType(kCCProgressTimerTypeBar);
	m_progressTimer->setPosition(ccp(size.width*0.55, size.height*0.936));
	m_progressTimer->setMidpoint(ccp(0, 0));

	m_progressTimer->setBarChangeRate(ccp(1, 0));
	m_progressTimer->setPercentage(0);
	this->addChild(m_progressTimer, 1);

	CCSprite*oneStar = CCSprite::create(PROGRESS_STAR_DARK_PNG);
	oneStar->setPosition(ccp(bgSize.width * 14 / 24, bgSize.height / 2));
	m_progressTimer->addChild(oneStar, 0, oneStarTag);

	CCSize starSize = oneStar->getContentSize();

	CCSprite*twoStar = CCSprite::create(PROGRESS_STAR_DARK_PNG);
	twoStar->setPosition(ccp(bgSize.width * 18 / 24, bgSize.height / 2));
	CCSprite*twoStarTemp = CCSprite::create(PROGRESS_STAR_DARK_PNG);
	twoStarTemp->setPosition(ccp(starSize.width * 3 / 4, starSize.height / 2));
	twoStar->addChild(twoStarTemp, 0, twoStarTag_one);

	m_progressTimer->addChild(twoStar, 0, twoStarTag);

	CCSprite*threeStar = CCSprite::create(PROGRESS_STAR_DARK_PNG);
	threeStar->setPosition(ccp(bgSize.width * 23 / 24, bgSize.height / 2));
	CCSprite*threeStarTemp1 = CCSprite::create(PROGRESS_STAR_DARK_PNG);
	threeStarTemp1->setPosition(ccp(starSize.width * 3 / 4, starSize.height * 2 / 3));
	threeStar->addChild(threeStarTemp1, 0, threeStarTag_one);
	CCSprite*threeStarTemp2 = CCSprite::create(PROGRESS_STAR_DARK_PNG);
	threeStarTemp2->setPosition(ccp(starSize.width * 8 / 7, starSize.height / 2));
	threeStar->addChild(threeStarTemp2, 0, threeStarTag_two);
	m_progressTimer->addChild(threeStar, 0, threeStarTag);
}

void GameScene::setProgressPercentage()
{
	float threeScore = THREE_STAR_SCORE_PER*m_bubbleCountNum;
	float per = m_nScore / threeScore*100.0f;
	m_progressTimer->setPercentage(per);

	if (per >= 14.0f / 24 * 100 && m_starNum == 0)
		setStarLight(1);
	else if (per >= 18.0f / 24 * 100 && m_starNum == 1)
		setStarLight(2);
	else if (per >= 23.0f / 24 * 100 && m_starNum == 2)
		setStarLight(3);
}

void GameScene::setStarLight(int num)
{
	m_starNum++;
	switch (num)
	{
	case 1:
	{
			  CCSprite*star = (CCSprite*)m_progressTimer->getChildByTag(oneStarTag);
			  star->setTexture(CCTextureCache::sharedTextureCache()->addImage(PROGRESS_STAR_LIGHT_PNG));
	}
		break;
	case 2:
	{
			  CCSprite*star = (CCSprite*)m_progressTimer->getChildByTag(twoStarTag);
			  star->setTexture(CCTextureCache::sharedTextureCache()->addImage(PROGRESS_STAR_LIGHT_PNG));
			  CCSprite*starTemp = (CCSprite*)star->getChildByTag(twoStarTag_one);
			  starTemp->setTexture(CCTextureCache::sharedTextureCache()->addImage(PROGRESS_STAR_LIGHT_PNG));
	}
		break;
	case 3:
	{
			  CCSprite*star = (CCSprite*)m_progressTimer->getChildByTag(threeStarTag);
			  star->setTexture(CCTextureCache::sharedTextureCache()->addImage(PROGRESS_STAR_LIGHT_PNG));
			  CCSprite*starTemp1 = (CCSprite*)star->getChildByTag(threeStarTag_one);
			  starTemp1->setTexture(CCTextureCache::sharedTextureCache()->addImage(PROGRESS_STAR_LIGHT_PNG));
			  CCSprite*starTemp2 = (CCSprite*)star->getChildByTag(threeStarTag_two);
			  starTemp2->setTexture(CCTextureCache::sharedTextureCache()->addImage(PROGRESS_STAR_LIGHT_PNG));
	}
		break;
	default:
		break;
	}
}

void GameScene::keyBackClicked()
{
	//CCDirector::sharedDirector()->replaceScene(StageSelectScene::scene());
	CCNode*pNode=this->getChildByTag(UI_PAUSE_LAYER_TAG);
	if (!pNode)
		this->addChild(UIPauseScene::create(), 2, UI_PAUSE_LAYER_TAG);
	else
		pNode->removeFromParentAndCleanup(true);
}

void GameScene::updateBubblePos(float dt)
{
	m_boardStartPos.y -= 2;
	BUBBLE_LIST::iterator ite = m_listBubble.begin();
	while (ite != m_listBubble.end())
	{
		Bubble*pBubble = *ite;
		pBubble->setPositionY(pBubble->getPositionY() - 2);
		ite++;
	}
	m_downBoard->setPositionY(m_downBoard->getPositionY() - 2);
	m_dropDowndistance -= 2;
	/*if(m_dropDowndistance<=0||m_downBoard->getPositionY()<=CCDirector::sharedDirector()->getVisibleSize().height-4*BUBBLE_RADIUS-10)
		this->unschedule(schedule_selector(GameScene::updateBubblePos));*/
	if (m_dropDowndistance <= 0)
		this->unschedule(schedule_selector(GameScene::updateBubblePos));
}
#include "SimulatorTwoGameScene.h"
#include "commonFunc.h"
#include "StartScene.h"
#include "MyUtilsConfig.h"
#include <algorithm>
#include "StageMapDict.h"
#include "DropDownList.h"
#include "platform/CCFileUtils.h"
#include "support/tinyxml2/tinyxml2.h"
#include "AnimationTools.h"
#include "CommonFunc.h"

using namespace cocos2d;
using namespace gui;
using namespace CocosDenshion;
using namespace tinyxml2;

#define UI_PATH   "ui/simulatorGameScene_2/simulatorGameScene_2.ExportJson"

static const char* BUTTON_NAME_PREFIX = "Button_1_%d";
static const char* BUTTON_NAME_EXPORT = "Button_27";
static const char* BUTTON_NAME_RETURN = "Button_28";
static const char* BUTTON_NAME_UPDATE_BOARD = "Button_31";
static const char* BUTTON_NAME_CLEAR_BUBBLE = "Button_32";
static const char* BOARD_ROW_NAME = "TextField_30";
static const char* BOARD_COLS_NAME = "TextField_29";
static const char* LEVEL_NAME = "TextField_34";
static const char* IMPORT_NAME = "Button_35";
static const char* ONE_STAR_LOWEST_STORE_NAME = "TextField_42_0";
static const char* TWO_STAR_LOWEST_STORE_NAME = "TextField_42_1";
static const char* THREE_STAR_LOWEST_STORE_NAME = "TextField_42";
static const char* SHOOT_BUBBLE_TOTAL_NAME = "TextField_52";

static const char* CHOICE_APPEAR_BUBBLE_YELLOW_CHECKBOX_NAME = "CheckBox_40";
static const char* CHOICE_APPEAR_BUBBLE_PURPLE_CHECKBOX_NAME = "CheckBox_39";
static const char* CHOICE_APPEAR_BUBBLE_BULE_CHECKBOX_NAME = "CheckBox_36";
static const char* CHOICE_APPEAR_BUBBLE_RED_CHECKBOX_NAME = "CheckBox_37";
static const char* CHOICE_APPEAR_BUBBLE_ORANGE_CHECKBOX_NAME = "CheckBox_38";
static const char* CHOICE_APPEAR_BUBBLE_PINK_CHECKBOX_NAME = "CheckBox_41";
static const char* CHOICE_APPEAR_BUBBLE_GREEN_CHECKBOX_NAME = "CheckBox_42";

static const char* ENEMY_ROLE_LISTVIEW_NAME = "ListView_52";
static const char* IS_GLIDE_DOWN_NAME = "CheckBox_55";
static const int PLAY_TYPE_DROP_DOWN_LIST_TAG = 1000;

static const int MOVE_DIS_Y = 2;
static const int CLEAR_BUBBLE = 100;
static const int ADDEDAMN_TAG = 1000;
static const int AMN_TOTAL = 10;
Widget* m_amnWidget = NULL;
SimulatorTwoGameScene::SimulatorTwoGameScene()
{

}

SimulatorTwoGameScene::~SimulatorTwoGameScene()
{
	//clear();
	m_bubblesBatchNode = NULL;
	m_amnWidget = NULL;
	CCSpriteFrameCache::sharedSpriteFrameCache()->purgeSharedSpriteFrameCache();
	CCTextureCache::sharedTextureCache()->purgeSharedTextureCache();
}

CCScene* SimulatorTwoGameScene::scene(int level, bool isRandom)
{
	CCScene* scene = NULL;
	do
	{
		CCEGLView *pEGLView = CCEGLView::sharedOpenGLView();
		pEGLView->setDesignResolutionSize(800, 1334, kResolutionFixedWidth);
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		SimulatorTwoGameScene* layer = SimulatorTwoGameScene::create();
		CC_BREAK_IF(!layer);

		scene->addChild(layer);

	} while (0);

	return scene;
}

bool SimulatorTwoGameScene::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!TouchGroup::init());
		m_start_x = BOARD_START_X;
		m_start_y = BOARD_START_Y;
		m_last_y = BOARD_START_Y;
		m_curBubbleAmnPath = "";
		m_curBubbleColorIndex = -1;
		m_exportIndex = 0;
		m_boardIsMoving = false;
		////加载背景
		//CCSprite *background = CCSprite::create("ui/myRes/beijing.png");
		//CC_BREAK_IF(!background);
		//background->setAnchorPoint(CCPointZero);
		//background->setPosition(CCPointZero);
		//this->addChild(background);

		m_amnWidget = Widget::create();
		Widget *widget = extension::GUIReader::shareReader()->widgetFromJsonFile(UI_PATH);
		CC_BREAK_IF(!widget);

		for (int i = 0; i < ALL_BUBBLE_COUNT - PROPERTY_BUBBLE_COUNT; i++)
		{
			char info[50];
			sprintf(info, BUTTON_NAME_PREFIX, i);
			Button* button = dynamic_cast<Button*>(widget->getChildByName(info));
			CC_BREAK_IF(!button);
			button->addTouchEventListener((CCObject*)this, toucheventselector(SimulatorTwoGameScene::touchedCallBack));
		}

		TextField* shootbubbletotaTextField = dynamic_cast<TextField*>(widget->getChildByName(SHOOT_BUBBLE_TOTAL_NAME));
		shootbubbletotaTextField->setText("0");

		TextField* oneStarLowestStoreTextField = dynamic_cast<TextField*>(widget->getChildByName(ONE_STAR_LOWEST_STORE_NAME));
		oneStarLowestStoreTextField->setText("0");

		TextField* twoStarLowestStoreTextField = dynamic_cast<TextField*>(widget->getChildByName(TWO_STAR_LOWEST_STORE_NAME));
		twoStarLowestStoreTextField->setText("0");

		TextField* threeStarLowestStoreTextField = dynamic_cast<TextField*>(widget->getChildByName(THREE_STAR_LOWEST_STORE_NAME));
		threeStarLowestStoreTextField->setText("0");


		Button* exportButton = dynamic_cast<Button*>(widget->getChildByName(BUTTON_NAME_EXPORT));
		CCLOG("%f",exportButton->getPositionY());
		CC_BREAK_IF(!exportButton);
		exportButton->addTouchEventListener((CCObject*)this, toucheventselector(SimulatorTwoGameScene::touchedCallBack));

		Button* returnButton = dynamic_cast<Button*>(widget->getChildByName(BUTTON_NAME_RETURN));
		CC_BREAK_IF(!returnButton);
		returnButton->addTouchEventListener((CCObject*)this, toucheventselector(SimulatorTwoGameScene::touchedCallBack));

		Button* updateBoardButton = dynamic_cast<Button*>(widget->getChildByName(BUTTON_NAME_UPDATE_BOARD));
		CC_BREAK_IF(!updateBoardButton);
		updateBoardButton->addTouchEventListener((CCObject*)this, toucheventselector(SimulatorTwoGameScene::touchedCallBack));

		Button* clearBubbleButton = dynamic_cast<Button*>(widget->getChildByName(BUTTON_NAME_CLEAR_BUBBLE));
		CC_BREAK_IF(!clearBubbleButton);
		clearBubbleButton->addTouchEventListener((CCObject*)this, toucheventselector(SimulatorTwoGameScene::touchedCallBack));

		Button* importButton = dynamic_cast<Button*>(widget->getChildByName(IMPORT_NAME));
		CC_BREAK_IF(!importButton);
		importButton->addTouchEventListener((CCObject*)this, toucheventselector(SimulatorTwoGameScene::touchedCallBack));

		m_levelInput = dynamic_cast<TextField*>(widget->getChildByName(LEVEL_NAME));
		m_levelInput->setPlaceHolder("level_num");
		m_levelInput->setText("");

		CCLabelTTF* labeldropdown = new CCLabelTTF();
		std::string text2 = "top eliminate";
		labeldropdown->initWithString(text2.c_str(), "黑体", 22);
		CCSize itemSize = CCSizeMake(300, 30);
		azl::DropDownList* pdd = azl::DropDownList::create(labeldropdown, itemSize);
		pdd->setTag(PLAY_TYPE_DROP_DOWN_LIST_TAG);
		CCLabelTTF* dd1 = new CCLabelTTF();
		std::string text_1 = "top eliminate";
		dd1->initWithString(text_1.c_str(), "黑体", 22);
		pdd->addLabel(dd1);

		CCLabelTTF* dd2 = new CCLabelTTF();
		std::string text_2 = "save role";
		dd2->initWithString(text_2.c_str(), "黑体", 22);
		pdd->addLabel(dd2);

		CCLabelTTF* dd3 = new CCLabelTTF();
		std::string text_3 = "beat enemy";
		dd3->initWithString(text_3.c_str(), "黑体", 22);
		pdd->addLabel(dd3);

		CCLabelTTF* dd4 = new CCLabelTTF();
		std::string text_4 = "windmill";
		dd4->initWithString(text_4.c_str(), "黑体", 22);
		pdd->addLabel(dd4);

		CCLabelTTF* dd5 = new CCLabelTTF();
		std::string text_5 = "beat move enemy";
		dd5->initWithString(text_5.c_str(), "黑体", 22);
		pdd->addLabel(dd5);

		pdd->setPosition(465, 334);
		this->addChild(pdd, 12);

		this->addWidget(widget);
		this->addWidget(m_amnWidget);
		CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage("ui/simulatorGameScene_2/gameScene0.png");
		m_bubblesBatchNode = CCSpriteBatchNode::createWithTexture(texture, 1500);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/simulatorGameScene_2/gameScene0.plist", texture);

		widget->addNode(m_bubblesBatchNode, 0);
		int level = 1;
		const StageMapInfo* stageMapInfo = StageMapDict::getInstance()->getStageMapInfos(level);
		int lines = stageMapInfo->m_lines;
		CC_BREAK_IF(!SimulatorTwoGameScene::initBoard(lines, MAX_COLS));



		ListView* enemyRoleLV = dynamic_cast<ListView*>(widget->getChildByName(ENEMY_ROLE_LISTVIEW_NAME));
		CC_BREAK_IF(!enemyRoleLV);
		enemyRoleLV->setTouchEnabled(true);

		for (int i = 0; i < AMN_TOTAL; i++)
		{
			//CCSprite* sprite = CCSprite::createWithSpriteFrameName(path.c_str());

			char info[50];
			sprintf(info, "enemyRole/role_%d.PNG", i);
			UIButton* custom_button = UIButton::create();
			custom_button->setTouchEnabled(true);
			custom_button->loadTextures(info, "", "", UI_TEX_TYPE_PLIST);
			char infoname[50];
			sprintf(infoname, "enemy_role_%d", i);
			custom_button->setName(infoname);
			custom_button->addTouchEventListener((CCObject*)this, toucheventselector(SimulatorTwoGameScene::touchedCallBack));

			Layout * custom_item = Layout::create();
			custom_item->setSize(custom_button->getContentSize());
			custom_button->setPosition(ccp(custom_item->getSize().width / 2, custom_item->getSize().height / 2));
			custom_item->addChild(custom_button);
			enemyRoleLV->pushBackCustomItem(custom_item);

		}

		bRet = true;
	} while (0);

	return bRet;
}



//初始化泡泡队列，
bool SimulatorTwoGameScene::initBoard(int rowValue, int colValue)
{
	m_maxRow = rowValue;
	bool bRet = false;
	do
	{
		for (int row = 0; row < MAX_ROWS; row++)
		{
			for (int col = 0; col < MAX_COLS - row % 2; col++)
			{
				m_board[row][col] = NULL;
			}
		}

		for (int row = 0; row < rowValue; row++)
		{
			for (int col = 0; col < MAX_COLS - row % 2; col++)
			{
				Bubble* pBubble = createBubble(BUBBLE_NONE);
				pBubble->m_hasIceCover = false;
				if (pBubble == NULL)
				{
					CC_BREAK_IF(!pBubble);
				}
				pBubble->setPosition(getPosByRowAndCol(row, col));

				m_bubblesBatchNode->addChild(pBubble, 0);

				m_board[row][col] = pBubble;
				m_board[row][col]->setRowColIndex(row, col);
				m_listBubble.push_back(pBubble);

				bRet = true;
			}
		}
	} while (0);

	return bRet;
}

//初始化泡泡队列，
bool SimulatorTwoGameScene::updateBoard(int rowValue, int colValue)
{
	m_maxRow = rowValue;
	bool bRet = false;
	do
	{
		for (int row = 0; row < MAX_ROWS; row++)
		{
			for (int col = 0; col < MAX_COLS - row % 2; col++)
			{
				if (NULL != m_board[row][col])
				{
					m_board[row][col]->setOpacity(0);
					m_board[row][col]->removeAllChildren();
					m_board[row][col] = NULL;
				}

			}
		}
		if (!m_listBubble.empty()){
			m_listBubble.clear();
		}
		for (int row = 0; row < rowValue; row++)
		{
			for (int col = 0; col < MAX_COLS - row % 2; col++)
			{

				Bubble* pBubble = createBubble(BUBBLE_NONE);
				pBubble->m_hasIceCover = false;

				if (pBubble == NULL)
				{
					CC_BREAK_IF(!pBubble);
				}

				// 
				pBubble->setPosition(getPosByRowAndCol(row, col));

				m_bubblesBatchNode->addChild(pBubble, 0);

				m_board[row][col] = pBubble;
				m_board[row][col]->setRowColIndex(row, col);
				m_listBubble.push_back(pBubble);

				bRet = true;
			}
		}
	} while (0);

	return bRet;
}

Bubble* SimulatorTwoGameScene::createBubble(BUBBLE_COLOR color)
{
	Bubble *pBubble = NULL;
	do
	{
		pBubble = Bubble::create();

		if (pBubble && pBubble->initWithSpriteFrameName(g_v2_bubbleName[color].c_str()))
		{
			pBubble->setBubbleColor(color);
			pBubble->m_resPath = "";
		}
		else
		{
			CC_SAFE_DELETE(pBubble);
		}
	} while (0);
	return pBubble;
}



bool SimulatorTwoGameScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	m_beganLocation = pTouch->getLocation();
	return TouchGroup::ccTouchBegan(pTouch, pEvent);
}

void SimulatorTwoGameScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	TouchGroup::ccTouchMoved(pTouch, pEvent);
	updateBoardPos(pTouch->getLocation().y - m_beganLocation.y);

}

void SimulatorTwoGameScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	m_last_y = m_start_y;
	TouchGroup::ccTouchEnded(pTouch, pEvent);
	if (!m_boardIsMoving)
	{
		setBubbleColor(pTouch->getLocation(), m_curBubbleAmnPath);
	}
	m_boardIsMoving = false;
}

void SimulatorTwoGameScene::touchedCallBack(CCObject* pSender, TouchEventType pType){
	switch (pType){
	case TOUCH_EVENT_BEGAN:
		break;
	case TOUCH_EVENT_MOVED:
		break;
	case TOUCH_EVENT_ENDED:
	{
		Widget *pNode = (Widget*)(pSender);

		if (0 == strcmp(pNode->getName(), BUTTON_NAME_EXPORT))
		{
			exportXmlFile();
		}
		else if (0 == strcmp(pNode->getName(), BUTTON_NAME_RETURN))
		{
			CCDirector::sharedDirector()->replaceScene(StartLayer::scene());
		}
		else if (0 == strcmp(pNode->getName(), BUTTON_NAME_UPDATE_BOARD))
		{
			TextField* rowInput = dynamic_cast<TextField*>(this->getWidgetByName(BOARD_ROW_NAME));
			TextField* colsInput = dynamic_cast<TextField*>(this->getWidgetByName(BOARD_COLS_NAME));
			int row = 10;
			std::stringstream rss;
			rss << rowInput->getStringValue();
			rss >> row;
				updateBoard(row, MAX_COLS);
		}
		else if (0 == strcmp(pNode->getName(), BUTTON_NAME_CLEAR_BUBBLE))
		{
			m_curBubbleColorIndex = CLEAR_BUBBLE;
		}
		else if (0 == strcmp(pNode->getName(), IMPORT_NAME))
		{
			importXmlFile();
		}
		else{
			bool clickEnemyRoleAmn = false;
			std::stringstream ss;
			for (int i = 0; i < AMN_TOTAL; i++)
			{
				char info[50];
				sprintf(info, "enemy_role_%d", i);
				if (0 == strcmp(pNode->getName(), info) && CLEAR_BUBBLE != m_curBubbleColorIndex)
				{
					sprintf(info, "enemyRole/role_%d.PNG", i);
					ss << info;
					ss >> m_curBubbleAmnPath;
					clickEnemyRoleAmn = true;
					break;
				}
			}

			if (!clickEnemyRoleAmn)
			{
				for (int i = 0; i < ALL_BUBBLE_COUNT - PROPERTY_BUBBLE_COUNT; i++)
				{
					char info[50];
					sprintf(info, BUTTON_NAME_PREFIX, i);
					if (0 == strcmp(pNode->getName(), info))
					{
						m_curBubbleColorIndex = i;
						CCLOG(info);
						CCLOG("%d", m_curBubbleColorIndex);
					}
				}
			}
		}
		break;
	}
	case TOUCH_EVENT_CANCELED:
		break;
	default:
		break;
	}
}

//坐标转换 根据行列转化为泡泡的屏幕坐标
CCPoint SimulatorTwoGameScene::getPosByRowAndCol(int row, int col)
{
	float posX, posY;
	posX = m_start_x + col * 2 * V2_BUBBLE_RADIUS + V2_BUBBLE_RADIUS + (row % 2) * V2_BUBBLE_RADIUS;//奇偶行加以区分 偶数行错开半个
	posY = m_start_y - (row * 2 * V2_BUBBLE_RADIUS * sin(PI / 3) + V2_BUBBLE_RADIUS);//等边三角形 sin 60
	return ccp(posX, posY);
}

void SimulatorTwoGameScene::setBubbleColor(CCPoint location, std::string path)
{
	if (0 > m_curBubbleColorIndex)
	{
		return;
	}
	RowCol rowCol = GetRowColByPos(location.x, location.y);
	if (!IsValidPos(rowCol))
	{
		return;
	}
	Bubble*pBubble = m_board[rowCol.m_nRow][rowCol.m_nCol];
	if (pBubble)
	{
		if (location.y < 220)
			return;
		if (BUBBLE_NONE != pBubble->getBubbleColor())
		{
			if (BUBBLE_ICE == m_curBubbleColorIndex)
			{
				pBubble->m_hasIceCover = true;
				CCSprite* sprite = CCSprite::createWithSpriteFrameName(g_v2_bubbleName[m_curBubbleColorIndex].c_str());
				sprite->setAnchorPoint(ccp(0, 0));
				sprite->setOpacity(120);
				sprite->setScale(0.9f);
				pBubble->addChild(sprite);
			}
			else if (CLEAR_BUBBLE == m_curBubbleColorIndex)
			{
				pBubble->removeAllChildren();
				pBubble->setBubbleColor(getBubbleColorByIndex(BUBBLE_NONE));
				pBubble->m_hasIceCover = false;
				CCSpriteFrame* frameTemp = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(g_v2_bubbleName[BUBBLE_NONE].c_str());
				pBubble->setDisplayFrame(frameTemp);
				pBubble->setAnchorPoint(ccp(0.5f, 0.5f));
			}
			else if (BUBBLE_DYE_BULE == m_curBubbleColorIndex)
			{
				pBubble->setBubbleColor(getBubbleColorByIndex(m_curBubbleColorIndex));
				CCSpriteFrame*frameTemp = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(g_v2_bubbleName[m_curBubbleColorIndex].c_str());
				pBubble->setDisplayFrame(frameTemp);
				pBubble->setAnchorPoint(ccp(0.5f, 0.75f));
			}
			else{
				pBubble->setBubbleColor(getBubbleColorByIndex(m_curBubbleColorIndex));
				CCSpriteFrame*frameTemp = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(g_v2_bubbleName[m_curBubbleColorIndex].c_str());
				pBubble->setDisplayFrame(frameTemp);
				pBubble->setAnchorPoint(ccp(0.5f, 0.5f));
			}
			if ("" != path){
				CCSprite* sprite = CCSprite::createWithSpriteFrameName(path.c_str());
				sprite->setAnchorPoint(ccp(0, 0));
				pBubble->addChild(sprite);
				pBubble->m_resPath = path;
				m_curBubbleAmnPath = "";
			}
		}
		else{

			CCLOG("choose bubble ");
			if (BUBBLE_DYE_BULE == m_curBubbleColorIndex)//占3个小球位置的球
			{
				pBubble->setBubbleColor(getBubbleColorByIndex(m_curBubbleColorIndex));
				CCSpriteFrame*frameTemp = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(g_v2_bubbleName[m_curBubbleColorIndex].c_str());
				pBubble->setDisplayFrame(frameTemp);
				pBubble->setAnchorPoint(ccp(0.5f, 0.75f));
			}
			else{
				pBubble->setBubbleColor(getBubbleColorByIndex(m_curBubbleColorIndex));
				CCLOG(g_v2_bubbleName[m_curBubbleColorIndex].c_str());
				CCSpriteFrame*frameTemp = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(g_v2_bubbleName[m_curBubbleColorIndex].c_str());
				pBubble->setDisplayFrame(frameTemp);
			}
		}
	}
}

int SimulatorTwoGameScene::getBubble2Index(BUBBLE_COLOR color)
{//应该与button_1_x   的x相同  存储为返回值下标，与程序对应
	if (color < 7)
	{
		return color;
	}
	switch (color)
	{
	case BUBBLE_LIGHTNING://20
		return 7;
	case BUBBLE_ICE:
		return 8;
	case BUBBLE_BLACK_HOLE:
		return 9;
	case BUBBLE_DYE_YELLOW: //彩色炸弹泡泡
		return 10;
	case BUBBLE_DYE_GREEN://障碍物泡泡 (占用位置个数 7)
		return 13;
	case BUBBLE_DYE_PURPLE://透明泡泡 (占用位置个数 1)
		return 16;
	case BUBBLE_DYE_BULE://透明泡泡 (占用位置个数 3)
		return 17;
	case BUBBLE_DYE_RED:
		return 18;//透明泡泡 (占用位置个数 7)
	case BUBBLE_DYE_ORANGE://随机泡泡
		return 19;
	case BUBBLE_DYE_PINK://加三个发射泡泡
		return 21;
	default:
		break;
	}
	return -1;
}

BUBBLE_COLOR SimulatorTwoGameScene::getBubble2Type(int color)
{//应该与button_1_x   的x相同  存储为返回值下标，与程序对应
	if (color < 7)
	{
		return (BUBBLE_COLOR)color;
	}
	switch (color)
	{
	case 7://20
		return BUBBLE_LIGHTNING;
	case 8:
		return BUBBLE_ICE;
	case 9:
		return BUBBLE_BLACK_HOLE;
	case 10: //彩色炸弹泡泡
		return BUBBLE_DYE_YELLOW;
	case 13://障碍物泡泡 (占用位置个数 7)
		return BUBBLE_DYE_GREEN;
	case 16://透明泡泡 (占用位置个数 1)
		return BUBBLE_DYE_PURPLE;
	case 17://透明泡泡 (占用位置个数 3)
		return BUBBLE_DYE_BULE;
	case 18:
		return BUBBLE_DYE_RED;//透明泡泡 (占用位置个数 7)
	case 19://随机泡泡
		return BUBBLE_DYE_ORANGE;
	case 21://加三个发射泡泡
		return BUBBLE_DYE_PINK;
	default:
		break;
	}
	return BUBBLE_NONE;
}


BUBBLE_COLOR SimulatorTwoGameScene::getBubbleColorByIndex(int index){
	switch (index)//按钮tag _对应的颜色
	{
	case 0:
		return BUBBLE_YELLOW;//黄色
	case 1:
		return BUBBLE_PURPLE;//紫色
	case 2:
		return BUBBLE_BULE;//蓝色
	case 3:
		return BUBBLE_RED;//红色
	case 4:
		return BUBBLE_GREEN;//绿色
	case 5:
		return BUBBLE_ORANGE;//橙色
	case 6:
		return BUBBLE_PINK;//粉色
	case 7:
		return BUBBLE_DYE_YELLOW; //彩色炸弹泡泡
	case 8:
		return BUBBLE_DYE_PURPLE; //透明泡泡 (占用位置个数 1)
	case 9:
		return BUBBLE_DYE_BULE;//透明泡泡 (占用位置个数 3)
	case 10:
		return BUBBLE_DYE_RED;//透明泡泡 (占用位置个数 7)
	case 11:
		return BUBBLE_DYE_GREEN;//障碍物泡泡 (占用位置个数 7)
	case 12:
		return BUBBLE_DYE_ORANGE;//随机泡泡
	case 13:
		return BUBBLE_DYE_PINK;//加三个发射泡泡
	case 14:
		return BUBBLE_DIED;//无
	case 15:
		return BUBBLE_ICE;//云
	case 16:
		return BUBBLE_GOLD;//无
	case 17:
		return BUBBLE_AIR;//无
	case 18:
		return BUBBLE_MYSTICAL;//无
	case 19:
		return BUBBLE_VIRUS;//无
	case 20:
		return BUBBLE_LIGHTNING;//闪电
	case 21:
		return BUBBLE_BLACK_HOLE;//黑洞
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

int SimulatorTwoGameScene::getBubble2TypeToTag(int xmltype)
{//与UI编辑器对应 返回按钮下标 与配表之间的对应	导入文件时专用
	if (xmltype < 7)
	{
		return xmltype;
	}
	switch (xmltype)
	{
	case 7://20闪电
		return 20;
	case 8:
		return 15;//云
	case 9:
		return 21;//黑洞
	case 10: //彩色炸弹泡泡
		return 7;
	case 13://障碍物泡泡 (占用位置个数 7)
		return 11;
	case 16://透明泡泡 (占用位置个数 1)
		return 8;
	case 17://透明泡泡 (占用位置个数 3)
		return 9;
	case 18://透明泡泡 (占用位置个数 7)
		return 10;
	case 19://随机泡泡
		return 12;
	case 21://加三个发射泡泡
		return 13;
	default:
		break;
	}
	return 0;
}

void SimulatorTwoGameScene::updateBoardPos(int y)
{
	if (y > 1 || y < -1)
	{
		m_boardIsMoving = true;
	}
	m_start_y = m_last_y + y;

	for (BUBBLE_LIST::reverse_iterator iterBubble = m_listBubble.rbegin(); iterBubble != m_listBubble.rend(); ++iterBubble)
	{
		Bubble *pBubble = *iterBubble;
		pBubble->setPositionY(m_start_y - (pBubble->getRowIndex() * 2 * V2_BUBBLE_RADIUS * sin(PI / 3) + V2_BUBBLE_RADIUS));//等边三角形 sin 60);
	}
	m_amnWidget->setPositionY(m_start_y - 960);
}


bool SimulatorTwoGameScene::loadMapData(int level)
{

	const StageMapInfo* stageMapInfo = StageMapDict::getInstance()->getStageMapInfos(level);
	vector<vector<std::string> > mapData = stageMapInfo->m_data;
	int lines = stageMapInfo->m_lines;

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

			Bubble*pBubble = m_board[i][j];
			if (color != -1)
			{
				BUBBLE_COLOR tempColor = (BUBBLE_COLOR)color;
				pBubble->setBubbleColor(tempColor);
				CCSpriteFrame*frameTemp = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(g_v2_bubbleName[color].c_str());
				pBubble->setDisplayFrame(frameTemp);
				pBubble->m_hasIceCover = isIceBubble;

				if (isIceBubble)
				{
					CCSprite* sprite = CCSprite::createWithSpriteFrameName(g_v2_bubbleName[BUBBLE_ICE].c_str());
					sprite->setAnchorPoint(ccp(0, 0));
					pBubble->addChild(sprite);
				}
				pBubble->setPosition(getPosByRowAndCol(i, j));

			}

		}
	}
	return true;
}

void SimulatorTwoGameScene::exportXmlFile(){
	//xml文档
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	if (NULL == pDoc) {
		return;
	}
	//xml声明
	XMLDeclaration *pDel = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
	if (NULL == pDel) {
		return;
	}
	pDoc->LinkEndChild(pDel);
	//节点plist
	XMLElement *includeElement = pDoc->NewElement("include");
	pDoc->LinkEndChild(includeElement);
	//节点dict
	XMLElement *bubblesElement = pDoc->NewElement("bubbles");
	XMLElement * stagePropertysElement = pDoc->NewElement("info");

	char info[6];
	char dest[MAX_ROWS*MAX_COLS * 6];
	for (int row = 0; row < m_maxRow; row++)
	{
		for (int col = 0; col < MAX_COLS; col++)
		{
			if (0 != row % 2 && col == MAX_COLS - 1)
			{
			}
			else{
				int colorIndex = getBubble2Index(m_board[row][col]->getBubbleColor());
				if (ALL_BUBBLE_COUNT - PROPERTY_BUBBLE_COUNT - 1 <= colorIndex)
				{
					colorIndex = -1;
				}

				if (-1 != colorIndex)
				{
					XMLElement * bubbleDataElement = pDoc->NewElement("data");
					bubbleDataElement->SetAttribute("type", colorIndex);//给节点设置属性
					CCLOG("colorIndex : %d", colorIndex);
					bubbleDataElement->SetAttribute("row", row);//给节点设置属性
					bubbleDataElement->SetAttribute("col", col);//给节点设置属性
					bubbleDataElement->SetAttribute("cloud", m_board[row][col]->m_hasIceCover);//给节点设置属性
					bubbleDataElement->SetAttribute("path", m_board[row][col]->m_resPath.c_str());//给节点设置属性
					bubblesElement->LinkEndChild(bubbleDataElement);
				}
			}
		}
	}
	includeElement->LinkEndChild(bubblesElement);

	TextField* shootbubbletotaTextField = dynamic_cast<TextField*>(this->getWidgetByName(SHOOT_BUBBLE_TOTAL_NAME));
	stagePropertysElement->SetAttribute("bubbleNum", shootbubbletotaTextField->getStringValue());//给节点设置属性


	azl::DropDownList* pdd = dynamic_cast<azl::DropDownList*>(this->getChildByTag(PLAY_TYPE_DROP_DOWN_LIST_TAG));
	CheckBox* isGlideDown = dynamic_cast<CheckBox*>(this->getWidgetByName(IS_GLIDE_DOWN_NAME));
	stagePropertysElement->SetAttribute("type", (1 << pdd->getSelectedIndex()) + isGlideDown->getSelectedState()?16:0);//给节点设置属性  16: 1<< 4

	TextField* oneStarLowestStoreTextField = dynamic_cast<TextField*>(this->getWidgetByName(ONE_STAR_LOWEST_STORE_NAME));
	stagePropertysElement->SetAttribute("start1", oneStarLowestStoreTextField->getStringValue());//给节点设置属性

	TextField* twoStarLowestStoreTextField = dynamic_cast<TextField*>(this->getWidgetByName(TWO_STAR_LOWEST_STORE_NAME));
	stagePropertysElement->SetAttribute("start2", twoStarLowestStoreTextField->getStringValue());

	TextField* threeStarLowestStoreTextField = dynamic_cast<TextField*>(this->getWidgetByName(THREE_STAR_LOWEST_STORE_NAME));
	stagePropertysElement->SetAttribute("start3 ", threeStarLowestStoreTextField->getStringValue());

	char stageProperty[4];
	char stagePropertys[50];
	strcpy(stagePropertys, "");
	CheckBox* choiceAppearBubbleYellowCheckbox = dynamic_cast<CheckBox*>(this->getWidgetByName(CHOICE_APPEAR_BUBBLE_YELLOW_CHECKBOX_NAME));
	sprintf(stageProperty, "%d", choiceAppearBubbleYellowCheckbox->getSelectedState());
	strcat(stagePropertys, stageProperty);
	CheckBox* choiceAppearBubblePurpleCheckbox = dynamic_cast<CheckBox*>(this->getWidgetByName(CHOICE_APPEAR_BUBBLE_PURPLE_CHECKBOX_NAME));
	sprintf(stageProperty, "%d", choiceAppearBubblePurpleCheckbox->getSelectedState());
	strcat(stagePropertys, stageProperty);
	CheckBox* choiceAppearBubbleBuleCheckbox = dynamic_cast<CheckBox*>(this->getWidgetByName(CHOICE_APPEAR_BUBBLE_BULE_CHECKBOX_NAME));
	sprintf(stageProperty, "%d", choiceAppearBubbleBuleCheckbox->getSelectedState());
	strcat(stagePropertys, stageProperty);
	CheckBox* choiceAppearBubbleRedCheckbox = dynamic_cast<CheckBox*>(this->getWidgetByName(CHOICE_APPEAR_BUBBLE_RED_CHECKBOX_NAME));
	sprintf(stageProperty, "%d", choiceAppearBubbleRedCheckbox->getSelectedState());
	strcat(stagePropertys, stageProperty);
	CheckBox* choiceAppearBubbleGreenCheckbox = dynamic_cast<CheckBox*>(this->getWidgetByName(CHOICE_APPEAR_BUBBLE_GREEN_CHECKBOX_NAME));
	sprintf(stageProperty, "%d", choiceAppearBubbleGreenCheckbox->getSelectedState());
	strcat(stagePropertys, stageProperty);
	CheckBox* choiceAppearBubbleOrangeCheckbox = dynamic_cast<CheckBox*>(this->getWidgetByName(CHOICE_APPEAR_BUBBLE_ORANGE_CHECKBOX_NAME));
	sprintf(stageProperty, "%d", choiceAppearBubbleOrangeCheckbox->getSelectedState());
	strcat(stagePropertys, stageProperty);
	CheckBox* choiceAppearBubblePinkCheckbox = dynamic_cast<CheckBox*>(this->getWidgetByName(CHOICE_APPEAR_BUBBLE_PINK_CHECKBOX_NAME));
	sprintf(stageProperty, "%d", choiceAppearBubblePinkCheckbox->getSelectedState());
	strcat(stagePropertys, stageProperty);

	stagePropertysElement->SetAttribute("colors ", stagePropertys);
	includeElement->LinkEndChild(stagePropertysElement);

	std::string fileOriginalPath = MyUtilsConfig::getInst()->m_userDefineStageXmlPath;
	size_t iPos = fileOriginalPath.find(".xml");
	std::string subPathStr = fileOriginalPath.substr(0, iPos);
	std::stringstream filePath;
	CCLOG("%s", m_levelInput->getStringValue());
	if (0 == strcmp(m_levelInput->getStringValue(), ""))
	{
		filePath << "res/nolevel.xml";
	}
	else
	{
		filePath << subPathStr.c_str() << m_levelInput->getStringValue() << ".xml";
	}
	CCLOG("%s", filePath);
	pDoc->SaveFile(filePath.str().c_str());//保存文件 参数：路径
	//pDoc->Print();//打印
	delete pDoc;

}




void SimulatorTwoGameScene::importXmlFile()
{
	char path[50];
	sprintf(path, "res/stage%s.xml", m_levelInput->getStringValue());
	CCLOG(path);
	if (!CCFileUtils::sharedFileUtils()->isFileExist(path))
	{
		if (!CCFileUtils::sharedFileUtils()->isFileExist("res/nolevel.xml"))
		{
			CCLOG("return");
			updateBoard(10, MAX_COLS);
			return;
		}
		else
		{
			sprintf(path, "res/nolevel.xml");
		}
	}

	CCLOG("hava data ");
	tinyxml2::XMLElement * rootNode = CommonFunc::loadMapDictFile(path);
	tinyxml2::XMLElement * bubblesNode = rootNode->FirstChildElement("bubbles");
	tinyxml2::XMLElement * dataNode = bubblesNode->FirstChildElement("data");
	int row_num = 0;

	while (dataNode){
		int row = 0;
		dataNode->QueryIntAttribute("row", &row);
		if (row > row_num)
		{
			row_num = row;
		}
		dataNode = dataNode->NextSiblingElement("data");
	}
	if (!updateBoard(row_num + 1, MAX_COLS))
	{
		return;
	}
	dataNode = bubblesNode->FirstChildElement("data");
	while (dataNode){
		int type;
		int row;
		int col;
		int cloud;
		dataNode->QueryIntAttribute("type", &type);
		dataNode->QueryIntAttribute("row", &row);
		dataNode->QueryIntAttribute("col", &col);
		dataNode->QueryIntAttribute("cloud", &cloud);
		Bubble* pBubble = m_board[row][col];
		pBubble->setBubbleColor(getBubble2Type(type));
		(cloud == 0) ? (pBubble->m_hasIceCover = false) : (pBubble->m_hasIceCover = true);
		CCLOG("%d", type);
		CCLOG(g_v2_bubbleName[type].c_str());
		CCSpriteFrame* frameTemp = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(g_v2_bubbleName[getBubble2TypeToTag(type)].c_str());
		pBubble->setDisplayFrame(frameTemp);
		pBubble->setAnchorPoint(ccp(0.5f, 0.5f));
		dataNode = dataNode->NextSiblingElement("data");
	}

	tinyxml2::XMLElement * infoNode = bubblesNode->NextSiblingElement("info");
	int bubbleNum;
	int type;
	int start1;
	int start2;
	int start3;
	char color[10];
	infoNode->QueryIntAttribute("bubbleNum", &bubbleNum);
	infoNode->QueryIntAttribute("type", &type);
	infoNode->QueryIntAttribute("start1", &start1);
	infoNode->QueryIntAttribute("start2", &start2);
	infoNode->QueryIntAttribute("start3", &start3);
	strcpy(color, infoNode->FirstAttribute()->Next()->Next()->Next()->Next()->Next()->Value());

	bool b_color[10];
	for (size_t c_index = 0; c_index < 7; c_index++)
	{
		if (color[c_index] == '1')
		{
			b_color[c_index] = true;
		}
		else
		{
			b_color[c_index] = false;
		}		
	}
	
	CheckBox* choiceAppearBubbleYellowCheckbox = dynamic_cast<CheckBox*>(this->getWidgetByName(CHOICE_APPEAR_BUBBLE_YELLOW_CHECKBOX_NAME));
	choiceAppearBubbleYellowCheckbox->setSelectedState(b_color[0]);
	CheckBox* choiceAppearBubblePurpleCheckbox = dynamic_cast<CheckBox*>(this->getWidgetByName(CHOICE_APPEAR_BUBBLE_PURPLE_CHECKBOX_NAME));
	choiceAppearBubblePurpleCheckbox->setSelectedState(b_color[1]);
	CheckBox* choiceAppearBubbleBuleCheckbox = dynamic_cast<CheckBox*>(this->getWidgetByName(CHOICE_APPEAR_BUBBLE_BULE_CHECKBOX_NAME));
	choiceAppearBubbleYellowCheckbox->setSelectedState(b_color[2]);
	CheckBox* choiceAppearBubbleRedCheckbox = dynamic_cast<CheckBox*>(this->getWidgetByName(CHOICE_APPEAR_BUBBLE_RED_CHECKBOX_NAME));
	choiceAppearBubbleYellowCheckbox->setSelectedState(b_color[3]);
	CheckBox* choiceAppearBubbleGreenCheckbox = dynamic_cast<CheckBox*>(this->getWidgetByName(CHOICE_APPEAR_BUBBLE_GREEN_CHECKBOX_NAME));
	choiceAppearBubbleYellowCheckbox->setSelectedState(b_color[4]);
	CheckBox* choiceAppearBubbleOrangeCheckbox = dynamic_cast<CheckBox*>(this->getWidgetByName(CHOICE_APPEAR_BUBBLE_ORANGE_CHECKBOX_NAME));
	choiceAppearBubbleYellowCheckbox->setSelectedState(b_color[5]);
	CheckBox* choiceAppearBubblePinkCheckbox = dynamic_cast<CheckBox*>(this->getWidgetByName(CHOICE_APPEAR_BUBBLE_PINK_CHECKBOX_NAME));
	choiceAppearBubbleYellowCheckbox->setSelectedState(b_color[6]);

	char str_element[5];

	TextField* shootbubbletotaTextField = dynamic_cast<TextField*>(this->getWidgetByName(SHOOT_BUBBLE_TOTAL_NAME));
	sprintf(str_element, "%d", bubbleNum);
	shootbubbletotaTextField->setText(str_element);

	azl::DropDownList* pdd = dynamic_cast<azl::DropDownList*>(this->getChildByTag(PLAY_TYPE_DROP_DOWN_LIST_TAG));
	pdd->setSelectedIndex(getGameTypeIndex(type));
	CheckBox* isGlideDown = dynamic_cast<CheckBox*>(this->getWidgetByName(IS_GLIDE_DOWN_NAME));
	isGlideDown->setSelectedState(type>16?1:0);

	TextField* oneStarLowestStoreTextField = dynamic_cast<TextField*>(this->getWidgetByName(ONE_STAR_LOWEST_STORE_NAME));
	sprintf(str_element, "%d", start1);
	oneStarLowestStoreTextField->setText(str_element);

	TextField* twoStarLowestStoreTextField = dynamic_cast<TextField*>(this->getWidgetByName(TWO_STAR_LOWEST_STORE_NAME));
	sprintf(str_element, "%d", start2);
	twoStarLowestStoreTextField->setText(str_element);

	TextField* threeStarLowestStoreTextField = dynamic_cast<TextField*>(this->getWidgetByName(THREE_STAR_LOWEST_STORE_NAME));
	sprintf(str_element, "%d", start3);
	threeStarLowestStoreTextField->setText(str_element);

	
}
int SimulatorTwoGameScene::getGameTypeIndex(int type){
	switch (type)
	{
	case 1:
	case 16+1:
		return 0;
	case 2:
	case 16 + 2:
		return 1;
	case 4:
	case 16 + 4:
		return 2;
	case 8:
	case 16 + 8:
		return 3;
	default:
		break;
	}
	return 0;
}
//void SimulatorGameScene::exportXmlFile(){
//
//	char info[6];
//	char dest[MAX_ROWS*MAX_COLS * 6];
//	strcpy(dest, "[\n");
//
//	for (int row = 0; row < m_maxRow; row++)
//	{
//		strcat(dest, "[");
//		for (int col = 0; col < MAX_COLS; col++)
//		{
//
//			std::stringstream placeholder;
//			if (MAX_COLS - 1 == col)
//			{
//				placeholder << "%d;%d";
//			}
//			else{
//				placeholder << "%d;%d,";
//			}
//
//			if (0 != row % 2 && col == MAX_COLS - 1)
//			{
//				sprintf(info, placeholder.str().c_str(), -1, false);
//				strcat(dest, info);
//			}
//			else{
//				int colorIndex = m_board[row][col]->getBubbleColor();
//				if (ALL_BUBBLE_COUNT - PROPERTY_BUBBLE_COUNT - 1 <= colorIndex)
//				{
//					colorIndex = -1;
//				}
//				sprintf(info, placeholder.str().c_str(), colorIndex, m_board[row][col]->m_hasIceCover);
//				strcat(dest, info);
//			}
//
//		}
//		strcat(dest, "],\n");
//	}
//	strcat(dest, "]");
//	std::string fileOriginalPath = MyUtilsConfig::getInst()->m_userDefineStageXmlPath;
//	size_t iPos = fileOriginalPath.find(".xml");
//	std::string subPathStr = fileOriginalPath.substr(0, iPos);
//	std::stringstream filePath;
//	filePath << subPathStr.c_str() << m_exportIndex++ << ".xml";
//
//	CommonFunc::createXmlFile(filePath.str().c_str(), 1, dest);
//}


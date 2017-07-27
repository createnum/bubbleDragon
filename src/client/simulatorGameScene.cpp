#include "SimulatorGameScene.h"
#include "commonFunc.h"
#include "StartScene.h"
#include "MyUtilsConfig.h"
#include <algorithm>
#include "StageMapDict.h"
#include "DropDownList.h"

using namespace cocos2d;
using namespace gui;
using namespace CocosDenshion;

#define UI_PATH   "ui/simulatorGameScene/simulatorGameScene.ExportJson"

static const char* BUTTON_NAME_PREFIX = "Button_1_%d";
static const char* BUTTON_NAME_EXPORT = "Button_27";
static const char* BUTTON_NAME_RETURN = "Button_28";
static const char* BUTTON_NAME_UPDATE_BOARD = "Button_31";
static const char* BUTTON_NAME_CLEAR_BUBBLE = "Button_32";
static const char* BOARD_ROW_NAME = "TextField_30";
static const char* BOARD_COLS_NAME = "TextField_29";
static const char* LEVEL_NAME = "TextField_34";
static const char* IMPORT_NAME = "Button_35";

static const int MOVE_DIS_Y = 2;
static const int CLEAR_BUBBLE = 100;
SimulatorGameScene::SimulatorGameScene()
{

}

SimulatorGameScene::~SimulatorGameScene()
{
	//clear();
	m_bubblesBatchNode = NULL;
	CCSpriteFrameCache::sharedSpriteFrameCache()->purgeSharedSpriteFrameCache();
	CCTextureCache::sharedTextureCache()->purgeSharedTextureCache();
}

CCScene* SimulatorGameScene::scene(int level, bool isRandom)
{
	CCScene* scene = NULL;
	do
	{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		SimulatorGameScene* layer = SimulatorGameScene::create();
		CC_BREAK_IF(!layer);

		scene->addChild(layer);

	} while (0);

	return scene;
}

bool SimulatorGameScene::init()
{
	bool bRet = false;
	do
	{

		CC_BREAK_IF(!TouchGroup::init());
		m_start_x = BOARD_START_X;
		m_start_y = BOARD_START_Y;
		m_last_y = BOARD_START_Y;
		m_curBubbleColorIndex = -1;
		m_exportIndex = 0;
		m_boardIsMoving = false;
		////加载背景
		//CCSprite *background = CCSprite::create("ui/myRes/beijing.png");
		//CC_BREAK_IF(!background);
		//background->setAnchorPoint(CCPointZero);
		//background->setPosition(CCPointZero);
		//this->addChild(background);

		Widget *widget = extension::GUIReader::shareReader()->widgetFromJsonFile(UI_PATH);
		CC_BREAK_IF(!widget);

		
		for (int i = 0; i < ALL_BUBBLE_COUNT - PROPERTY_BUBBLE_COUNT; i++)
		{
			char info[50];
			sprintf(info, BUTTON_NAME_PREFIX, i);
			Button* button = dynamic_cast<Button*>(widget->getChildByName(info));
			CC_BREAK_IF(!button);
			button->addTouchEventListener((CCObject*)this, toucheventselector(SimulatorGameScene::touchedCallBack));
		}

		Button* exportButton = dynamic_cast<Button*>(widget->getChildByName(BUTTON_NAME_EXPORT));
		CC_BREAK_IF(!exportButton);
		exportButton->addTouchEventListener((CCObject*)this, toucheventselector(SimulatorGameScene::touchedCallBack));
		
		Button* returnButton = dynamic_cast<Button*>(widget->getChildByName(BUTTON_NAME_RETURN));
		CC_BREAK_IF(!returnButton);
		returnButton->addTouchEventListener((CCObject*)this, toucheventselector(SimulatorGameScene::touchedCallBack));
		
		Button* updateBoardButton = dynamic_cast<Button*>(widget->getChildByName(BUTTON_NAME_UPDATE_BOARD));
		CC_BREAK_IF(!updateBoardButton);
		updateBoardButton->addTouchEventListener((CCObject*)this, toucheventselector(SimulatorGameScene::touchedCallBack));
		
		Button* clearBubbleButton = dynamic_cast<Button*>(widget->getChildByName(BUTTON_NAME_CLEAR_BUBBLE));
		CC_BREAK_IF(!clearBubbleButton);
		clearBubbleButton->addTouchEventListener((CCObject*)this, toucheventselector(SimulatorGameScene::touchedCallBack));

		Button* importButton = dynamic_cast<Button*>(widget->getChildByName(IMPORT_NAME));
		CC_BREAK_IF(!importButton);
		importButton->addTouchEventListener((CCObject*)this, toucheventselector(SimulatorGameScene::touchedCallBack));
		
		this->addWidget(widget);
		CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage("ui/simulatorGameScene/gameScene0.png");
		m_bubblesBatchNode = CCSpriteBatchNode::createWithTexture(texture, 1500);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/simulatorGameScene/gameScene0.plist", texture);
		widget->addNode(m_bubblesBatchNode, 0);
		int level = 1;
		const StageMapInfo* stageMapInfo = StageMapDict::getInstance()->getStageMapInfos(level);
		int lines = stageMapInfo->m_lines;
		CC_BREAK_IF(!SimulatorGameScene::initBoard(lines, MAX_COLS));
		
		CCLabelTTF* labeldropdown = new CCLabelTTF();
		std::string text2 = "playType";
		labeldropdown->initWithString(text2.c_str(), "黑体", 22);
		CCSize itemSize = CCSizeMake(100, 30);
		azl::DropDownList* pdd = azl::DropDownList::create(labeldropdown, itemSize);

		CCLabelTTF* dd1 = new CCLabelTTF();
		std::string text_1 = "playType1";
		dd1->initWithString(text_1.c_str(), "黑体", 22);
		pdd->addLabel(dd1);

		CCLabelTTF* dd2 = new CCLabelTTF();
		std::string text_2 = "playType2";
		dd2->initWithString(text_2.c_str(), "黑体", 22);
		pdd->addLabel(dd2);

		CCLabelTTF* dd3 = new CCLabelTTF();
		std::string text_3 = "playType3";
		dd3->initWithString(text_3.c_str(), "黑体", 22);
		pdd->addLabel(dd3);

		pdd->setPosition(20, 300);
		this->addChild(pdd, 12);
		bRet = true;
	} while (0);

	return bRet;
}



//初始化泡泡队列，
bool SimulatorGameScene::initBoard(int rowValue, int colValue)
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
bool SimulatorGameScene::updateBoard(int rowValue, int colValue)
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

Bubble* SimulatorGameScene::createBubble(BUBBLE_COLOR color)
{
	Bubble *pBubble = NULL;
	do
	{
		pBubble = Bubble::create();

		if (pBubble && pBubble->initWithSpriteFrameName( g_bubbleName[color].c_str()))
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



bool SimulatorGameScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	TouchGroup::ccTouchBegan(pTouch, pEvent);
	m_beganLocation = pTouch->getLocation();
	return true;
}

void SimulatorGameScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	TouchGroup::ccTouchMoved(pTouch, pEvent);
	updateBoardPos(pTouch->getLocation().y - m_beganLocation.y);
	
}

void SimulatorGameScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	m_last_y = m_start_y;
	TouchGroup::ccTouchEnded(pTouch, pEvent);
	if (!m_boardIsMoving)
	{
		setBubbleColor(pTouch->getLocation());
	}
	m_boardIsMoving = false;


}

void SimulatorGameScene::touchedCallBack(CCObject* pSender, TouchEventType pType){
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
			int row = 0;
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
			TextField* levelInput = dynamic_cast<TextField*>(this->getWidgetByName(LEVEL_NAME));
			int level = 0;
			std::stringstream rss;
			rss << levelInput->getStringValue();
			rss >> level;
			const StageMapInfo* stageMapInfo = StageMapDict::getInstance()->getStageMapInfos(level);
			int lines = stageMapInfo->m_lines;
			updateBoard(lines, MAX_COLS);
			loadMapData(level);
		}
		else{

			for (int i = 0; i < ALL_BUBBLE_COUNT - PROPERTY_BUBBLE_COUNT; i++)
			{
				char info[50];
				sprintf(info, BUTTON_NAME_PREFIX, i);
				if (0 == strcmp(pNode->getName(), info))
				{
					m_curBubbleColorIndex = i;
					CCLOG(info);
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
CCPoint SimulatorGameScene::getPosByRowAndCol(int row, int col)
{
	float posX, posY;

	posX = m_start_x + col * 2 * BUBBLE_RADIUS + BUBBLE_RADIUS + (row % 2) * BUBBLE_RADIUS;//奇偶行加以区分 偶数行错开半个
	posY = m_start_y - (row * 2 * BUBBLE_RADIUS * sin(PI / 3) + BUBBLE_RADIUS);//等边三角形 sin 60

	return ccp(posX, posY);
}

void SimulatorGameScene::setBubbleColor(CCPoint location)
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
		if (BUBBLE_NONE != pBubble->getBubbleColor())
		{
			if (BUBBLE_ICE == m_curBubbleColorIndex)
			{
				pBubble->m_hasIceCover = true;
				CCSprite* sprite = CCSprite::createWithSpriteFrameName(g_bubbleName[m_curBubbleColorIndex].c_str());
				sprite->setAnchorPoint(ccp(0, 0));
				pBubble->addChild(sprite);
			}
			else if (CLEAR_BUBBLE == m_curBubbleColorIndex)
			{
				pBubble->setBubbleColor(getBubbleColorByIndex(BUBBLE_NONE));
				pBubble->m_hasIceCover = false;
				CCSpriteFrame* frameTemp = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(g_bubbleName[BUBBLE_NONE].c_str());
				pBubble->setDisplayFrame(frameTemp);
			}
			else{
				pBubble->setBubbleColor(getBubbleColorByIndex(m_curBubbleColorIndex));
				CCSpriteFrame*frameTemp = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(g_bubbleName[m_curBubbleColorIndex].c_str());
				pBubble->setDisplayFrame(frameTemp);
			}
			
		}
		else{
			pBubble->setBubbleColor(getBubbleColorByIndex(m_curBubbleColorIndex));
			CCSpriteFrame*frameTemp = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(g_bubbleName[m_curBubbleColorIndex].c_str());
			pBubble->setDisplayFrame(frameTemp);
		}

	}
}

BUBBLE_COLOR SimulatorGameScene::getBubbleColorByIndex(int index){
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


void SimulatorGameScene::updateBoardPos(int y)
{
	if (y>1 || y<-1)
	{
		m_boardIsMoving = true;
	}
	m_start_y = m_last_y + y;

	for (BUBBLE_LIST::reverse_iterator iterBubble = m_listBubble.rbegin(); iterBubble != m_listBubble.rend(); ++iterBubble)
	{
		Bubble *pBubble = *iterBubble;
		pBubble->setPositionY(m_start_y - (pBubble->getRowIndex() * 2 * BUBBLE_RADIUS * sin(PI / 3) + BUBBLE_RADIUS));//等边三角形 sin 60);
		
	}

}


bool SimulatorGameScene::loadMapData(int level)
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
					CCSpriteFrame*frameTemp = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(g_bubbleName[color].c_str());
					pBubble->setDisplayFrame(frameTemp);
					pBubble->m_hasIceCover = isIceBubble;

					if (isIceBubble)
					{
						CCSprite* sprite = CCSprite::createWithSpriteFrameName(g_bubbleName[BUBBLE_ICE].c_str());
						sprite->setAnchorPoint(ccp(0, 0));
						pBubble->addChild(sprite);
					}
					pBubble->setPosition(getPosByRowAndCol(i, j));

				}

			}
		}
	return true;
}

void SimulatorGameScene::exportXmlFile(){

	char info[6];
	char dest[MAX_ROWS*MAX_COLS * 6];
	strcpy(dest, "");
	for (int row = 0; row < m_maxRow; row++)
	{
		for (int col = 0; col < MAX_COLS; col++)
		{

			if (0 != row % 2 && col == MAX_COLS - 1)
			{
				sprintf(info, "%d,%d;", -1, false);
				strcat(dest, info);
			}
			else{
				int colorIndex = m_board[row][col]->getBubbleColor();
				if (ALL_BUBBLE_COUNT - PROPERTY_BUBBLE_COUNT - 1 <= colorIndex)
				{
					colorIndex = -1;
				}
				sprintf(info, "%d,%d;", colorIndex, m_board[row][col]->m_hasIceCover);
				strcat(dest, info);
			}

		}
	}

	std::string fileOriginalPath = MyUtilsConfig::getInst()->m_userDefineStageXmlPath;
	size_t iPos = fileOriginalPath.find(".xml");
	std::string subPathStr = fileOriginalPath.substr(0, iPos);
	std::stringstream filePath;
	filePath << subPathStr.c_str() << m_exportIndex++ << ".xml";

	CommonFunc::createXmlFile(filePath.str().c_str(), 1, dest);
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


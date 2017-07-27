
#ifndef _SIMULATOR_TWO_GAME_SCENE_H_
#define _SIMULATOR_TWO_GAME_SCENE_H_

#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "Bubble.h"
#include "BubbleConstant.h"
#include "myGame.h"
#include <list>
#include <vector>
#include "cocos-ext.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace gui;
typedef std::list<Bubble*> BUBBLE_LIST;
typedef std::list<RowCol>  ROWCOL_LIST;

#define V2_BUBBLE_RADIUS		(36) //小球半径

static const std::string g_v2_bubbleName[ALL_BUBBLE_COUNT] =
{
	"myRes2/paopaohuang.PNG",
	"myRes2/paopaozi.PNG",
	"myRes2/paopaolan.PNG",
	"myRes2/paopaohong.PNG",
	"myRes2/paopaolv.PNG",
	"myRes2/paopaocheng.PNG",
	"myRes2/paopaofen.PNG",
	"myRes2/paopaozhadan.PNG",
	"myRes2/paopaotouming.PNG",
	"myRes2/paopaotoumingsanjiao.PNG",
	"myRes2/kuangliubianxing.PNG",
	"myRes2/kuangliubianxing.PNG",
	"myRes2/paopaohei.PNG",
	"myRes2/addBubbleNumble.png",
	"myRes/paopaosiwang.png",
	"myRes2/paopaoyun.PNG",
	"myRes/qipaojinbi.png",
	"myRes/qipao.png",
	"myRes/paopaoshenmi.png",//神秘泡泡
	"myRes/paopaobingdu.png",//病毒泡泡
	"myRes2/paopaoshandian.PNG",
	"myRes2/paopaoheidong.PNG",
	"myRes/paopaomukuai.png",
	"myRes2/paopaosuiji.PNG",
	"myRes2/paopaoyun.PNG",
	"myRes2/bubbleNone.png",
	"myRes/daojucaiqiu.png",
	"myRes/daojuzhadan.png",

};

//游戏主场景， 所有的逻辑和表现都在这个场景中完成
class SimulatorTwoGameScene : public cocos2d::gui::TouchGroup
{
public:
	SimulatorTwoGameScene();
	~SimulatorTwoGameScene();

	const static int BOARD_START_X = 5;
	const static int BOARD_START_Y = 960;
public:
	//界面显示
	static cocos2d::CCScene* scene(int level, bool isRandom);

	virtual bool init();

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	void touchedCallBack(CCObject*, cocos2d::gui::TouchEventType);//按钮回调
	BUBBLE_COLOR getBubble2Type(int color);
	CREATE_FUNC(SimulatorTwoGameScene);
	int m_maxRow;
private:
	Bubble* createBubble(BUBBLE_COLOR color);
	bool initBoard(int rowValue, int colValue);
	CCPoint getPosByRowAndCol(int row, int col);

	RowCol GetRowColByPos(float nPosX, float nPosY)
	{
		int nRow, nCol;
		//需要四舍五入
		nPosY = m_start_y - nPosY;

		nRow = (nPosY - V2_BUBBLE_RADIUS) / (2 * V2_BUBBLE_RADIUS *sin(PI / 3)) + 0.5;

		nCol = (nPosX - (nRow % 2) * V2_BUBBLE_RADIUS - V2_BUBBLE_RADIUS - BOARD_START_X) * 1.0 / (2 * V2_BUBBLE_RADIUS) + 0.5;

		return RowCol(nRow, nCol);
	}


	//一个位置是否有效的停靠点
	bool IsValidPos(int nRow, int nCol)
	{
		if (nRow < 0 || nCol < 0)
		{
			return false;
		}
		if (nRow >= MAX_ROWS || nCol >= MAX_COLS - nRow % 2)
		{
			return false;
		}

		return true;
	}

	bool IsValidPos(RowCol nRownCol)
	{
		return IsValidPos(nRownCol.m_nRow, nRownCol.m_nCol);
	}
	void setBubbleColor(CCPoint location, std::string path = "");
	BUBBLE_COLOR getBubbleColorByIndex(int index);
	void updateBoardPos(int y);
	void exportXmlFile();
	void importXmlFile();
	bool updateBoard(int rowValue, int colValue);
	bool loadMapData(int level);
	int getBubble2Index(BUBBLE_COLOR color);
	BUBBLE_COLOR getBubble2IndexWithType(int type);
	int getBubble2TypeToTag(int xmltype);
	int getGameTypeIndex(int type);
	////wstring高字节不为0，返回FALSE
	//bool WStringToString(const std::wstring &wstr, std::string &str)
	//{
	//	int nLen = (int)wstr.length();
	//	str.resize(nLen, ' ');

	//	int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), nLen, (LPSTR)str.c_str(), nLen, NULL, NULL);

	//	if (nResult == 0)
	//	{
	//		return FALSE;
	//	}

	//	return TRUE;
	//}
private:
	TextField* m_levelInput;
	Bubble* m_board[MAX_ROWS][MAX_COLS];		//整个面板所有能放的泡泡
	CCSpriteBatchNode* m_bubblesBatchNode;
	BUBBLE_LIST m_listBubble;		//当前图上所有的球
	int m_curBubbleColorIndex;
	std::string m_curBubbleAmnPath;
	CCPoint m_beganLocation;
	int m_start_x;
	int m_start_y;
	int m_last_y;
	bool m_boardIsMoving;
	int m_exportIndex;

};
#endif
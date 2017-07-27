
#ifndef _SIMULATOR_GAME_SCENE_H_
#define _SIMULATOR_GAME_SCENE_H_

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

typedef std::list<Bubble*> BUBBLE_LIST;
typedef std::list<RowCol>  ROWCOL_LIST;

//��Ϸ�������� ���е��߼��ͱ��ֶ���������������
class SimulatorGameScene : public cocos2d::gui::TouchGroup
{
public:
	SimulatorGameScene();
	~SimulatorGameScene();

	const static int BOARD_START_X = 5;
	const static int BOARD_START_Y = 960;
public:
	//������ʾ
	static cocos2d::CCScene* scene(int level, bool isRandom);

	virtual bool init();

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	void touchedCallBack(CCObject*, cocos2d::gui::TouchEventType);//��ť�ص�

	CREATE_FUNC(SimulatorGameScene);
	int m_maxRow;
private:
	Bubble* createBubble(BUBBLE_COLOR color);
	bool initBoard(int rowValue, int colValue);
	CCPoint getPosByRowAndCol(int row, int col);

	RowCol GetRowColByPos(float nPosX, float nPosY)
	{
		int nRow, nCol;
		//��Ҫ��������
		nPosY = m_start_y - nPosY;

		nRow = (nPosY - BUBBLE_RADIUS) / (2 * BUBBLE_RADIUS *sin(PI / 3)) + 0.5;

		nCol = (nPosX - (nRow % 2) * BUBBLE_RADIUS - BUBBLE_RADIUS - BOARD_START_X) * 1.0 / (2 * BUBBLE_RADIUS) + 0.5;

		return RowCol(nRow, nCol);
	}


	//һ��λ���Ƿ���Ч��ͣ����
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
	void setBubbleColor(CCPoint location);
	BUBBLE_COLOR getBubbleColorByIndex(int index);
	void updateBoardPos(int y);
	void exportXmlFile();
	bool updateBoard(int rowValue, int colValue);
	bool loadMapData(int level);
private:
	Bubble* m_board[MAX_ROWS][MAX_COLS];		//������������ܷŵ�����
	CCSpriteBatchNode* m_bubblesBatchNode;
	BUBBLE_LIST m_listBubble;		//��ǰͼ�����е���
	int m_curBubbleColorIndex;
	CCPoint m_beganLocation;
	int m_start_x;
	int m_start_y;
	int m_last_y;
	bool m_boardIsMoving;
	int m_exportIndex;

};
#endif
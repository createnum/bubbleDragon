/************************************************************************
function:游戏的主逻辑，全部放在此模块中,主要包括:
1、游戏泡泡和等待发射的泡泡初始化
2、游戏触摸的开关和处理
3、泡泡的碰撞检测和处理（调整泡泡的位置）
4、碰撞后查找相同泡泡的算法
5、泡泡的消除
6、消除后是否有悬挂泡泡的检测
************************************************************************/

#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#include "Bubble.h"
#include "BubbleConstant.h"
#include "stageMapInfo.h"

#include <list>
#include <vector>
#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace CocosDenshion;

typedef std::list<Bubble*> BUBBLE_LIST;
typedef std::list<RowCol>  ROWCOL_LIST;

class GuideLine;
//游戏主场景， 所有的逻辑和表现都在这个场景中完成
class GameScene : public cocos2d::gui::TouchGroup
{
public:
	GameScene();
	~GameScene();

public:
	//界面显示
	//const static float GUIDE_POINT_DIS = 20.0f;//
	static cocos2d::CCScene* scene(int level, bool isRandom);

	virtual bool init();
	static GameScene* getInstance()
	{
		return m_inst;
	}

	static CCPoint getBoardStartPos()
	{
		if (m_inst)
		{
			return m_inst->m_boardStartPos;
		}
		else{
			assert(m_inst);
			return ccp(0, 0);
		}
	}

	void loop(float);
	void update(float);//状态更新

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	void touchedCallBack(CCObject*, cocos2d::gui::TouchEventType);//按钮回调
	void disposeTouchEvent(cocos2d::gui::UIWidget*pWidget);
	void screenAdaptive(cocos2d::gui::UIWidget*pWidget);
	void onEnterTransitionDidFinish();
	CREATE_FUNC(GameScene);

private:
	Bubble* randomBubble(BUBBLE_TYPE bubbleType);//随机产生一张颜色的泡泡
	Bubble*randomBubble(std::vector<ColorAndNum> colorVectoe);
	Bubble* createBubble(BUBBLE_COLOR color);//根据颜色创造一种泡泡
	Bubble* createIceBubble(BUBBLE_COLOR color);//根据颜色创造一种冰块泡泡
	void clear();
	CCRect getBoundingBoxByPos(CCPoint pos);

	bool initScheduler();
	bool initReadyBubble();
	bool initBoard();
	bool initWaitBubble();
	bool isCollisionWithBorder(float x);	//X方向是否出界

	bool isCollisionWithTopBorder(Bubble *pBubble);	//Y方向是否出界
	bool isCollisionWithBubble(CCPoint pos1, float radius1, CCPoint pos2, float radius2);	//是否和其他的球碰撞
	bool isCollision();	//是否碰撞，仅包括是否和上方所有的球碰撞和顶层边缘碰撞， 不包括和左右边缘碰撞
	ccColor3B getGuideColor();
	void adjustBubblePosition();//调整泡泡位置
	void adjustBubblePosition(Bubble*pBubble);
	void adjustBubblePos();//Y方向超出
	RowCol calculatePosition(RowCol mindle, float angle);
	RowCol calculateByPosition(RowCol mindle, float angle);
	/**
	*根据三个小球的相对位置来确定当前球应该摆放的位置 A:碰撞的小球 B:即将要去的位置
	*/
	RowCol calculatePositionByTwo(RowCol A, RowCol B);

	ROWCOL_LIST doSpecialBubble(Bubble *pReadyBubble);//根据当前球查找需要清除的球的集合

	ROWCOL_LIST findSameBubble(Bubble *pReadyBubble);//找到同样的球，并返回找到的同样的球的集合
	//ROWCOL_LIST findTheVirusBubble(Bubble*pBubble);//查找要消除的病毒泡泡  增加by黄泽昊
	void addTheVirusBubble(ROWCOL_LIST &bubbleList);//把要消除的泡泡相邻的病毒泡泡添加到消除列表 增加by黄泽昊
	//void findSameBubble(Bubble *pReadyBubble, ROWCOL_LIST &samelist);
	ROWCOL_LIST doLightningBubble(Bubble* pReadyBubble);//处理闪电泡泡
	ROWCOL_LIST doColorBubble(Bubble* pReadyBubble);
	ROWCOL_LIST doBombLine(Bubble* pReadyBubble);

	//根据调整过后的球的位置和颜色类型， 作出相应的处理，如：金银色特殊泡泡的爆炸，球的下落等
	void execClearBubble(Bubble* pReadyBubble);

	void clearBubble(const ROWCOL_LIST &bubbleList);//清除球的集合

	void removeBubbleAction(CCNode*pSender);//清除一个球的表现动作
	void callbackRemoveBubble(CCNode *obj);
	void removeObjFromParent(CCNode *obj);

	ROWCOL_LIST checkFallBubble(int startIndex);

	BUBBLE_LIST collisionBubble();//从后遍历判断碰撞

	float FallBubble(const ROWCOL_LIST &fallBubbleList);//执行可以掉落的泡泡
	void FallBubble();
	void downBubbleAction(CCNode*pSender, void*pData);//掉落泡泡

	void createProgressTimer();
	void changeWaitToReady();//将wait状态的球换成ready状态
	void adjustDownBubble(Bubble* pBubble);//调整最下面的小球
	void setEnable();
	void setDisableEnable();
	void keyBackClicked();
	void addBubbleToColorArr(Bubble*pBubble);
	void removeBubbleFromColorArr(Bubble*pBubble);

	inline bool hasBall(int row, int col)
	{
		return m_board[row][col] != NULL;
	}

	bool doDyeBubble(Bubble *pBubble);
	void clickedProperty(PROPERTY_TYPE pType);

	void updateBoardPos();//更新小球位置
	void clearBarrierIce(ROWCOL_LIST listRowCol);
	bool updateGuide(CCPoint touchPos);//更新辅导线

	void virtualInfectionWithBubble();//病毒感染操作
	void removeCurBubble();//移除当前球
	void removeBubbleWithNoAction(Bubble*pBubble);
	void dealWithAirAndGoldBubble(Bubble*pBubble);
	void playCombEffect();
	void dragByBlackHole(Bubble*pBlackHoleBubble);//小球被黑洞吸走调用
	void bubbleIceDestroy(Bubble*pBubble);//冰块泡泡破碎
	void destoryBubbleWithHammer(CCPoint location);//砸碎泡泡
	void bubbleCollisionAction(BUBBLE_LIST pBubbleList);//泡泡碰撞动画
	BUBBLE_LIST findTheCollisionBubble(Bubble*pBubble);//查找要参与动画的泡泡
	

public:
	void setPropertyNumTTF();//设置技能数
	void gameIsOver();//游戏结束
	void gameResurrection();
private:
	void gameResult();
	long long getCurrentTime();
	void GetTime();
	void passGameFailed();
	void fallDownAction();
	void passGameSuccess();
	int getAnRandomNum(int min, int max);
	BUBBLE_LIST GetAroundBubble(Bubble*pBubble);//获取周围泡泡
	bool isObstacle(Bubble*pBubble);//判断这个泡泡是否为障碍物
	void bubbleCollisonDispose();//碰撞处理
	ROWCOL_LIST collisionWithBubble(BUBBLE_LIST bubbleList);//球球碰撞
	ROWCOL_LIST collisionWithTheSpecialBubble(BUBBLE_LIST bubbleList);//碰到了特殊泡泡 金币 死亡 黑洞 冰块

	void collisionWithDiedBubble(CCNode*pNode);
	bool judgeCollision(int nRow, int nCol);//判断区域碰撞
	//BUBBLE_LIST findFallDownBubble(Bubble*pBubble);//查找可以掉落的球体
	float calculateTheAngle(CCPoint firstPoint, CCPoint secondPoint);//计算角度
	bool IsDistanceOverStandard(); //是否需要整体下落
	void wholeDropDowm();//整体下落
	void setStepsNum(int num);
	void setScoreCountNum();
	
	void updateBubblePos(float dt);
	void goToReadyPos(float dt);
	void createBubbleScore(int num, CCPoint pos);
	void createBubbleScore(CCNode*pSender, void*pData);

	void scoreCallFunc();
	void setProgressPercentage();
	void setStarLight(int num);
	void delayCallBack();
	bool isAnAvailablePos(RowCol temp);

	bool loadMapData(int level, bool isRandom);
	static BUBBLE_COLOR getBubbleColorByIndex(int index);
	void playliziAni(std::string path, float x, float y);//粒子效果动画
	void playSequenceFrameAction(char*pListName, CCPoint pos);//序列帧动画
	void changeBetweenWaitWithReady();
	void ReadyChangeWaitEnd(CCNode *obj);
	void WaitChangeReadyEnd(CCNode *obj);
	void armatureCallBack(cocos2d::extension::CCArmature * armature, cocos2d::extension::MovementEventType eventType, const char *name);
	void setGuideLineMovingTrue();
public:
	int m_stage;
	float m_clearTime;
	int m_clearAction;
	int score_tag;
	int score_tag_temp;
	int partical_tag;
	int partical_tag_temp;
private:
	static GameScene* m_inst;
	bool isUsingGuide;
	//Bubble***m_board;	//整个面板所有能放的泡泡
	std::vector<std::vector<Bubble*> >m_board;

	Bubble* m_wait[MAX_WAIT_BUBBLE];			//正在等待发射的泡泡队列

	Bubble* m_curReady;							//当前正要发射的泡泡

	CCSprite *m_guideArrow;
	CCNode *m_guideLine;

	BUBBLE_LIST m_listBubble;		//当前图上所有的球
	BUBBLE_LIST m_virusBubble;	//病毒泡泡列表
	BUBBLE_LIST m_collisionBubble; //处于碰撞链条的泡泡
	std::vector<ColorAndNum>m_colorCount;
	double m_dRadian;						//范围为30度为150;默认为90度， 竖直方向
	int m_nRotation;						//发射器角度改变方向，也是玩的反弹角度
	float m_guideRotation;
	int m_nWidth;			//游戏界面的宽度和高度
	int m_nHeight;

	int m_nScore;			//游戏所得到的分数
	int m_steps;				//步数
	int m_showStartRow;				//展示起始行数
	double m_dropDowndistance;
	double m_upDistance;
	int m_nGoldenBubbleCount;
	int m_nSliveryBubbleCount;

	GameState m_state;

	CCPoint m_real;
	Bubble * m_collisionToBubble;

	PROPERTY_TYPE m_curUsePropertyType;
	bool m_clickIslocking;
	bool m_isClearBubble;
	bool m_guideLineIsMoving;
	bool m_isHammerSelect;
	cocos2d::gui::Button* pHammerButton;//锤子按钮
	CCPoint m_diedLinePos;//死亡界限
	CCPoint m_boardStartPos;//小球开始点
	Bubble*m_downBubble;//最底的小球
	CCSprite*pDiedLine;
	bool isCollisonWithBlackHole;
	const StageMapInfo*stageMapInfo;
	CCSprite*m_downBoard;
	cocos2d::gui::UILabelAtlas*m_curSatgeAtlas;
	cocos2d::extension::CCArmature*armature;

	CCSprite*m_countDown;
	CCLabelAtlas*m_scoreAtlas;
	CCProgressTimer*m_progressTimer;
	CCSpriteBatchNode *m_bubbleBatchNode;
	CCPoint m_goldPoint;

	int m_bubbleCountNum;
	int m_starNum;
	int m_comboNum;

	GuideLine*m_guideLineMy;
	bool isUItouched;
	float m_upLine;
	int m_checkLine;
	bool m_isHitGoldOrAir;
	bool m_isAddSurScene;
	bool isDisponse;
	bool isChangeWaitToReady;
	bool isCollisionWithDiedBubble;
public:
	bool m_isGameOver;
};

#endif
/************************************************************************
function:��Ϸ�����߼���ȫ�����ڴ�ģ����,��Ҫ����:
1����Ϸ���ݺ͵ȴ���������ݳ�ʼ��
2����Ϸ�����Ŀ��غʹ���
3�����ݵ���ײ���ʹ����������ݵ�λ�ã�
4����ײ�������ͬ���ݵ��㷨
5�����ݵ�����
6���������Ƿ����������ݵļ��
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
//��Ϸ�������� ���е��߼��ͱ��ֶ���������������
class GameScene : public cocos2d::gui::TouchGroup
{
public:
	GameScene();
	~GameScene();

public:
	//������ʾ
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
	void update(float);//״̬����

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	void touchedCallBack(CCObject*, cocos2d::gui::TouchEventType);//��ť�ص�
	void disposeTouchEvent(cocos2d::gui::UIWidget*pWidget);
	void screenAdaptive(cocos2d::gui::UIWidget*pWidget);
	void onEnterTransitionDidFinish();
	CREATE_FUNC(GameScene);

private:
	Bubble* randomBubble(BUBBLE_TYPE bubbleType);//�������һ����ɫ������
	Bubble*randomBubble(std::vector<ColorAndNum> colorVectoe);
	Bubble* createBubble(BUBBLE_COLOR color);//������ɫ����һ������
	Bubble* createIceBubble(BUBBLE_COLOR color);//������ɫ����һ�ֱ�������
	void clear();
	CCRect getBoundingBoxByPos(CCPoint pos);

	bool initScheduler();
	bool initReadyBubble();
	bool initBoard();
	bool initWaitBubble();
	bool isCollisionWithBorder(float x);	//X�����Ƿ����

	bool isCollisionWithTopBorder(Bubble *pBubble);	//Y�����Ƿ����
	bool isCollisionWithBubble(CCPoint pos1, float radius1, CCPoint pos2, float radius2);	//�Ƿ������������ײ
	bool isCollision();	//�Ƿ���ײ���������Ƿ���Ϸ����е�����ײ�Ͷ����Ե��ײ�� �����������ұ�Ե��ײ
	ccColor3B getGuideColor();
	void adjustBubblePosition();//��������λ��
	void adjustBubblePosition(Bubble*pBubble);
	void adjustBubblePos();//Y���򳬳�
	RowCol calculatePosition(RowCol mindle, float angle);
	RowCol calculateByPosition(RowCol mindle, float angle);
	/**
	*��������С������λ����ȷ����ǰ��Ӧ�ðڷŵ�λ�� A:��ײ��С�� B:����Ҫȥ��λ��
	*/
	RowCol calculatePositionByTwo(RowCol A, RowCol B);

	ROWCOL_LIST doSpecialBubble(Bubble *pReadyBubble);//���ݵ�ǰ�������Ҫ�������ļ���

	ROWCOL_LIST findSameBubble(Bubble *pReadyBubble);//�ҵ�ͬ�����򣬲������ҵ���ͬ������ļ���
	//ROWCOL_LIST findTheVirusBubble(Bubble*pBubble);//����Ҫ�����Ĳ�������  ����by�����
	void addTheVirusBubble(ROWCOL_LIST &bubbleList);//��Ҫ�������������ڵĲ���������ӵ������б� ����by�����
	//void findSameBubble(Bubble *pReadyBubble, ROWCOL_LIST &samelist);
	ROWCOL_LIST doLightningBubble(Bubble* pReadyBubble);//������������
	ROWCOL_LIST doColorBubble(Bubble* pReadyBubble);
	ROWCOL_LIST doBombLine(Bubble* pReadyBubble);

	//���ݵ�����������λ�ú���ɫ���ͣ� ������Ӧ�Ĵ����磺����ɫ�������ݵı�ը����������
	void execClearBubble(Bubble* pReadyBubble);

	void clearBubble(const ROWCOL_LIST &bubbleList);//�����ļ���

	void removeBubbleAction(CCNode*pSender);//���һ����ı��ֶ���
	void callbackRemoveBubble(CCNode *obj);
	void removeObjFromParent(CCNode *obj);

	ROWCOL_LIST checkFallBubble(int startIndex);

	BUBBLE_LIST collisionBubble();//�Ӻ�����ж���ײ

	float FallBubble(const ROWCOL_LIST &fallBubbleList);//ִ�п��Ե��������
	void FallBubble();
	void downBubbleAction(CCNode*pSender, void*pData);//��������

	void createProgressTimer();
	void changeWaitToReady();//��wait״̬���򻻳�ready״̬
	void adjustDownBubble(Bubble* pBubble);//�����������С��
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

	void updateBoardPos();//����С��λ��
	void clearBarrierIce(ROWCOL_LIST listRowCol);
	bool updateGuide(CCPoint touchPos);//���¸�����

	void virtualInfectionWithBubble();//������Ⱦ����
	void removeCurBubble();//�Ƴ���ǰ��
	void removeBubbleWithNoAction(Bubble*pBubble);
	void dealWithAirAndGoldBubble(Bubble*pBubble);
	void playCombEffect();
	void dragByBlackHole(Bubble*pBlackHoleBubble);//С�򱻺ڶ����ߵ���
	void bubbleIceDestroy(Bubble*pBubble);//������������
	void destoryBubbleWithHammer(CCPoint location);//��������
	void bubbleCollisionAction(BUBBLE_LIST pBubbleList);//������ײ����
	BUBBLE_LIST findTheCollisionBubble(Bubble*pBubble);//����Ҫ���붯��������
	

public:
	void setPropertyNumTTF();//���ü�����
	void gameIsOver();//��Ϸ����
	void gameResurrection();
private:
	void gameResult();
	long long getCurrentTime();
	void GetTime();
	void passGameFailed();
	void fallDownAction();
	void passGameSuccess();
	int getAnRandomNum(int min, int max);
	BUBBLE_LIST GetAroundBubble(Bubble*pBubble);//��ȡ��Χ����
	bool isObstacle(Bubble*pBubble);//�ж���������Ƿ�Ϊ�ϰ���
	void bubbleCollisonDispose();//��ײ����
	ROWCOL_LIST collisionWithBubble(BUBBLE_LIST bubbleList);//������ײ
	ROWCOL_LIST collisionWithTheSpecialBubble(BUBBLE_LIST bubbleList);//�������������� ��� ���� �ڶ� ����

	void collisionWithDiedBubble(CCNode*pNode);
	bool judgeCollision(int nRow, int nCol);//�ж�������ײ
	//BUBBLE_LIST findFallDownBubble(Bubble*pBubble);//���ҿ��Ե��������
	float calculateTheAngle(CCPoint firstPoint, CCPoint secondPoint);//����Ƕ�
	bool IsDistanceOverStandard(); //�Ƿ���Ҫ��������
	void wholeDropDowm();//��������
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
	void playliziAni(std::string path, float x, float y);//����Ч������
	void playSequenceFrameAction(char*pListName, CCPoint pos);//����֡����
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
	//Bubble***m_board;	//������������ܷŵ�����
	std::vector<std::vector<Bubble*> >m_board;

	Bubble* m_wait[MAX_WAIT_BUBBLE];			//���ڵȴ���������ݶ���

	Bubble* m_curReady;							//��ǰ��Ҫ���������

	CCSprite *m_guideArrow;
	CCNode *m_guideLine;

	BUBBLE_LIST m_listBubble;		//��ǰͼ�����е���
	BUBBLE_LIST m_virusBubble;	//���������б�
	BUBBLE_LIST m_collisionBubble; //������ײ����������
	std::vector<ColorAndNum>m_colorCount;
	double m_dRadian;						//��ΧΪ30��Ϊ150;Ĭ��Ϊ90�ȣ� ��ֱ����
	int m_nRotation;						//�������Ƕȸı䷽��Ҳ����ķ����Ƕ�
	float m_guideRotation;
	int m_nWidth;			//��Ϸ����Ŀ�Ⱥ͸߶�
	int m_nHeight;

	int m_nScore;			//��Ϸ���õ��ķ���
	int m_steps;				//����
	int m_showStartRow;				//չʾ��ʼ����
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
	cocos2d::gui::Button* pHammerButton;//���Ӱ�ť
	CCPoint m_diedLinePos;//��������
	CCPoint m_boardStartPos;//С��ʼ��
	Bubble*m_downBubble;//��׵�С��
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
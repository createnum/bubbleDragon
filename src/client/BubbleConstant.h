/************************************************************************/
/* 

** Note	 : 1����Ϸ��һЩ����������
		   2����Ϸ��һЩö�ٱ���������
		   3����Ϸ��һЩ���ýṹ�������
*/
/************************************************************************/

#ifndef _BUBBLE_CONSTANT_H_
#define _BUBBLE_CONSTANT_H_

#include <string>
#include "cocos2d.h"

#include "myGame.h"

USING_NS_CC;

#define PI					(3.14) //Paiֵ
#define MENU_SPACE			(40)
#define BUBBLE_RADIUS		(28) //С��뾶


#define MAX_ROWS			(155) //�������
#define MAX_COLS			(11)//�������
#define INIT_LINE			(5)//��ʼ����


#define MAX_WAIT_BUBBLE		(1)
#define BUBBLE_SPEED		(28)
#define BUBBLE_COLLISION_RADIUS 25.0f

#define READY_BUBBLE_POS	ccp(320, 150)
#define WAITE_BUBBLE_POS	ccp(205, 135)
#define COUNT_STEPS_POS ccp(100,195)

#define REMOVE_COUNT		(3)
#define SILVERY_COUNT		(4)
#define GOLDEN_COUNT		(5)
#define NORMAL_BUBBLE_COUNT		(7)
#define PROPERTY_BUBBLE_COUNT		(2)
#define ABLE_SHOOT_BUBBLE_COUNT		(9)
#define ALL_ANIMATIONS_COUNT		(4)

#define MAX_ANGLE 70.0f

#define ICE_BUBBLE_CHILD 100

enum BUBBLE_TYPE
{
    BUBBLE_TYPE_NORMAL,
    BUBBLE_TYPE_SPECIAL,
    BUBBLE_TYPE_ALL
};

enum PROPERTY_TYPE
{
    //����
    PROPERTY_TYPE_NONE,
    PROPERTY_TYPE_COLOR,	//����
    PROPERTY_TYPE_BOMB,	//ը��
    PROPERTY_TYPE_HAMMER, //����
    PROPERTY_TYPE_GUIDE //������
};

enum BUBBLE_COLOR
{
    //��ͨ����
	BUBBLE_YELLOW,
	BUBBLE_PURPLE,	
	BUBBLE_BULE,
	BUBBLE_RED,
	BUBBLE_GREEN,
    BUBBLE_ORANGE,
    BUBBLE_PINK,

    //Ⱦɫ����
    BUBBLE_DYE_YELLOW,
    BUBBLE_DYE_PURPLE,	
    BUBBLE_DYE_BULE,
    BUBBLE_DYE_RED,
    BUBBLE_DYE_GREEN,
    BUBBLE_DYE_ORANGE,
    BUBBLE_DYE_PINK,

     //��������
    BUBBLE_DIED,//��������
    BUBBLE_ICE,//��������
    BUBBLE_GOLD,
    BUBBLE_AIR,
    BUBBLE_MYSTICAL,	//��������
    BUBBLE_VIRUS,	//����
    BUBBLE_LIGHTNING,	//����
    BUBBLE_BLACK_HOLE,	//�ڶ�

    //�ϰ���
	BUBBLE_BARRIER_BLOCK,	//ľ��
    BUBBLE_BARRIER_STONE,	//ʯͷ
    BUBBLE_BARRIER_ICE,	//����

	//������
	BUBBLE_NONE,
    //��������
    BUBBLE_PROPERTY_COLOR,	//���� ���Ժ���������������
    BUBBLE_PROPERTY_BOMB,	//ը��

	ALL_BUBBLE_COUNT	//��������
};

enum GameDirection 
{
	LEFT, 
	RIGHT,
	TOP_LEFT, 
	TOP_RIGHT,
	BOTTOM_LEFT, 
	BOTTOM_RIGHT, 
	DIRECTION_COUNT
};

enum GameState 
{
	GS_START,
	GS_FLY,
	GS_FIXED,
	GS_FALL
};

static const std::string g_bubbleName[ALL_BUBBLE_COUNT] = 
{
	"myRes/paopaohuang.png",
	"myRes/paopaozi.png",
	"myRes/paopaolan.png",
	"myRes/paopaohong.png",
	"myRes/paopaolv.png",
	"myRes/paopaocheng.png",
	"myRes/paopaofen.png",
    "myRes/qipaohuang.png",
    "myRes/qipaozi.png",
    "myRes/qipaolan.png",
    "myRes/qipaohong.png",
    "myRes/qipaolv.png",
    "myRes/qipaocheng.png",
    "myRes/qipaofen.png",
    "myRes/paopaosiwang.png",
    "myRes/paopaobingkuai.png",
    "myRes/qipaojinbi.png",
    "myRes/qipao.png",
    "myRes/paopaoshenmi.png",//��������
    "myRes/paopaobingdu.png",//��������
    "myRes/qipaoshandian.png",
    "myRes/paopaoheidong.png",
    "myRes/paopaomukuai.png",
    "myRes/paopaoshikuai.png",
    "myRes/paopaobingkuai.png",
	"myRes/bubbleNone.png",
    "myRes/daojucaiqiu.png",
    "myRes/daojuzhadan.png"
};

static const std::string g_smallBubbleName[] = 
{
	"paopaohuangxiao.png",
	"paopaozixiao.png",
	"paopaolanxiao.png",
	"paopaohongxiao.png",
	"paopaolvxiao.png",
	"paopaochengxiao.png",
	"paopaofenxiao.png",
	"paopaoxiao.png",
};

static const std::string g_animationsName[ALL_ANIMATIONS_COUNT] = 
{
    "ui/animations/paopao/paopao.plist",
	"ui/animations/follow/follow.plist",
	"ui/animations/baoTX/ppzd.plist",
	"ui/animations/finish/finish.plist",
};

struct ColorAndNum
{
	BUBBLE_COLOR m_color;
	int m_num;
	ColorAndNum()
	{
		m_color=BUBBLE_NONE;
		m_num=1;
	}
	ColorAndNum(BUBBLE_COLOR color)
	{
		m_color=color;
		m_num=1;
	}
};

//��Ϸ���нṹ��ķ�װ 
struct RowCol
{
	bool operator==( const RowCol & rPos ) const 
	{
		return m_nRow == rPos.m_nRow && m_nCol == rPos.m_nCol;
	}

	RowCol()
	{
		m_nRow=0;m_nCol=0;
	}

	RowCol( int nRow, int nCol )
	{
		m_nRow = nRow;
		m_nCol = nCol;
	}
	int m_nRow;
	int m_nCol;
};

#endif	//_BUBBLE_CONSTANT_H_
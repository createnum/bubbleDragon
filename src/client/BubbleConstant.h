/************************************************************************/
/* 

** Note	 : 1、游戏中一些常量的声明
		   2、游戏中一些枚举变量的声明
		   3、游戏中一些常用结构体的声明
*/
/************************************************************************/

#ifndef _BUBBLE_CONSTANT_H_
#define _BUBBLE_CONSTANT_H_

#include <string>
#include "cocos2d.h"

#include "myGame.h"

USING_NS_CC;

#define PI					(3.14) //Pai值
#define MENU_SPACE			(40)
#define BUBBLE_RADIUS		(28) //小球半径


#define MAX_ROWS			(155) //最大行数
#define MAX_COLS			(11)//最大列数
#define INIT_LINE			(5)//初始行数


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
    //道具
    PROPERTY_TYPE_NONE,
    PROPERTY_TYPE_COLOR,	//彩球
    PROPERTY_TYPE_BOMB,	//炸弹
    PROPERTY_TYPE_HAMMER, //锤子
    PROPERTY_TYPE_GUIDE //辅助线
};

enum BUBBLE_COLOR
{
    //普通泡泡
	BUBBLE_YELLOW,
	BUBBLE_PURPLE,	
	BUBBLE_BULE,
	BUBBLE_RED,
	BUBBLE_GREEN,
    BUBBLE_ORANGE,
    BUBBLE_PINK,

    //染色泡泡
    BUBBLE_DYE_YELLOW,
    BUBBLE_DYE_PURPLE,	
    BUBBLE_DYE_BULE,
    BUBBLE_DYE_RED,
    BUBBLE_DYE_GREEN,
    BUBBLE_DYE_ORANGE,
    BUBBLE_DYE_PINK,

     //特殊泡泡
    BUBBLE_DIED,//死亡泡泡
    BUBBLE_ICE,//冰块泡泡
    BUBBLE_GOLD,
    BUBBLE_AIR,
    BUBBLE_MYSTICAL,	//神秘泡泡
    BUBBLE_VIRUS,	//病毒
    BUBBLE_LIGHTNING,	//闪电
    BUBBLE_BLACK_HOLE,	//黑洞

    //障碍物
	BUBBLE_BARRIER_BLOCK,	//木块
    BUBBLE_BARRIER_STONE,	//石头
    BUBBLE_BARRIER_ICE,	//冰块

	//空泡泡
	BUBBLE_NONE,
    //道具泡泡
    BUBBLE_PROPERTY_COLOR,	//彩球 可以和任意球相连消除
    BUBBLE_PROPERTY_BOMB,	//炸弹

	ALL_BUBBLE_COUNT	//泡泡总数
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
    "myRes/paopaoshenmi.png",//神秘泡泡
    "myRes/paopaobingdu.png",//病毒泡泡
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

//游戏行列结构体的封装 
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
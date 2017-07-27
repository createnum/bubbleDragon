/************************************************************************/
/* 

** Note	 : 游戏中，泡泡类的组合，包含一些基本成员和函数
*/
/************************************************************************/

#ifndef _BUBBLE_H_
#define _BUBBLE_H_

#include "cocos2d.h"

#include "BubbleConstant.h"

using namespace cocos2d;

class Bubble : public cocos2d::CCSprite
{
public:
	Bubble();
	~Bubble();

public:
	virtual bool init();
	CREATE_FUNC(Bubble);
	bool m_hasIceCover;
	std::string m_resPath;

public:
	void setBubbleColor(BUBBLE_COLOR Color);
	BUBBLE_COLOR getBubbleColor();

	void setRowIndex(int row);//设置行数
	int getRowIndex();

	void setColumnIndex(int col);//设置列数
	int getColumnIndex();

	RowCol getBubbleRowCol();

	void setRowColIndex(int row, int col)//设置行列
	{
		m_nRowIndex = row;
		m_nColIndex = col;
	}

	BUBBLE_COLOR randomColor();		//随机生成一种颜色
	
	void setJumpRange(int range);  //设置跳动幅度
	int getJumpRange();

	void setJumpDirection(CCPoint point);
	CCPoint getJumpDirection();

	cocos2d::CCRect getBoundingBox();
	friend bool operator==(const Bubble &left,const Bubble & right);
private:
	//泡泡所有位置的行和列
	int m_nRowIndex;
	int m_nColIndex;
	int m_jumpRange;//跳动幅度
	CCPoint m_jumpDirection;//跳动方向
	BUBBLE_COLOR m_color;
};
#endif	//_BUBBLE_H_
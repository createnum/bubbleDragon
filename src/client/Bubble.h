/************************************************************************/
/* 

** Note	 : ��Ϸ�У����������ϣ�����һЩ������Ա�ͺ���
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

	void setRowIndex(int row);//��������
	int getRowIndex();

	void setColumnIndex(int col);//��������
	int getColumnIndex();

	RowCol getBubbleRowCol();

	void setRowColIndex(int row, int col)//��������
	{
		m_nRowIndex = row;
		m_nColIndex = col;
	}

	BUBBLE_COLOR randomColor();		//�������һ����ɫ
	
	void setJumpRange(int range);  //������������
	int getJumpRange();

	void setJumpDirection(CCPoint point);
	CCPoint getJumpDirection();

	cocos2d::CCRect getBoundingBox();
	friend bool operator==(const Bubble &left,const Bubble & right);
private:
	//��������λ�õ��к���
	int m_nRowIndex;
	int m_nColIndex;
	int m_jumpRange;//��������
	CCPoint m_jumpDirection;//��������
	BUBBLE_COLOR m_color;
};
#endif	//_BUBBLE_H_
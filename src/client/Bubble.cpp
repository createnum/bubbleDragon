#include "Bubble.h"

Bubble::Bubble()
{
	m_jumpRange=10;
	m_jumpDirection=ccp(0,0);
}

Bubble::~Bubble()
{
}



bool Bubble::init()
{
	if(!CCSprite::init())
		return false;
	m_color = ALL_BUBBLE_COUNT;
	m_nRowIndex = -1;
	m_nColIndex = -1;
	return true;
}

void Bubble::setBubbleColor(BUBBLE_COLOR color)
{
	m_color = color;
	//this->setTexture(CCTextureCache::sharedTextureCache()->addImage(g_bubbleName[color].c_str()));
}

BUBBLE_COLOR Bubble::getBubbleColor()
{
	return m_color;
}

void Bubble::setRowIndex(int row)
{
	m_nRowIndex = row;
}

int Bubble::getRowIndex()
{
	return m_nRowIndex;
}

void Bubble::setColumnIndex(int col)
{
	m_nColIndex = col;
}

int Bubble::getColumnIndex()
{
	return m_nColIndex;
}

BUBBLE_COLOR Bubble::randomColor()
{
	m_color= static_cast<BUBBLE_COLOR>(rand() % (ALL_BUBBLE_COUNT-PROPERTY_BUBBLE_COUNT));
	return m_color;
}

void Bubble::setJumpRange(int range)
{
	m_jumpRange=range;
}
int Bubble::getJumpRange()
{
	return m_jumpRange;
}

void Bubble::setJumpDirection(CCPoint point)
{
	m_jumpDirection=point;
}
CCPoint Bubble::getJumpDirection()
{
	return m_jumpDirection;
}

CCRect Bubble::getBoundingBox()
{
	float R=this->boundingBox().size.width;
	float widthAdjust=R*sin(45.0f*PI/180);
	float ds=(R-widthAdjust)/2;
	CCRect rec=this->boundingBox();
	rec.setRect(rec.origin.x+ds,rec.origin.y+ds,widthAdjust,widthAdjust);
	return rec;
}

RowCol Bubble::getBubbleRowCol()
{
	RowCol temp(m_nRowIndex,m_nColIndex);
	return temp;
}
bool operator==(const Bubble &left, const Bubble & right)
{
	if (left.m_nRowIndex == right.m_nRowIndex && left.m_nColIndex == right.m_nColIndex)
	{
		return true;
	}
	return false;
}
#include "ToolsGuideLine.h"
#include "Utility.h"
#include "BubbleConstant.h"

USING_NS_CC;

//#define PI	 3.14159265
#define BUBBLE_DISTANCE 30
#define BUBBLE_MOVE_DIS 2

#define BUBBLE_PLIST "ui/myRes/smallBubble.plist"
#define BUBBLE_IMAGE "ui/myRes/smallBubble.png"

/************************************************************************/
/* class: GuideBubble
/* func: the element of the guide line,the difference from sprite is the variable of direction
/*
/************************************************************************/

GuideBubble::GuideBubble()
{
	m_direction=1;
}

void GuideBubble::setBubbleDirection(int dir)
{
	m_direction=dir;
}

int GuideBubble::getBubbleDirection()
{
	return m_direction;
}

GuideBubble* GuideBubble::createWithTexture(cocos2d::CCTexture2D *pTexture)
{ 
	GuideBubble *pBubble = new GuideBubble();
	if (pBubble && pBubble->initWithTexture(pTexture))
	{
		pBubble->autorelease();
		return pBubble;
	}
	CC_SAFE_DELETE(pBubble);
	return NULL;
};

//bool GuideBubble::initWithSpriteFrameName(const char *pszSpriteFrameName)
//{
//	CCAssert(pszSpriteFrameName != NULL, "");
//
//	CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);
//	return initWithSpriteFrame(pFrame);
//}

/************************************************************************/
/* class: GuideLine
/* func: the element of the guide line,the difference from sprite is the variable of direction
/*
/************************************************************************/

GuideLine::GuideLine():
m_leftBoundary(0),
m_rightBoundary(0),
m_upBoundary(0),
m_downBoundary(0),
m_time(0),
m_initAngle(0.0f),
pszSpriteFrameNum(0)
{
	m_targetRoot=NULL;
	m_bathNode=NULL;
	m_lastBubble=NULL;
}

GuideLine::~GuideLine()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(BUBBLE_PLIST);
	this->unDisplayGuideLine();
}

bool GuideLine::init(CCNode*pTarget,char*pListfileImage)
{
	if(!CCNode::init()||!pTarget)
	{
		return false;
	}
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(BUBBLE_PLIST);

	m_targetRoot=pTarget;
	m_bathNode=CCSpriteBatchNode::create(BUBBLE_IMAGE);
	m_bathNode->setPosition(pTarget->getPosition());
	this->addChild(m_bathNode);

	CCSize size=CCDirector::sharedDirector()->getVisibleSize();
	this->setBoundaryLine(BUBBLE_RADIUS,size.width-BUBBLE_RADIUS,BUBBLE_RADIUS,size.height-BUBBLE_RADIUS);

	cc_timeval psv;
	CCTime::gettimeofdayCocos2d(&psv, NULL);
	unsigned long int rand_seed = psv.tv_sec*1000 + psv.tv_usec/1000;    
	srand(rand_seed);

	return true;
}

GuideLine*GuideLine::create(CCNode*pTarget,char*fileImage)
{
	GuideLine*pLine=new GuideLine();
	if(pLine&&pLine->init(pTarget,fileImage))
	{
		pLine->autorelease();
	}
	else
	{
		delete pLine;
		pLine=NULL;
	}
	return pLine;
}

void GuideLine::setTargetNode(cocos2d::CCNode*pTarget)
{
	m_targetRoot=pTarget;
}

void GuideLine::setBubbleSpriteFrameName(int num)
{
	if(num>=7)
		num=7;
	pszSpriteFrameNum=num;
}

void GuideLine::setBoundaryLine(float minX,float maxX,float minY,float maxY)
{
	m_leftBoundary=minX;
	m_rightBoundary=maxX;
	m_downBoundary=minY;
	m_upBoundary=maxY;
}

float GuideLine::calculateAngle(CCPoint rootPoint,CCPoint curPoint)
{
	float tempX = (curPoint.x-rootPoint.x);
	float tempY = (curPoint.y-rootPoint.y);

	float angleValue = PI/2 - atan2(tempY, tempX);
	float rotation=angleValue*180/PI;

	return rotation;
}

void GuideLine::displayGuideLine(CCPoint touchPos,bool isUpdateEnabled)
{
	if(!m_bathNode||m_bathNode->getChildrenCount()!=0)
		return;
	m_bathNode->setVisible(true);
	float rotation=calculateAngle(m_targetRoot->getPosition(),touchPos);
	if(rotation>MAX_ANGLE||rotation<-MAX_ANGLE)
		return;
	m_targetRoot->setRotation(rotation);
	this->setInitialAngle(rotation);

	rotation=rotation*PI/180.0f;

	CCSize size=CCDirector::sharedDirector()->getVisibleSize();
	bool islastOne=false;
	int num=0;

	while(!islastOne)
	{
		islastOne=isBubbleOutLine(createBubbleOnGuideLine(num,rotation));
		num++;
	}
	if(isUpdateEnabled)
		this->schedule(schedule_selector(GuideLine::updateGuideLine),1.0f/60);
}

void GuideLine::unDisplayGuideLine()
{
	this->unschedule(schedule_selector(GuideLine::updateGuideLine));
	m_bathNode->removeAllChildrenWithCleanup(true);
	m_time=0;
}


void GuideLine::setGuideLineVisible(bool visible)
{
	m_bathNode->setVisible(visible);
}

void GuideLine::rotateGuideLine(CCPoint touchPos)
{
	float rotation=calculateAngle(m_targetRoot->getPosition(),touchPos);
	if(rotation>MAX_ANGLE||rotation<-MAX_ANGLE)
		return;
	m_lastBubble=NULL;

	float preAngle=m_targetRoot->getRotation();

	m_targetRoot->setRotation(rotation);
	this->setInitialAngle(rotation);
	rotation=rotation*PI/180.0f;

	CCArray*bubbleArr=m_bathNode->getChildren();
	CCObject*pObj=NULL;
	CCSize size=CCDirector::sharedDirector()->getVisibleSize();

	CCArray*clearArr=CCArray::create();

	int num=0;
	bool islast=false;

	CCARRAY_FOREACH(bubbleArr,pObj)
	{
		GuideBubble*pBubble=(GuideBubble*)pObj;
		float length=calculateDistanceByAngle(preAngle,pBubble);
		CCPoint rootPos=m_bathNode->getPosition();
		CCPoint temp=ccp(rootPos.x+length*sin(rotation),rootPos.y+length*cos(rotation));

		this->convertWorldPosToNode(pBubble,temp);
		temp=m_bathNode->convertToNodeSpace(temp);
		pBubble->setPosition(temp);

		islast=isBubbleOutLine(pBubble);
		if(islast)
			clearArr->addObject(pObj);
		num++;
	}

	this->clearBubbleFromGuideLine(clearArr);

	float disX=sin(rotation)*BUBBLE_MOVE_DIS;
	float disY=cos(rotation)*BUBBLE_MOVE_DIS;
	while(!islast)
	{
		GuideBubble*pTemp=createBubbleOnGuideLine(num,rotation);
		this->adjustBubblePosByAddDis(pTemp,ccp(m_time*disX,m_time*disY));
		islast=isBubbleOutLine(pTemp);
		num++;
	}
}

float GuideLine::calculateDistanceByAngle(float rootAngle,GuideBubble*pBubble)
{
	CCPoint rootPoint=m_bathNode->getPosition();
	CCPoint curPoint=m_bathNode->convertToWorldSpace(pBubble->getPosition());

	float curAngle=calculateAngle(rootPoint,curPoint);
	while(abs(curAngle-rootAngle)>0.1f)
	{
		if(pBubble->getBubbleDirection()==1)
		{
			if(rootAngle>0)
				curPoint.x=2*m_leftBoundary-curPoint.x;
			else
				curPoint.x=2*m_rightBoundary-curPoint.x;
			pBubble->setBubbleDirection(-pBubble->getBubbleDirection());
		}
		if (pBubble->getBubbleDirection()==-1)
		{
			if(rootAngle>0)
				curPoint.x=2*m_rightBoundary-curPoint.x;
			else
				curPoint.x=2*m_leftBoundary-curPoint.x;
			pBubble->setBubbleDirection(-pBubble->getBubbleDirection());
		}
		curAngle=calculateAngle(rootPoint,curPoint);
	}
	float dis=ccpDistance(rootPoint,curPoint);
	return dis;
}

GuideBubble* GuideLine::createBubbleOnGuideLine(int num,float rotation)
{
	float disX=(num+3)*BUBBLE_DISTANCE*sin(rotation);
	float disY=(num+3)*BUBBLE_DISTANCE*cos(rotation);

	CCPoint rootPoint=m_bathNode->getPosition();
	CCPoint temp=ccpAdd(rootPoint,ccp(disX,disY));

	//GuideBubble*pBubble=GuideBubble::createWithTexture(m_bathNode->getTexture());
	GuideBubble*pBubble=new GuideBubble();
	pBubble->initWithSpriteFrameName(g_smallBubbleName[pszSpriteFrameNum].c_str());

	this->convertWorldPosToNode(pBubble,temp);
	temp=m_bathNode->convertToNodeSpace(temp);
	pBubble->setPosition(temp);
	float scaleNum=(rand()%5+6)/10.0f;
	pBubble->setScale(scaleNum);

	m_bathNode->addChild(pBubble,0,num);

	return pBubble;
}

void GuideLine::convertWorldPosToNode(GuideBubble*pBubble,cocos2d::CCPoint &curPos)
{
	CCSize size=CCDirector::sharedDirector()->getVisibleSize();
	pBubble->setBubbleDirection(1);
	while(curPos.x<m_leftBoundary||curPos.x>m_rightBoundary)
	{
		if(curPos.x<m_leftBoundary)
		{
			curPos.x=2*m_leftBoundary-curPos.x;
			pBubble->setBubbleDirection(-pBubble->getBubbleDirection());
		}
		if (curPos.x>m_rightBoundary)
		{
			curPos.x=2*m_rightBoundary-curPos.x;
			pBubble->setBubbleDirection(-pBubble->getBubbleDirection());
		}
	}
}

void GuideLine::clearBubbleFromGuideLine(CCArray*clearArr)
{
	CCObject*obj;
	CCARRAY_FOREACH(clearArr,obj)
	{
		CCNode*pNode=(CCNode*)obj;
		if(m_lastBubble&&m_lastBubble==obj)
			m_lastBubble=NULL;
		m_bathNode->removeChild(pNode,true);
	}
}

bool GuideLine::isBubbleOutLine(GuideBubble*pBubble)
{
	CCPoint tempPoint=m_bathNode->convertToWorldSpace(pBubble->getPosition());
	if(tempPoint.y>GameScene::getBoardStartPos().y||tempPoint.y<m_downBoundary)
		return true;
	if(m_lastBubble)
	{
		CCPoint temp=m_bathNode->convertToWorldSpace(m_lastBubble->getPosition());
		if(tempPoint.y>temp.y)
			return true;
	}
	RowCol tempRC=GetRowColByPos(tempPoint.x,tempPoint.y);
	if(IsValidPos(tempRC))
	{
		std::list<RowCol>listTemp=GetAroundList(tempRC.m_nRow,tempRC.m_nCol);
		std::list<RowCol>::iterator ite=listTemp.begin();
		for (ite;ite!=listTemp.end();ite++)
		{
			RowCol temp=*ite;
			Bubble*pB=(*m_pBoard)[temp.m_nRow][temp.m_nCol];
			if(pB)
			{
				CCPoint pBubblePoint=pB->getPosition();
				bool pBool = pow(pBubblePoint.x - tempPoint.x, 2) + pow(pBubblePoint.y - tempPoint.y, 2) < pow(BUBBLE_COLLISION_RADIUS + BUBBLE_COLLISION_RADIUS, 2);
				if(pBool)
				{
					m_lastBubble=pBubble;
					return pBool;
				}
			}
		}
	}
	return false;
}

void GuideLine::updateGuideLine(float dt)
{
	CCArray*bubbleArr=m_bathNode->getChildren();
	CCObject*pObj=NULL;

	CCSize size=CCDirector::sharedDirector()->getVisibleSize();

	CCArray*clearArr=CCArray::create();

	float angle=m_targetRoot->getRotation();
	angle=angle*PI/180;
	float disX=sin(angle)*BUBBLE_MOVE_DIS;
	float disY=cos(angle)*BUBBLE_MOVE_DIS;

	int steps=BUBBLE_DISTANCE/BUBBLE_MOVE_DIS;

	CCARRAY_FOREACH(bubbleArr,pObj)
	{
		GuideBubble*pBubble=(GuideBubble*)pObj;
		this->adjustBubblePosByAddDis(pBubble,ccp(disX,disY));
		if(/*m_time%(steps-1)==0&&*/isBubbleOutLine(pBubble))
			clearArr->addObject(pObj);
	}

	m_time++;
	if(m_time%steps==0)
	{
		this->createBubbleOnGuideLine(0,angle);
		m_time=0;
	}
	this->clearBubbleFromGuideLine(clearArr);
}

void GuideLine::adjustBubblePosByAddDis(GuideBubble*pBubble,CCPoint addPoint)
{
	CCPoint temp=m_bathNode->convertToWorldSpace(pBubble->getPosition());
	temp.x+=pBubble->getBubbleDirection()*addPoint.x;temp.y+=addPoint.y;
	if(temp.x<m_leftBoundary)
	{
		temp.x=2*m_leftBoundary-temp.x;
		pBubble->setBubbleDirection(-pBubble->getBubbleDirection());
	}
	if (temp.x>m_rightBoundary)
	{
		temp.x=2*m_rightBoundary-temp.x;
		pBubble->setBubbleDirection(-pBubble->getBubbleDirection());
	}
	temp=m_bathNode->convertToNodeSpace(temp);
	pBubble->setPosition(temp);
}

float GuideLine::getInitialAngle()
{
	return m_initAngle;
}

void GuideLine::setInitialAngle(float angle)
{
	m_initAngle=angle;
}

void GuideLine::clearBathNodeChildren()
{
	if(m_bathNode)
		m_bathNode->removeAllChildrenWithCleanup(true);
}

void GuideLine::changeBathNodeColor(int num)
{
	CCArray*bubbleArr=m_bathNode->getChildren();
	CCObject*pObj=NULL;

	CCARRAY_FOREACH(bubbleArr,pObj)
	{
		GuideBubble*pBubble=(GuideBubble*)pObj;
		CCSpriteFrame*frameTemp=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(g_smallBubbleName[pszSpriteFrameNum].c_str());
		pBubble->setDisplayFrame(frameTemp);
	}
}
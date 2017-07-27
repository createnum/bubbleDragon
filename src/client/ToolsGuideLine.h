#ifndef __TOOLS_GUIDE_LINE_H__
#define __TOOLS_GUIDE_LINE_H__

#include "cocos2d.h"

class GuideBubble:public cocos2d::CCSprite
{
public:
	GuideBubble();
	int getBubbleDirection();
	void setBubbleDirection(int dir);
	static GuideBubble* createWithTexture(cocos2d::CCTexture2D *pTexture);
	//virtual bool initWithSpriteFrameName(const char *pszSpriteFrameName);
private:
	int m_direction;
};

class Bubble;
class GuideLine : public cocos2d::CCNode
{
public:
	GuideLine();
	~GuideLine();
	virtual bool init(cocos2d::CCNode*pTarget,char*fileImage);
	static GuideLine*create(cocos2d::CCNode*pTarget,char*fileImage);

public:
	virtual void setTargetNode(cocos2d::CCNode*pTarget);
	// set the boundary of guide line 
	virtual void setBoundaryLine(float minX,float maxX,float minY,float maxY);
	// calculate the angle between two point A and B
	virtual float calculateAngle(cocos2d::CCPoint rootPoint,cocos2d::CCPoint curPoint);
	// display the guide line,you can decide that the guide line is dynamic or not
	virtual void displayGuideLine(cocos2d::CCPoint touchPos,bool isDynamicOrNot);
	//you can use this function when touch end to decided whether the guide line continue to display
	virtual void unDisplayGuideLine();
	
	virtual void setGuideLineVisible(bool visible);
	
	//you can use this function when touch moved to rotate the guide line and the target
	virtual void rotateGuideLine(cocos2d::CCPoint touchPos);
	//update the guide line,the default update time is 1.0/60.0f
	virtual void updateGuideLine(float dt);
	//you can overwrite this function to decide when to remove the child bubble
	virtual bool isBubbleOutLine(GuideBubble*pBubble);
	//the function is used in converted world position to node position,just like the function name
	virtual void convertWorldPosToNode(GuideBubble*pBubble,cocos2d::CCPoint &curPos);
	//adjust the bubble position by add a distance
	virtual void adjustBubblePosByAddDis(GuideBubble*pBubble,cocos2d::CCPoint addPoint);

	virtual GuideBubble* createBubbleOnGuideLine(int num,float rotation);
	virtual void clearBubbleFromGuideLine(cocos2d::CCArray*arr);
	virtual void clearBathNodeChildren();
	virtual void changeBathNodeColor(int num=0);
	virtual float calculateDistanceByAngle(float rootAngle,GuideBubble*pBubble);

	virtual float getInitialAngle();
	virtual void setInitialAngle(float angle);

	virtual void setBubbleSpriteFrameName(int num);

	//RowCol GetRowColByPos( float nPosX, float nPosY);
public:
	std::vector<std::vector<Bubble*> > *m_pBoard;
private:
	/**
	 * The boundary line of four direction
	 */
	float m_leftBoundary;
	float m_rightBoundary;
	float m_upBoundary;
	float m_downBoundary;

	int m_time;
	float m_initAngle;
	int pszSpriteFrameNum;
	GuideBubble*m_lastBubble;
	cocos2d::CCNode*m_targetRoot;
	cocos2d::CCSpriteBatchNode*m_bathNode;
};

#endif // __TOOLS_GUIDE_LINE_H__

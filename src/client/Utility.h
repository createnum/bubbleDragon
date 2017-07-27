#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <cmath>
#include <vector>
#include <list>
#include <algorithm>

#include "cocos2d.h"
#include "BubbleConstant.h"
#include "GameScene.h"
#include "myGame.h"

using namespace cocos2d;
using namespace std;

//inline CCPoint getPosByRowAndCol(int row, int col);
//inline RowCol GetRowColByPos( int nPosX, int nPosY);
//
//inline bool IsValidPos(int nRow, int nCol);//判断该点是否是有效位置
//inline bool IsValidPos(RowCol nRownCol);//判断该点是否是有效位置
//inline void GetAround( int nRow, int nCol, std::vector<RowCol> & vecRowCol );
//
////得到整条斜线上球的集合，分为左右斜线之分，左斜线代表从左上角到右下角， 右斜线代表从右上角到左下角,由mode值来区分
//inline void GetGoldenLine (int nRow, int nCol, ROWCOL_LIST& listRowCol);
//inline void GetSliveryLine(int nRow, int nCol, ROWCOL_LIST& listRowCol);
//inline void GetLightningLine(int nRow, int nCol, ROWCOL_LIST& listRowCol);
//inline void GetDyeArea(int nRow, int nCol, ROWCOL_LIST& listRowCol);
//inline void GetColorLine(int nRow, int nCol, ROWCOL_LIST& listRowCol, Bubble*** m_board);//获取某一点左上或右上的泡泡
//inline void GetBombLine(int nRow, int nCol, ROWCOL_LIST& listRowCol, Bubble*** m_board);

//泡泡初始坐标 即第一行第一列的坐标 从上往下数

//一个位置是否有效的停靠点

inline bool IsValidPos( int nRow, int nCol )
{
	if (nRow < 0 || nCol < 0)
	{
		return false;
	}
	if (nRow >= MyGame::getInstance()->max_rows || nCol >= MAX_COLS - nRow % 2)
	{
		return false;
	}

	return true;
}

inline bool IsValidPos(RowCol nRownCol)
{
	return IsValidPos(nRownCol.m_nRow, nRownCol.m_nCol);
}

inline CCPoint getBubbleStartPos()
{
	return GameScene::getBoardStartPos();
}

//坐标转换 根据行列转化为泡泡的屏幕坐标
inline CCPoint getPosByRowAndCol(int row, int col)
{
	float posX, posY;

	CCSize size = CCDirector::sharedDirector()->getVisibleSize();

	posX = getBubbleStartPos().x + col * 2 * BUBBLE_RADIUS + BUBBLE_RADIUS + (row % 2) * BUBBLE_RADIUS;//奇偶行加以区分 偶数行错开半个
	posY = getBubbleStartPos().y - ( row * 2 * BUBBLE_RADIUS * sin(PI/3) + BUBBLE_RADIUS );//等边三角形 sin 60

	return ccp(posX, posY);
}

//坐标转换 根据屏幕坐标转化为行列
inline RowCol GetRowColByPos( float nPosX, float nPosY)
{
	int nRow, nCol;
	//需要四舍五入
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	nPosY = getBubbleStartPos().y - nPosY;

	nRow = ( nPosY -BUBBLE_RADIUS )/( 2 *BUBBLE_RADIUS *sin ( PI/3 ) ) +0.5;

	nCol = ( nPosX - ( nRow % 2 ) * BUBBLE_RADIUS - BUBBLE_RADIUS - getBubbleStartPos().x) * 1.0 /(  2 *BUBBLE_RADIUS ) + 0.5;

	return RowCol( nRow, nCol );
}

//@获得周围停靠位置的列表nRow,nCol为要计算的停靠位置，vecPos返回它周围的位置
inline void GetAround( int nRow, int nCol, list<RowCol> & vecPos )
{
	if (!IsValidPos(nRow, nCol))
	{
		return;
	}

	//同一行
	if (IsValidPos(nRow, nCol - 1))
	{
		list<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow, nCol - 1)); 
		if(result==vecPos.end())
			vecPos.push_back(RowCol(nRow, nCol - 1));
	}
	if (IsValidPos(nRow, nCol + 1))
	{
		list<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow, nCol + 1)); 
		if(result==vecPos.end())
			vecPos.push_back(RowCol(nRow, nCol + 1));
	}

	if (nRow % 2 == 0) //双行
	{
		if (IsValidPos(nRow - 1, nCol-1))//左上角
		{
			list<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow - 1, nCol-1)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow - 1, nCol-1));
		}
		if (IsValidPos(nRow + 1, nCol))//右下角
		{
			list<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow + 1, nCol)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow + 1, nCol));
		}
		if (IsValidPos(nRow + 1, nCol-1))//左下角
		{
			list<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow + 1, nCol-1)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow + 1, nCol-1));
		}
		if (IsValidPos(nRow - 1, nCol))//右上角
		{
			list<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow - 1, nCol)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow - 1, nCol));
		}
	}
	else
	{
		if (IsValidPos(nRow - 1, nCol))//左上角
		{
			list<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow - 1, nCol)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow - 1, nCol));
		}
		if (IsValidPos(nRow + 1, nCol+1))//右下角
		{
			list<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow + 1, nCol+1)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow + 1, nCol+1));
		}
		if (IsValidPos(nRow + 1, nCol))//左下角
		{
			list<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow + 1, nCol)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow + 1, nCol));
		}
		if (IsValidPos(nRow - 1, nCol+1))//右上角
		{
			list<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow - 1, nCol+1)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow - 1, nCol+1));
		}
	}
}

//@获得周围停靠位置的列表nRow,nCol为要计算的停靠位置，vecPos返回它周围的位置
inline void GetAround( int nRow, int nCol, vector<RowCol> & vecPos )
{
	if (!IsValidPos(nRow, nCol))
	{
		return;
	}

	//同一行
	if (IsValidPos(nRow, nCol - 1))
	{
		vector<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow, nCol - 1)); 
		if(result==vecPos.end())
			vecPos.push_back(RowCol(nRow, nCol - 1));
	}
	if (IsValidPos(nRow, nCol + 1))
	{
		vector<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow, nCol + 1)); 
		if(result==vecPos.end())
			vecPos.push_back(RowCol(nRow, nCol + 1));
	}

	if (nRow % 2 == 0) //双行
	{
		if (IsValidPos(nRow - 1, nCol-1))//左上角
		{
			vector<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow - 1, nCol-1)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow - 1, nCol-1));
		}
		if (IsValidPos(nRow + 1, nCol))//右下角
		{
			vector<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow + 1, nCol)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow + 1, nCol));
		}
		if (IsValidPos(nRow + 1, nCol-1))//左下角
		{
			vector<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow + 1, nCol-1)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow + 1, nCol-1));
		}
		if (IsValidPos(nRow - 1, nCol))//右上角
		{
			vector<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow - 1, nCol)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow - 1, nCol));
		}
	}
	else
	{
		if (IsValidPos(nRow - 1, nCol))//左上角
		{
			vector<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow - 1, nCol)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow - 1, nCol));
		}
		if (IsValidPos(nRow + 1, nCol+1))//右下角
		{
			vector<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow + 1, nCol+1)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow + 1, nCol+1));
		}
		if (IsValidPos(nRow + 1, nCol))//左下角
		{
			vector<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow + 1, nCol)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow + 1, nCol));
		}
		if (IsValidPos(nRow - 1, nCol+1))//右上角
		{
			vector<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow - 1, nCol+1)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow - 1, nCol+1));
		}
	}
}

//@获得周围停靠位置的列表nRow,nCol为要计算的停靠位置，vecPos返回它周围的位置
inline std::list<RowCol> GetAroundList( int nRow, int nCol)
{
	std::list<RowCol> tempList;
	if (!IsValidPos(nRow, nCol))
	{
		return tempList;
	}

	//同一行
	if (IsValidPos(nRow, nCol - 1))
	{
		list<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow, nCol - 1)); 
		if(result==tempList.end())
			tempList.push_back(RowCol(nRow, nCol - 1));
	}
	if (IsValidPos(nRow, nCol + 1))
	{
		list<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow, nCol + 1)); 
		if(result==tempList.end())
			tempList.push_back(RowCol(nRow, nCol + 1));
	}

	if (nRow % 2 == 0) //双行
	{
		if (IsValidPos(nRow - 1, nCol-1))//左上角
		{
			list<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow - 1, nCol-1)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow - 1, nCol-1));
		}
		if (IsValidPos(nRow + 1, nCol))//右下角
		{
			list<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow + 1, nCol)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow + 1, nCol));
		}
		if (IsValidPos(nRow + 1, nCol-1))//左下角
		{
			list<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow + 1, nCol-1)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow + 1, nCol-1));
		}
		if (IsValidPos(nRow - 1, nCol))//右上角
		{
			list<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow - 1, nCol)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow - 1, nCol));
		}
	}
	else
	{
		if (IsValidPos(nRow - 1, nCol))//左上角
		{
			list<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow - 1, nCol)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow - 1, nCol));
		}
		if (IsValidPos(nRow + 1, nCol+1))//右下角
		{
			list<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow + 1, nCol+1)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow + 1, nCol+1));
		}
		if (IsValidPos(nRow + 1, nCol))//左下角
		{
			list<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow + 1, nCol)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow + 1, nCol));
		}
		if (IsValidPos(nRow - 1, nCol+1))//右上角
		{
			list<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow - 1, nCol+1)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow - 1, nCol+1));
		}
	}
	return tempList;
}

//@获得周围停靠位置的列表nRow,nCol为要计算的停靠位置，vecPos返回它周围的位置
inline std::vector<RowCol> GetAroundVector( int nRow, int nCol)
{
	std::vector<RowCol> tempList;
	if (!IsValidPos(nRow, nCol))
	{
		return tempList;
	}

	//同一行
	if (IsValidPos(nRow, nCol - 1))
	{
		vector<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow, nCol - 1)); 
		if(result==tempList.end())
			tempList.push_back(RowCol(nRow, nCol - 1));
	}
	if (IsValidPos(nRow, nCol + 1))
	{
		vector<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow, nCol + 1)); 
		if(result==tempList.end())
			tempList.push_back(RowCol(nRow, nCol + 1));
	}

	if (nRow % 2 == 0) //双行
	{
		if (IsValidPos(nRow - 1, nCol-1))//左上角
		{
			vector<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow - 1, nCol-1)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow - 1, nCol-1));
		}
		if (IsValidPos(nRow + 1, nCol))//右下角
		{
			vector<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow + 1, nCol)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow + 1, nCol));
		}
		if (IsValidPos(nRow + 1, nCol-1))//左下角
		{
			vector<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow + 1, nCol-1)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow + 1, nCol-1));
		}
		if (IsValidPos(nRow - 1, nCol))//右上角
		{
			vector<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow - 1, nCol)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow - 1, nCol));
		}
	}
	else
	{
		if (IsValidPos(nRow - 1, nCol))//左上角
		{
			vector<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow - 1, nCol)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow - 1, nCol));
		}
		if (IsValidPos(nRow + 1, nCol+1))//右下角
		{
			vector<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow + 1, nCol+1)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow + 1, nCol+1));
		}
		if (IsValidPos(nRow + 1, nCol))//左下角
		{
			vector<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow + 1, nCol)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow + 1, nCol));
		}
		if (IsValidPos(nRow - 1, nCol+1))//右上角
		{
			vector<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow - 1, nCol+1)); 
			if(result!=tempList.end())
				tempList.push_back(RowCol(nRow - 1, nCol+1));
		}
	}
	return tempList;
}

inline void GetGoldenLine(int nRow, int nCol, ROWCOL_LIST& listRowCol)
{		
	if (!IsValidPos(nRow, nCol))
		return;

	RowCol nextRowCol(nRow,nCol);
	bool nextFlag = false;
	
	listRowCol.push_back(nextRowCol);
	
	//左上角
	do 
	{
		if (nextRowCol.m_nRow % 2 == 0)
		{
			nextRowCol.m_nRow -= 1;
			nextRowCol.m_nCol -= 1;
		}
		else
		{
			nextRowCol.m_nRow -= 1;
		}

		if (IsValidPos(nextRowCol))
		{
			listRowCol.push_back(nextRowCol);
			nextFlag = true;
		}
		else
		{
			nextFlag = false;
		}
	} while (nextFlag);

// 	右下角
	nextRowCol.m_nRow = nRow;
	nextRowCol.m_nCol = nCol;
	do 
	{
		if (nextRowCol.m_nRow % 2 == 0)
		{
			nextRowCol.m_nRow += 1;
		}
		else
		{
			nextRowCol.m_nRow += 1;
			nextRowCol.m_nCol += 1;
		}
	
		if (IsValidPos(nextRowCol))
		{
			listRowCol.push_back(nextRowCol);
			nextFlag = true;
		}
		else
		{
			nextFlag = false;
		}
	} while (nextFlag);

	//同一行
	nextRowCol.m_nRow = nRow;
	nextRowCol.m_nCol = -1;
	do 
	{
		nextRowCol.m_nCol += 1;
	
		if (IsValidPos(nextRowCol))
		{
			listRowCol.push_back(nextRowCol);
			nextFlag = true;
		}
		else
		{
			nextFlag = false;
		}
	} while (nextFlag);
// 	
	//右上角
	nextRowCol.m_nRow = nRow;
	nextRowCol.m_nCol = nCol;
	do 
	{
		if (nextRowCol.m_nRow % 2 == 0)
		{
			nextRowCol.m_nRow -= 1;
		}
		else
		{
			nextRowCol.m_nRow -= 1;
			nextRowCol.m_nCol += 1;
		}
	
		if (IsValidPos(nextRowCol))
		{
			listRowCol.push_back(nextRowCol);
			nextFlag = true;
		}
		else
		{
			nextFlag = false;
		}
	} while (nextFlag);
	
	//左下角
	nextRowCol.m_nRow = nRow;
	nextRowCol.m_nCol = nCol;
	do 
	{
		if (nextRowCol.m_nRow % 2 == 0)
		{
			nextRowCol.m_nRow += 1;
			nextRowCol.m_nCol -= 1;
		}
		else
		{
			nextRowCol.m_nRow += 1;
		}
	
		if (IsValidPos(nextRowCol))
		{
			listRowCol.push_back(nextRowCol);
			nextFlag = true;
		}
		else
		{
			nextFlag = false;
		}
	} while (nextFlag);

}

inline void GetSliveryLine(int nRow, int nCol, ROWCOL_LIST& listRowCol)
{
	if (!IsValidPos(nRow, nCol))
		return;

	RowCol nextRowCol(nRow,nCol);
	bool nextFlag = false;

	listRowCol.push_back(nextRowCol);

	//左上角
	do 
	{
		if (nextRowCol.m_nRow % 2 == 0)
		{
			nextRowCol.m_nRow -= 1;
			nextRowCol.m_nCol -= 1;
		}
		else
		{
			nextRowCol.m_nRow -= 1;
		}

		if (IsValidPos(nextRowCol))
		{
			listRowCol.push_back(nextRowCol);
			nextFlag = true;
		}
		else
		{
			nextFlag = false;
		}
	} while (nextFlag);

	//同一行
	nextRowCol.m_nRow = nRow;
	nextRowCol.m_nCol = -1;
	do 
	{
		nextRowCol.m_nCol += 1;

		if (IsValidPos(nextRowCol))
		{
			listRowCol.push_back(nextRowCol);
			nextFlag = true;
		}
		else
		{
			nextFlag = false;
		}
	} while (nextFlag);

	//右上角
	nextRowCol.m_nRow = nRow;
	nextRowCol.m_nCol = nCol;
	do 
	{
		if (nextRowCol.m_nRow % 2 == 0)
		{
			nextRowCol.m_nRow -= 1;
		}
		else
		{
			nextRowCol.m_nRow -= 1;
			nextRowCol.m_nCol += 1;
		}

		if (IsValidPos(nextRowCol))
		{
			listRowCol.push_back(nextRowCol);
			nextFlag = true;
		}
		else
		{
			nextFlag = false;
		}
	} while (nextFlag);
}


inline void GetLightningLine(int nRow, int nCol, ROWCOL_LIST& listRowCol)
{
    if (!IsValidPos(nRow, nCol))
        return;

    RowCol nextRowCol(nRow,nCol);
    bool nextFlag = false;

    listRowCol.push_back(nextRowCol);

    //同一行
    nextRowCol.m_nRow = nRow;
    nextRowCol.m_nCol = -1;
    do 
    {
        nextRowCol.m_nCol += 1;

        if (IsValidPos(nextRowCol))
        {
            listRowCol.push_back(nextRowCol);
            nextFlag = true;
        }
        else
        {
            nextFlag = false;
        }
    } while (nextFlag);
}

//获取某一点的相连泡泡
inline void GetColorLine(int nRow, int nCol, ROWCOL_LIST& listRowCol, Bubble***m_board)
{
	if (!IsValidPos(nRow, nCol))
		return;
	std::list<RowCol> tempList=GetAroundList(nRow,nCol);
	std::list<RowCol>::iterator ite=tempList.begin();
	while(ite!=tempList.end())
	{
		RowCol tep=*ite;
		GetAround(tep.m_nRow,tep.m_nCol,listRowCol);
		ite++;
	}
}

inline void GetBombLine(int nRow, int nCol, ROWCOL_LIST& listRowCol)
{
    if (!IsValidPos(nRow, nCol))
        return;

	std::list<RowCol> tempList=GetAroundList(nRow,nCol);
	std::list<RowCol>::iterator ite=tempList.begin();
	while(ite!=tempList.end())
	{
		RowCol tep=*ite;
		GetAround(tep.m_nRow,tep.m_nCol,listRowCol);
		ite++;
	}
}


inline void GetDyeArea(int nRow, int nCol, ROWCOL_LIST& listRowCol)
{
    if (!IsValidPos(nRow, nCol))
        return;

    RowCol nextRowCol(nRow,nCol);
    bool nextFlag = false;

    listRowCol.push_back(nextRowCol);

    //左上角
    do 
    {
        if (nextRowCol.m_nRow % 2 == 0)
        {
            nextRowCol.m_nRow -= 1;
            nextRowCol.m_nCol -= 1;
        }
        else
        {
            nextRowCol.m_nRow -= 1;
        }

        if (IsValidPos(nextRowCol))
        {
            listRowCol.push_back(nextRowCol);
            nextFlag = false;
        }
        else
        {
            nextFlag = false;
        }
    } while (nextFlag);

    //同行左边
    nextRowCol.m_nRow = nRow;
    nextRowCol.m_nCol = nCol-1;

    if (IsValidPos(nextRowCol))
    {
        listRowCol.push_back(nextRowCol);
    }

    //左上角
    do 
    {
        if (nextRowCol.m_nRow % 2 == 0)
        {
            nextRowCol.m_nRow -= 1;
            nextRowCol.m_nCol -= 1;
        }
        else
        {
            nextRowCol.m_nRow -= 1;
        }

        if (IsValidPos(nextRowCol))
        {
            listRowCol.push_back(nextRowCol);
            nextFlag = false;
        }
        else
        {
            nextFlag = false;
        }
    } while (nextFlag);

    //同行左边
    nextRowCol.m_nRow = nRow;
    nextRowCol.m_nCol = nCol-1;

    if (IsValidPos(nextRowCol))
    {
        listRowCol.push_back(nextRowCol);
    }

    //左下角
    nextRowCol.m_nRow = nRow;
    nextRowCol.m_nCol = nCol;
    do 
    {
        if (nextRowCol.m_nRow % 2 == 0)
        {
            nextRowCol.m_nRow += 1;
            nextRowCol.m_nCol -= 1;
        }
        else
        {
            nextRowCol.m_nRow += 1;
        }

        if (IsValidPos(nextRowCol))
        {
            listRowCol.push_back(nextRowCol);
            nextFlag = true;
        }
        else
        {
            nextFlag = false;
        }
    
    } while (nextFlag);

    //右上角
    nextRowCol.m_nRow = nRow;
    nextRowCol.m_nCol = nCol;
    do 
    {
        if (nextRowCol.m_nRow % 2 == 0)
        {
            nextRowCol.m_nRow -= 1;
        }
        else
        {
            nextRowCol.m_nRow -= 1;
            nextRowCol.m_nCol += 1;
        }

        if (IsValidPos(nextRowCol))
        {
            listRowCol.push_back(nextRowCol);
            nextFlag = false;
        }
        else
        {
            nextFlag = false;
        }
    } while (nextFlag);

    //同行右边
    nextRowCol.m_nRow = nRow;
    nextRowCol.m_nCol = nCol+1;

    if (IsValidPos(nextRowCol))
    {
        listRowCol.push_back(nextRowCol);
    }

    // 	右下角
    nextRowCol.m_nRow = nRow;
    nextRowCol.m_nCol = nCol;
    do 
    {
        if (nextRowCol.m_nRow % 2 == 0)
        {
            nextRowCol.m_nRow += 1;
        }
        else
        {
            nextRowCol.m_nRow += 1;
            nextRowCol.m_nCol += 1;
        }

        if (IsValidPos(nextRowCol))
        {
            listRowCol.push_back(nextRowCol);
            nextFlag = true;
        }
        else
        {
            nextFlag = false;
        }
    } while (nextFlag);

}

#endif	//_UTILITY_H_
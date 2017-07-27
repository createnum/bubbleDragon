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
//inline bool IsValidPos(int nRow, int nCol);//�жϸõ��Ƿ�����Чλ��
//inline bool IsValidPos(RowCol nRownCol);//�жϸõ��Ƿ�����Чλ��
//inline void GetAround( int nRow, int nCol, std::vector<RowCol> & vecRowCol );
//
////�õ�����б������ļ��ϣ���Ϊ����б��֮�֣���б�ߴ�������Ͻǵ����½ǣ� ��б�ߴ�������Ͻǵ����½�,��modeֵ������
//inline void GetGoldenLine (int nRow, int nCol, ROWCOL_LIST& listRowCol);
//inline void GetSliveryLine(int nRow, int nCol, ROWCOL_LIST& listRowCol);
//inline void GetLightningLine(int nRow, int nCol, ROWCOL_LIST& listRowCol);
//inline void GetDyeArea(int nRow, int nCol, ROWCOL_LIST& listRowCol);
//inline void GetColorLine(int nRow, int nCol, ROWCOL_LIST& listRowCol, Bubble*** m_board);//��ȡĳһ�����ϻ����ϵ�����
//inline void GetBombLine(int nRow, int nCol, ROWCOL_LIST& listRowCol, Bubble*** m_board);

//���ݳ�ʼ���� ����һ�е�һ�е����� ����������

//һ��λ���Ƿ���Ч��ͣ����

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

//����ת�� ��������ת��Ϊ���ݵ���Ļ����
inline CCPoint getPosByRowAndCol(int row, int col)
{
	float posX, posY;

	CCSize size = CCDirector::sharedDirector()->getVisibleSize();

	posX = getBubbleStartPos().x + col * 2 * BUBBLE_RADIUS + BUBBLE_RADIUS + (row % 2) * BUBBLE_RADIUS;//��ż�м������� ż���д����
	posY = getBubbleStartPos().y - ( row * 2 * BUBBLE_RADIUS * sin(PI/3) + BUBBLE_RADIUS );//�ȱ������� sin 60

	return ccp(posX, posY);
}

//����ת�� ������Ļ����ת��Ϊ����
inline RowCol GetRowColByPos( float nPosX, float nPosY)
{
	int nRow, nCol;
	//��Ҫ��������
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	nPosY = getBubbleStartPos().y - nPosY;

	nRow = ( nPosY -BUBBLE_RADIUS )/( 2 *BUBBLE_RADIUS *sin ( PI/3 ) ) +0.5;

	nCol = ( nPosX - ( nRow % 2 ) * BUBBLE_RADIUS - BUBBLE_RADIUS - getBubbleStartPos().x) * 1.0 /(  2 *BUBBLE_RADIUS ) + 0.5;

	return RowCol( nRow, nCol );
}

//@�����Χͣ��λ�õ��б�nRow,nColΪҪ�����ͣ��λ�ã�vecPos��������Χ��λ��
inline void GetAround( int nRow, int nCol, list<RowCol> & vecPos )
{
	if (!IsValidPos(nRow, nCol))
	{
		return;
	}

	//ͬһ��
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

	if (nRow % 2 == 0) //˫��
	{
		if (IsValidPos(nRow - 1, nCol-1))//���Ͻ�
		{
			list<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow - 1, nCol-1)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow - 1, nCol-1));
		}
		if (IsValidPos(nRow + 1, nCol))//���½�
		{
			list<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow + 1, nCol)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow + 1, nCol));
		}
		if (IsValidPos(nRow + 1, nCol-1))//���½�
		{
			list<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow + 1, nCol-1)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow + 1, nCol-1));
		}
		if (IsValidPos(nRow - 1, nCol))//���Ͻ�
		{
			list<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow - 1, nCol)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow - 1, nCol));
		}
	}
	else
	{
		if (IsValidPos(nRow - 1, nCol))//���Ͻ�
		{
			list<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow - 1, nCol)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow - 1, nCol));
		}
		if (IsValidPos(nRow + 1, nCol+1))//���½�
		{
			list<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow + 1, nCol+1)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow + 1, nCol+1));
		}
		if (IsValidPos(nRow + 1, nCol))//���½�
		{
			list<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow + 1, nCol)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow + 1, nCol));
		}
		if (IsValidPos(nRow - 1, nCol+1))//���Ͻ�
		{
			list<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow - 1, nCol+1)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow - 1, nCol+1));
		}
	}
}

//@�����Χͣ��λ�õ��б�nRow,nColΪҪ�����ͣ��λ�ã�vecPos��������Χ��λ��
inline void GetAround( int nRow, int nCol, vector<RowCol> & vecPos )
{
	if (!IsValidPos(nRow, nCol))
	{
		return;
	}

	//ͬһ��
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

	if (nRow % 2 == 0) //˫��
	{
		if (IsValidPos(nRow - 1, nCol-1))//���Ͻ�
		{
			vector<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow - 1, nCol-1)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow - 1, nCol-1));
		}
		if (IsValidPos(nRow + 1, nCol))//���½�
		{
			vector<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow + 1, nCol)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow + 1, nCol));
		}
		if (IsValidPos(nRow + 1, nCol-1))//���½�
		{
			vector<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow + 1, nCol-1)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow + 1, nCol-1));
		}
		if (IsValidPos(nRow - 1, nCol))//���Ͻ�
		{
			vector<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow - 1, nCol)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow - 1, nCol));
		}
	}
	else
	{
		if (IsValidPos(nRow - 1, nCol))//���Ͻ�
		{
			vector<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow - 1, nCol)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow - 1, nCol));
		}
		if (IsValidPos(nRow + 1, nCol+1))//���½�
		{
			vector<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow + 1, nCol+1)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow + 1, nCol+1));
		}
		if (IsValidPos(nRow + 1, nCol))//���½�
		{
			vector<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow + 1, nCol)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow + 1, nCol));
		}
		if (IsValidPos(nRow - 1, nCol+1))//���Ͻ�
		{
			vector<RowCol>::iterator result = find( vecPos.begin(), vecPos.end(), RowCol(nRow - 1, nCol+1)); 
			if(result==vecPos.end())
				vecPos.push_back(RowCol(nRow - 1, nCol+1));
		}
	}
}

//@�����Χͣ��λ�õ��б�nRow,nColΪҪ�����ͣ��λ�ã�vecPos��������Χ��λ��
inline std::list<RowCol> GetAroundList( int nRow, int nCol)
{
	std::list<RowCol> tempList;
	if (!IsValidPos(nRow, nCol))
	{
		return tempList;
	}

	//ͬһ��
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

	if (nRow % 2 == 0) //˫��
	{
		if (IsValidPos(nRow - 1, nCol-1))//���Ͻ�
		{
			list<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow - 1, nCol-1)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow - 1, nCol-1));
		}
		if (IsValidPos(nRow + 1, nCol))//���½�
		{
			list<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow + 1, nCol)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow + 1, nCol));
		}
		if (IsValidPos(nRow + 1, nCol-1))//���½�
		{
			list<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow + 1, nCol-1)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow + 1, nCol-1));
		}
		if (IsValidPos(nRow - 1, nCol))//���Ͻ�
		{
			list<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow - 1, nCol)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow - 1, nCol));
		}
	}
	else
	{
		if (IsValidPos(nRow - 1, nCol))//���Ͻ�
		{
			list<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow - 1, nCol)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow - 1, nCol));
		}
		if (IsValidPos(nRow + 1, nCol+1))//���½�
		{
			list<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow + 1, nCol+1)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow + 1, nCol+1));
		}
		if (IsValidPos(nRow + 1, nCol))//���½�
		{
			list<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow + 1, nCol)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow + 1, nCol));
		}
		if (IsValidPos(nRow - 1, nCol+1))//���Ͻ�
		{
			list<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow - 1, nCol+1)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow - 1, nCol+1));
		}
	}
	return tempList;
}

//@�����Χͣ��λ�õ��б�nRow,nColΪҪ�����ͣ��λ�ã�vecPos��������Χ��λ��
inline std::vector<RowCol> GetAroundVector( int nRow, int nCol)
{
	std::vector<RowCol> tempList;
	if (!IsValidPos(nRow, nCol))
	{
		return tempList;
	}

	//ͬһ��
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

	if (nRow % 2 == 0) //˫��
	{
		if (IsValidPos(nRow - 1, nCol-1))//���Ͻ�
		{
			vector<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow - 1, nCol-1)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow - 1, nCol-1));
		}
		if (IsValidPos(nRow + 1, nCol))//���½�
		{
			vector<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow + 1, nCol)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow + 1, nCol));
		}
		if (IsValidPos(nRow + 1, nCol-1))//���½�
		{
			vector<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow + 1, nCol-1)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow + 1, nCol-1));
		}
		if (IsValidPos(nRow - 1, nCol))//���Ͻ�
		{
			vector<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow - 1, nCol)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow - 1, nCol));
		}
	}
	else
	{
		if (IsValidPos(nRow - 1, nCol))//���Ͻ�
		{
			vector<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow - 1, nCol)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow - 1, nCol));
		}
		if (IsValidPos(nRow + 1, nCol+1))//���½�
		{
			vector<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow + 1, nCol+1)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow + 1, nCol+1));
		}
		if (IsValidPos(nRow + 1, nCol))//���½�
		{
			vector<RowCol>::iterator result = find( tempList.begin(), tempList.end(), RowCol(nRow + 1, nCol)); 
			if(result==tempList.end())
				tempList.push_back(RowCol(nRow + 1, nCol));
		}
		if (IsValidPos(nRow - 1, nCol+1))//���Ͻ�
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
	
	//���Ͻ�
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

// 	���½�
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

	//ͬһ��
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
	//���Ͻ�
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
	
	//���½�
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

	//���Ͻ�
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

	//ͬһ��
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

	//���Ͻ�
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

    //ͬһ��
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

//��ȡĳһ�����������
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

    //���Ͻ�
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

    //ͬ�����
    nextRowCol.m_nRow = nRow;
    nextRowCol.m_nCol = nCol-1;

    if (IsValidPos(nextRowCol))
    {
        listRowCol.push_back(nextRowCol);
    }

    //���Ͻ�
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

    //ͬ�����
    nextRowCol.m_nRow = nRow;
    nextRowCol.m_nCol = nCol-1;

    if (IsValidPos(nextRowCol))
    {
        listRowCol.push_back(nextRowCol);
    }

    //���½�
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

    //���Ͻ�
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

    //ͬ���ұ�
    nextRowCol.m_nRow = nRow;
    nextRowCol.m_nCol = nCol+1;

    if (IsValidPos(nextRowCol))
    {
        listRowCol.push_back(nextRowCol);
    }

    // 	���½�
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
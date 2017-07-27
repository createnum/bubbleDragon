/****************************************************************************
 FileName : storeBuyItenDict.h
 Creator	: chen mengjie
 Date		: 2015.6.4 10:40
 Comment : 游戏中购买项目 
*****************************************************************************/
#ifndef __STORE_BUY_ITEM_DICT_H__
#define __STORE_BUY_ITEM_DICT_H__

#include <map>
#include "cocos2d.h"

struct BuyItem
{
public:
	int id;
	int gold;
	int presentGold;
	int rmb;
	bool isvisible;
};

class StoreBuyItemDict
{
public:
	virtual bool init();
	static StoreBuyItemDict* getInstance();
	virtual void getInfoFromXml(char*pFileName);
    const BuyItem getBuyItemInfos(int stageId);
	std::vector<BuyItem>getBuyItems();
protected:
    static StoreBuyItemDict* m_dict;
    std::vector<BuyItem>m_buyItems;
};
#endif //__STORE_BUY_ITEM_DICT_H__


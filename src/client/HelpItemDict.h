/****************************************************************************
 FileName : helpItemDict.h
 Creator	: chen mengjie
 Date		: 2015.6.17 10:40
 Comment : °ïÖúÏîÄ¿ 
*****************************************************************************/
#ifndef __HELP_ITEM_DICT_H__
#define __HELP_ITEM_DICT_H__

#include <map>
#include <string>
#include "cocos2d.h"

struct HelpItem
{
public:
	int id;
	std::string imagePath;
	std::string declarePath;
	std::string declareContent;
};

class HelpItemDict
{
public:
	virtual bool init();
	static HelpItemDict* getInstance();
	virtual void getInfoFromXml(char*pFileName);
    const HelpItem getHelpItemInfos(int helpId);
	std::vector<HelpItem>getHelpItems();
protected:
    static HelpItemDict* m_helpDic;
    std::vector<HelpItem>m_helpItems;
};
#endif //__HELP_ITEM_DICT_H__


/**************************************************************************
 * FileName	: XMLTools.h
 * Creator		: chen mengjie
 * Date			: 2015.6.4 14:31
 * Comment	: xml���ߣ�����xml���� 
**************************************************************************/  
#ifndef __XML_TOOLS_H__
#define __XML_TOOLS_H__

#include "cocos2d.h"
#include "support/tinyxml2/tinyxml2.h"
using namespace tinyxml2;

struct stageInfo
{
public:
	stageInfo()
	{
		m_stageNum=0;
		m_isSpecialStage=false;
		m_isLocked=true;
		m_firstStar=0;
		m_secondStar=0;
		m_threeStar=0;
		m_maxScore=0;
	};
public:
	int m_stageNum;//�ؿ���
	bool m_isSpecialStage;//�Ƿ�������ؿ�
	bool m_isLocked;//�Ƿ�����
	int m_firstStar;//�ؿ�������
	int m_secondStar;//�ؿ�������
	int m_threeStar;//�ؿ�������
	int m_maxScore;
};

class XMLTools
{
public:
	XMLTools();
	~XMLTools();
public:
	static XMLTools*getInstance();
	virtual void createXML(char*name);
	virtual tinyxml2::XMLElement *createElementWithStageInfo(tinyxml2::XMLDocument *pDoc,stageInfo*info);
	//virtual void modifyXMLWithStageInfos(char*name,std::vector<stageInfo>stageConfig);
	virtual bool isFileExist(const char *path);
	virtual std::vector<stageInfo> readXML(char*name);
	virtual std::vector<stageInfo> readXMLWithFix(char*name);
	virtual void addElementToXmlFile(char*name);
	virtual void modifyXML(char*fileName,stageInfo*info);
	std::vector<stageInfo> getStageInfo();
	virtual void unlockMapStageByIndex(stageInfo*info);
	virtual void unlockMapStageByIndex(int index);
	virtual void updateMapStageInfo(int index,int starNum,int maxScore);
	virtual int getStageCount();
	virtual int getUnlockStageNum();
	virtual stageInfo*getStageInfoByIndex(int index);
	virtual void modifyStageInfoByInfo(stageInfo*info);
private:
	static XMLTools*m_inst;
	int m_unlockStageNum;
	std::vector<stageInfo> m_stageInfoArray;
};
#endif //__EXCHANGE_DICT_H__


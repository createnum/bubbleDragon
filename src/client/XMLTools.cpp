#include "XMLTools.h"
#include <string>
#include "stageMapDict.h"

USING_NS_CC;

#define UI_STAGE_CONFIGER_FILE "stageConfig.xml"

XMLTools*XMLTools::m_inst=NULL;

XMLTools::XMLTools()
{
	m_unlockStageNum=0;
}
XMLTools::~XMLTools()
{

}
XMLTools*XMLTools::getInstance()
{
	if(!m_inst)
	{
		m_inst=new XMLTools();
		std::string pathName = CCFileUtils::sharedFileUtils()->getWritablePath() + UI_STAGE_CONFIGER_FILE;
		if(!m_inst->isFileExist(pathName.c_str()))
			m_inst->createXML(UI_STAGE_CONFIGER_FILE);
		m_inst->readXMLWithFix(UI_STAGE_CONFIGER_FILE);
	}
	return m_inst;
}

void XMLTools::createXML(char*fileName)
{
	std::string filePath = CCFileUtils::sharedFileUtils()->getWritablePath()+fileName;
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	//xml 声明
	XMLDeclaration*pDecl=pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
	pDoc->LinkEndChild(pDecl);
	//添加节点
	XMLElement *pNodeElement = pDoc->NewElement("data");
	pNodeElement->SetAttribute("version", "1.0");
	pDoc->LinkEndChild(pNodeElement);
	//添加注释
	XMLComment *commentElement = pDoc->NewComment("this is stageXML");
	pNodeElement->LinkEndChild(commentElement);

	int num=StageMapDict::getInstance()->getStageCountNum();

	for (int i=0;i<num;i++)
	{
		stageInfo*info=new stageInfo();
		
		info->m_stageNum=i+1;
		if(info->m_stageNum==1)
			info->m_isLocked=false;
		if(info->m_stageNum%20==1&&info->m_stageNum!=1)
			info->m_isSpecialStage=true;
		//添加子节点
		XMLElement *dicElement = this->createElementWithStageInfo(pDoc,info);
		pNodeElement->LinkEndChild(dicElement);
	}

	pDoc->SaveFile(filePath.c_str());
	//pDoc->Print();
	delete pDoc;
}

XMLElement *XMLTools::createElementWithStageInfo(tinyxml2::XMLDocument *pDoc,stageInfo*info)
{
	//添加子节点
	//XMLElement *dicElement = pDoc->NewElement("stageNum");
	XMLElement *dicElement = pDoc->NewElement(CCString::createWithFormat("stageNum%d",info->m_stageNum)->getCString());
	//添加key节点
	XMLElement *keyElement = pDoc->NewElement("key");

	keyElement->LinkEndChild(pDoc->NewText(CCString::createWithFormat("%d",info->m_stageNum)->getCString()));
	dicElement->LinkEndChild(keyElement);

	XMLElement *arrayElement = pDoc->NewElement("startArray");
	dicElement->LinkEndChild(arrayElement);

	for (int j = 0; j<3; j++) 
	{
		char str[10]={0};
		sprintf(str,"start%d",j+1);
		XMLElement *elm = pDoc->NewElement(str);

		CCString*pString=NULL;
		if(j==0)
			pString=CCString::createWithFormat("%d",info->m_firstStar);
		else if(j==1)
			pString=CCString::createWithFormat("%d",info->m_secondStar);
		else if(j==2)
			pString=CCString::createWithFormat("%d",info->m_threeStar);

		elm->LinkEndChild(pDoc->NewText(pString->getCString()));
		arrayElement->LinkEndChild(elm);
	}

	XMLElement *isLockElement = pDoc->NewElement("islock");
	if(info->m_isLocked)
		isLockElement->LinkEndChild(pDoc->NewText("true"));
	else
		isLockElement->LinkEndChild(pDoc->NewText("false"));
	dicElement->LinkEndChild(isLockElement);

	XMLElement *isSpecialElement = pDoc->NewElement("isSpecial");
	if(info->m_isSpecialStage)
		isSpecialElement->LinkEndChild(pDoc->NewText("true"));
	else
		isSpecialElement->LinkEndChild(pDoc->NewText("false"));
	dicElement->LinkEndChild(isSpecialElement);

	XMLElement *maxScore = pDoc->NewElement("maxScore");
	maxScore->LinkEndChild(pDoc->NewText(CCString::createWithFormat("%d",info->m_maxScore)->getCString()));
	dicElement->LinkEndChild(maxScore);

	return dicElement;
}

std::vector<stageInfo>XMLTools::readXML(char*fileName)
{
	std::string filePath = CCFileUtils::sharedFileUtils()->getWritablePath() + fileName;
	std::cout<<"filePath:-------------------"<<filePath<<std::endl;
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	tinyxml2::XMLError errorId = pDoc->LoadFile(filePath.c_str());

	m_stageInfoArray.clear();

	if (errorId != 0) 
	{
		cocos2d::CCLog("load xml file:%s. error:%d", fileName, errorId);
		return m_stageInfoArray;
	}
	int num=1;
	tinyxml2::XMLElement * rootElement =pDoc->RootElement();
	tinyxml2::XMLElement * curRoot =rootElement->FirstChildElement(CCString::createWithFormat("stageNum%d",num)->getCString());
	while (curRoot)
	{
		stageInfo stage;
		tinyxml2::XMLElement *curElement=curRoot->FirstChildElement("key");
		curElement->QueryIntText(&stage.m_stageNum);
		curElement=curElement->NextSiblingElement("startArray");
		{
			tinyxml2::XMLElement *tempElement=curElement->FirstChildElement();
			tempElement->QueryIntText(&stage.m_firstStar);
			tempElement=tempElement->NextSiblingElement();
			tempElement->QueryIntText(&stage.m_secondStar);
			tempElement=tempElement->NextSiblingElement();
			tempElement->QueryIntText(&stage.m_threeStar);
		}
		curElement=curElement->NextSiblingElement("islock");
		curElement->QueryBoolText(&stage.m_isLocked);
		if(!stage.m_isLocked)
			m_unlockStageNum++;
		curElement=curElement->NextSiblingElement("isSpecial");
		curElement->QueryBoolText(&stage.m_isSpecialStage);
		curElement=curElement->NextSiblingElement("maxScore");
		curElement->QueryIntText(&stage.m_maxScore);

		m_stageInfoArray.push_back(stage);
		num++;
		curRoot=curRoot->NextSiblingElement(CCString::createWithFormat("stageNum%d",num)->getCString());
	}
	delete pDoc;
	return m_stageInfoArray;
}

std::vector<stageInfo> XMLTools::readXMLWithFix(char*name)
{
	this->readXML(name);
	if(m_stageInfoArray.size()!=StageMapDict::getInstance()->getStageCountNum())
	{
		this->addElementToXmlFile(name);
	}
	return m_stageInfoArray;
}

void XMLTools::addElementToXmlFile(char*fileName)
{
	std::string pathName = CCFileUtils::sharedFileUtils()->getWritablePath() + fileName;
	if(!isFileExist(pathName.c_str()))
		return;
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	tinyxml2::XMLError errorId = pDoc->LoadFile(pathName.c_str());

	if (errorId != 0) 
	{
		cocos2d::CCLog("load xml file:%s. error:%d", fileName, errorId);
		return;
	}

	tinyxml2::XMLElement * pNodeElement =pDoc->RootElement();
	int curNum=m_stageInfoArray.size();
	int countNum=StageMapDict::getInstance()->getStageCountNum();
	for (int i=curNum;i<countNum;i++)
	{
		stageInfo*info=new stageInfo();
		info->m_stageNum=i+1;
		if(i==0)
			info->m_isLocked=false;
		if(info->m_stageNum%20==1&&info->m_stageNum!=1)
			info->m_isSpecialStage=true;
		m_stageInfoArray.push_back(*info);
		//添加子节点
		XMLElement *dicElement = this->createElementWithStageInfo(pDoc,info);
		pNodeElement->LinkEndChild(dicElement);
	}
	pDoc->SaveFile(pathName.c_str());
	//pDoc->Print();
	delete pDoc;
}

void XMLTools::modifyXML(char*fileName,stageInfo*info)
{
	std::string pathName = CCFileUtils::sharedFileUtils()->getWritablePath() + fileName;
	if(!isFileExist(pathName.c_str()))
		this->createXML(fileName);

	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	tinyxml2::XMLError errorId = pDoc->LoadFile(pathName.c_str());

	if (errorId != 0) 
	{
		cocos2d::CCLog("load xml file:%s. error:%d", fileName, errorId);
		return;
	}

	tinyxml2::XMLElement * rootElement =pDoc->RootElement();

	tinyxml2::XMLElement * curRoot =rootElement->FirstChildElement(CCString::createWithFormat("stageNum%d",info->m_stageNum)->getCString());

	char strTemp[20]={0};

	tinyxml2::XMLElement *curElement=curRoot->FirstChildElement("startArray");
	{
		tinyxml2::XMLElement *tempElement=curElement->FirstChildElement();
		sprintf(strTemp,"%d",info->m_firstStar);
		tempElement->FirstChild()->SetValue(strTemp);

		tempElement=tempElement->NextSiblingElement();
		sprintf(strTemp,"%d",info->m_secondStar);
		tempElement->FirstChild()->SetValue(strTemp);

		tempElement=tempElement->NextSiblingElement();
		sprintf(strTemp,"%d",info->m_threeStar);
		tempElement->FirstChild()->SetValue(strTemp);
	}
	curElement=curElement->NextSiblingElement("islock");
	if(info->m_isLocked)
		sprintf(strTemp,"true");
	else
		sprintf(strTemp,"false");
	curElement->FirstChild()->SetValue(strTemp);

	curElement = curElement->NextSiblingElement("isSpecial");
	if (info->m_isSpecialStage)
		sprintf(strTemp, "true");
	else
		sprintf(strTemp, "false");
	curElement->FirstChild()->SetValue(strTemp);

	curElement=curElement->NextSiblingElement("maxScore");
	sprintf(strTemp,"%d",info->m_maxScore);
	curElement->FirstChild()->SetValue(strTemp);

	pDoc->SaveFile(pathName.c_str());
	//pDoc->Print();
	delete pDoc;
}


bool XMLTools::isFileExist(const char *path)
{
	FILE *fp = fopen(path, "r");
	bool bRet = false;
	if (fp)
	{
		bRet = true;
		fclose(fp);
	}
	return bRet;
}

std::vector<stageInfo> XMLTools::getStageInfo()
{
	return m_stageInfoArray;
}

void XMLTools::unlockMapStageByIndex(stageInfo*info)
{
	m_stageInfoArray[info->m_stageNum-1]=*info;
	this->modifyXML(UI_STAGE_CONFIGER_FILE,&m_stageInfoArray[info->m_stageNum-1]);
}

void XMLTools::unlockMapStageByIndex(int index)
{
	if (index - 1 >= m_stageInfoArray.size())
		return;
	m_stageInfoArray[index-1].m_isLocked=false;
	if (m_unlockStageNum == index-1)
	{
		m_unlockStageNum++;
	}
	if (m_stageInfoArray[index - 1].m_isSpecialStage)
	{
		int num = 0;
		for (int i = 1 + 20 * (index / 20-1); i < index; i++)
		{
			if (m_stageInfoArray[i - 1].m_firstStar)
				num++;
			if (m_stageInfoArray[i - 1].m_secondStar)
				num++;
			if (m_stageInfoArray[i - 1].m_threeStar)
				num++;
		}
		if (num>=50)
		{
			m_stageInfoArray[index - 1].m_isSpecialStage = false;
		}
	}
	this->modifyXML(UI_STAGE_CONFIGER_FILE,&m_stageInfoArray[index-1]);
}

void XMLTools::updateMapStageInfo(int index,int starNum,int maxScore)
{
	stageInfo*info=&m_stageInfoArray[index-1];
	int num=info->m_firstStar+info->m_secondStar+info->m_threeStar;

	if(num<starNum)
	{
		if(starNum>=1)
			info->m_firstStar=1;
		if(starNum>=2)
			info->m_secondStar=1;
		if(starNum>=3)
			info->m_threeStar=1;
	}
	if(maxScore<info->m_maxScore)
		return;
	info->m_maxScore=maxScore;
	this->modifyXML(UI_STAGE_CONFIGER_FILE,&m_stageInfoArray[index-1]);
}

int XMLTools::getStageCount()
{
	return m_stageInfoArray.size();
}

int XMLTools::getUnlockStageNum()
{
	return m_unlockStageNum;
}

stageInfo*XMLTools::getStageInfoByIndex(int index)
{
	return &m_stageInfoArray[index-1];
}

void XMLTools::modifyStageInfoByInfo(stageInfo*info)
{
	m_stageInfoArray[info->m_stageNum-1]=*info;
	this->modifyXML(UI_STAGE_CONFIGER_FILE, &m_stageInfoArray[info->m_stageNum - 1]);
}
#include "HelpItemDict.h"
#include "support/tinyxml2/tinyxml2.h"
#include "CommonFunc.h"

#define UI_HELP_XML_INFO "res/helpData.xml"

using namespace cocos2d;
using namespace std;
using namespace tinyxml2;

HelpItemDict* HelpItemDict::m_helpDic = NULL;

HelpItemDict* HelpItemDict::getInstance()
{
	if(!m_helpDic)
	{
		m_helpDic=new HelpItemDict();
		m_helpDic->init();
	}
	return m_helpDic;
}

bool HelpItemDict::init()
{
	this->getInfoFromXml(UI_HELP_XML_INFO);
	return true;
}

const HelpItem HelpItemDict::getHelpItemInfos(int helpId)
{
    return m_helpItems[helpId];
}

void HelpItemDict::getInfoFromXml(char*pFileName)
{
	tinyxml2::XMLDocument doc;
	if(!CommonFunc::loadInnerDictFile(doc, pFileName))
	{
		return;
	}
	m_helpItems.clear();
	tinyxml2::XMLElement * curNode = NULL;
	curNode=doc.RootElement()->FirstChildElement("data");
	while(curNode)
	{
		HelpItem info;
		curNode->QueryIntAttribute("id",&info.id);
		info.imagePath=curNode->Attribute("imagePath");
		info.declarePath=curNode->Attribute("declarePath");
		info.declareContent=curNode->Attribute("declareContent");
		m_helpItems.push_back(info);
		curNode = curNode->NextSiblingElement("data");
	}
}

std::vector<HelpItem>HelpItemDict::getHelpItems()
{
	return m_helpItems;
}
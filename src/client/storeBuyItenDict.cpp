#include "storeBuyItenDict.h"
#include "support/tinyxml2/tinyxml2.h"
#include "CommonFunc.h"

#define UI_STORE_XML_INFO "res/exchange.xml"

using namespace cocos2d;
using namespace std;
using namespace tinyxml2;

StoreBuyItemDict* StoreBuyItemDict::m_dict = NULL;

StoreBuyItemDict* StoreBuyItemDict::getInstance()
{
	if(!m_dict)
	{
		m_dict=new StoreBuyItemDict();
		m_dict->init();
	}
	return m_dict;
}

bool StoreBuyItemDict::init()
{
	this->getInfoFromXml(UI_STORE_XML_INFO);
	return true;
}

const BuyItem StoreBuyItemDict::getBuyItemInfos(int stageId)
{
    return m_buyItems[stageId];
}

void StoreBuyItemDict::getInfoFromXml(char*pFileName)
{
	tinyxml2::XMLDocument doc;
	if(!CommonFunc::loadInnerDictFile(doc, pFileName)){
		return;
	}
	m_buyItems.clear();
	tinyxml2::XMLElement * curNode = NULL;
	curNode=doc.RootElement()->FirstChildElement("store");
	tinyxml2::XMLElement * nextNode = NULL;
	nextNode = curNode->FirstChildElement("data");
	while (nextNode)
	{
		BuyItem info;
		nextNode->QueryIntAttribute("id", &info.id);
		nextNode->QueryIntAttribute("gold", &info.gold);
		nextNode->QueryIntAttribute("rmb", &info.rmb);
		nextNode->QueryIntAttribute("presentGold", &info.presentGold);
		nextNode->QueryBoolAttribute("isVisibleInStore", &info.isvisible);
		m_buyItems.push_back(info);
		nextNode = nextNode->NextSiblingElement("data");
	}
}

std::vector<BuyItem>StoreBuyItemDict::getBuyItems()
{
	return m_buyItems;
}
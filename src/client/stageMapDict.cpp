#include "StageMapDict.h"
#include "CommonFunc.h"
#include "support/tinyxml2/tinyxml2.h"
#include "platform/CCFileUtils.h"
#include "myGame.h"
#include "BubbleConstant.h"

#define STAGE_MAP_XML_PATH      "res/stage.xml"

using namespace std;

StageMapDict* StageMapDict::m_dict = NULL;

int StageMapDict::init(void)
{
    m_dict = this;
    return 0;
}
StageMapDict::StageMapDict(void)
{
}

StageMapDict::~StageMapDict(void)
{
}

StageMapDict* StageMapDict::loadInst(){
    std::string path = std::string("");
    path += cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath()+STAGE_MAP_XML_PATH;
    tinyxml2::XMLDocument doc;//创建一个xml对象
    if(!CommonFunc::loadInnerDictFile(doc, STAGE_MAP_XML_PATH)){
        return NULL;
    }
    StageMapDict * stageMapDict = new StageMapDict;
    stageMapDict->loadInfos(doc.RootElement());
    m_dict = stageMapDict;
    return stageMapDict;
}

int StageMapDict::loadInfos( tinyxml2::XMLElement * rootNode )
{   
    if(NULL == rootNode){
        return 1;
    }
    stageMapInfos.clear();
    tinyxml2::XMLElement * curNode = NULL;
    curNode = rootNode->FirstChildElement("data");
    int index = 0;
    while(curNode){
        StageMapInfo* info = new StageMapInfo();
        info->m_id = curNode->IntAttribute("id");
        string mapDataStr = curNode->Attribute("data");
        info->m_data = parse(mapDataStr);
		info->m_lines=info->m_data.size();
        this->stageMapInfos[info->m_id] = info;
        curNode = curNode->NextSiblingElement("data");
    }
    return 0;
}

const vector<vector<std::string> > StageMapDict::parse(string data)
{
	vector<vector<std::string> > stageMapData;
    CommonFunc::splitDoubleArray(stageMapData, data, ";",MAX_COLS);
    return stageMapData;
}

const StageMapInfo* StageMapDict::getStageMapInfos(int stageId)
{
    return stageMapInfos[stageId];
}

int StageMapDict::getStageCountNum()
{
	return stageMapInfos.size();
}

#ifndef __STAGE_MAP_DICT_H__
#define __STAGE_MAP_DICT_H__
#include <vector>
#include <map>
#include "stageMapInfo.h"
#include "support/tinyxml2/tinyxml2.h"
#include "BubbleConstant.h"
using namespace std;

class StageMapDict
{
public:
    static StageMapDict* getInstance()
    {
        if (!m_dict)
        {
            loadInst();
        }
        return m_dict;
    }
    int init();
    StageMapDict(void);
    ~StageMapDict(void);
    static StageMapDict* loadInst();
    int loadInfos( tinyxml2::XMLElement * rootNode );
    const StageMapInfo* getStageMapInfos(int stageId);
	const vector<vector<std::string> > parse(string data);
	int getStageCountNum();
protected:
    static StageMapDict* m_dict;
    map<int, StageMapInfo*> stageMapInfos;
};
#endif //__EXCHANGE_DICT_H__


#ifndef __STAGE_MAP_INFO_H__
#define __STAGE_MAP_INFO_H__
#include <string>
#include <vector>
#include "BubbleConstant.h"


class StageMapInfo
{
public:
    StageMapInfo(void); 
    ~StageMapInfo(void);
    int m_id;
	std::vector<std::vector<std::string> > m_data;
	int m_lines;
};
#endif//__EXCHANGE_INFO_H__

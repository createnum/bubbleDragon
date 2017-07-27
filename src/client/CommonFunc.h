#ifndef __COMMON_FUCTION_H__
#define __COMMON_FUCTION_H__
#include "support/tinyxml2/tinyxml2.h"
#include <vector>
#include <string>
#include "myGame.h"

using namespace cocos2d;
class CommonFunc
{
public:
    CommonFunc(void);
    ~CommonFunc(void);
    static bool loadInnerDictFile(tinyxml2::XMLDocument &doc, const char * fileName);
	static tinyxml2::XMLElement * loadMapDictFile(const char * fileName);
    static bool stringEqual(const char* first, const char* second, int cycleCount = INT_MAX);
    static void parseStringToIntArray(std::vector<int>& array, const char* str, char signal);
    static bool writeToNativePlace(char *pContent, const char * pFileName);
    static void split(std::vector<int>& array, std::string str, std::string pattern);
	static void splitDoubleArray(std::vector<std::vector<std::string> >& array, std::string str, std::string pattern, int len);
    static std::string getCurTimeStr();
    static long long getCurMillisecond();

    static void addChildByAction(cocos2d::gui::TouchGroup* Layer){
        addChildByAction(Layer, 0);
    };
    static void addChildByAction(cocos2d::gui::TouchGroup* Layer, int layerNum){
        addChildByAction(Layer, layerNum, 0.15f, 0.1f);
    }
    static void addChildByAction(cocos2d::gui::TouchGroup* Layer, int layerNum, float extendDuration, float shrinkDuration);
    static void replaceSceneByActoin( cocos2d::CCScene* scene );
	static void createXmlFile(const char * fileName, int id, char * data);
};


#endif //__COMMON_FUCTION_H__
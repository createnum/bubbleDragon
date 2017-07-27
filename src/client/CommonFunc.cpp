#include "CommonFunc.h"
#include "platform/CCFileUtils.h"
#include "support/tinyxml2/tinyxml2.h"
#include <ctime>

#define  ANDROID_SD_ROOT_PATH  "/mnt/sdcard/";
using namespace tinyxml2;
using namespace NEWBEE_UTILS;

CommonFunc::CommonFunc(void)
{

}

CommonFunc::~CommonFunc(void)
{
}
XMLElement * CommonFunc::loadMapDictFile(const char * fileName){
	unsigned char *pBytes = NULL;
	unsigned long size = 0;
	pBytes = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fileName, "rb", &size);
	if (size <= 0)
	{
		MyGame::getInstance()->m_mainLog->printlogs(Logging::LOG_LEVEL_WARN, "load xml file:%s. error size:%d", fileName, size);
		return NULL;
	}
	tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument;
	XMLError error = doc->Parse(reinterpret_cast<char*>(pBytes), size);
	if (XML_NO_ERROR != error)
	{
		MyGame::getInstance()->m_mainLog->printlogs(Logging::LOG_LEVEL_WARN, "load xml file:%s. error:%d", fileName, error);
		return NULL;
	}
	return doc->RootElement();
}


void CommonFunc::createXmlFile(const char * fileName, int id, char * data)
{
	//xml文档
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	if (NULL == pDoc) {
		return;
	}
	//xml声明
	XMLDeclaration *pDel = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
	if (NULL == pDel) {
		return;
	}
	pDoc->LinkEndChild(pDel);
	//节点plist
	XMLElement *includeElement = pDoc->NewElement("include");
	
	pDoc->LinkEndChild(includeElement);
	//节点dict
	XMLElement *dataElement = pDoc->NewElement("data");
	dataElement->SetAttribute("id", id);//给节点设置属性
	dataElement->SetAttribute("data", data);//给节点设置属性
	includeElement->LinkEndChild(dataElement);


	pDoc->SaveFile(fileName);//保存文件 参数：路径
	//pDoc->Print();//打印
	delete pDoc;
}

//读取文件数据
bool CommonFunc::loadInnerDictFile(tinyxml2::XMLDocument &doc, const char * fileName){
    unsigned char *pBytes = NULL;
    unsigned long size = 0;
	//底层调用fopen  rb 读写打开一个二进制文件，允许读写数据，文件必须存在
    pBytes = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fileName,"rb" , &size);
    if (size <= 0 || !pBytes)
    {
        cocos2d::CCLog("load xml file:%s. error size:%d", fileName, size);
        return false;
    }
	//读取文本内容到xml对象的_charBuffer
    XMLError error = doc.Parse(reinterpret_cast<char*>(pBytes), size);
    delete []pBytes;
    pBytes = 0;
    if(XML_NO_ERROR != error)
    {
        cocos2d::CCLog("load xml file:%s. error:%d", fileName, error);
        return false;
    }
    return true;
}

//判断两个字符串是否相等
bool CommonFunc::stringEqual(const char* first, const char* second, int cycleCount)
{
    int count = 0;
    if (first == second)
    {
        return true;
    }
    while(*first != '\0' && *second != '\0' && *first == *second && count < cycleCount)//'0' = 0
    {
        ++first;
        ++second;
        ++count;
    }
    if (count == cycleCount || (*first == 0 && *second == 0))
    {
        return true;
    }
    return false;
}

//文本内容写入
bool CommonFunc::writeToNativePlace(char *pContent, const char * pFileName){
    std::string path = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath() + pFileName;  
    FILE* file = fopen(path.c_str(), "w");  
    if (file) {  
        fputs(pContent, file);  
        fclose(file);
        return true;
    }  
    return false;  
}

//将文本中的字符串转化为与之对应的int整数,并压入数组
void CommonFunc::parseStringToIntArray(std::vector<int>& array, const char* str, char signal)
{
    int result = 0;
    char spiltStr[sizeof(str)];
    memset(spiltStr, 0 , sizeof(spiltStr));
    int count = 0;
    while (*str !='\0')
    {
        if (*str == signal)
        {
            spiltStr[count] = '\0';
            result = atoi(spiltStr);
            array.push_back(result);
            memset(spiltStr, 0 , sizeof(spiltStr));
            count = 0;
        }else{
            spiltStr[count] = *str;
            count++;
        }
        ++str;
    }
    if (count > 0)
    {
        spiltStr[count] = '\0';
        result = atoi(spiltStr);
        array.push_back(result);
    }
}

//字符串分割函数
void CommonFunc::split(std::vector<int>& array, std::string str, std::string pattern)
{
    std::string::size_type pos;//由string类类型和vector类类型定义的类型,以保存任意string对象或vector对象的长度
    str+=pattern;//扩展字符串以方便操作
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);//字符串截取
            int intValue = -1;
            std::stringstream ss;//输入输出流
            ss << s;//读入
            ss >> intValue;//将 intValue的值改变为s 
            array.push_back(intValue);
            i=pos+pattern.size()-1;
        }
    }
}

//字符串分割至二维数组
void CommonFunc::splitDoubleArray(std::vector<std::vector<std::string> >& array, std::string str, std::string pattern, int len)
{
	std::string::size_type pos;//由string类类型和vector类类型定义的类型,以保存任意string对象或vector对象的长度
	str+=pattern;//扩展字符串以方便操作
	int size=str.size();
	int count=0;
	int i=0;

	while(i<size-1)
	{
		std::vector<std::string> tempArr;
		for (int j=0;j<len;j++)
		{
			pos=str.find(pattern,i);//从字符串str下标i开始,查找字符串pattern,返回pattern在str中的下标
			if(pos<size-1)
			{
				std::string s=str.substr(i,pos-i);//字符串截取
				//int intValue = -1;
				//std::stringstream ss;//输入输出流
				//ss << s;//读入
				//ss >> intValue;//将 intValue的值改变为s
				tempArr.push_back(s);
				i=pos+pattern.size()-1;
				i++;
			}
		}
		array.push_back(tempArr);
	}
}


//获取当前时间
std::string CommonFunc::getCurTimeStr()
{
    struct tm *tm;  
    time_t timep;  
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	time(&timep);  
#else  
	struct cc_timeval now;   
	CCTime::gettimeofdayCocos2d(&now, NULL);   
	timep = now.tv_sec;  
#endif  
    tm = localtime(&timep);  
    int year = tm->tm_year + 1900;  
    int month = tm->tm_mon + 1;  
    int day = tm->tm_mday;  
    int hour=tm->tm_hour;  
    int min=tm->tm_min;  
    int second=tm->tm_sec; 
    std::ostringstream curTime;
    curTime <<year<<month<<day<<hour<<min<<second;
    return curTime.str();
}
//
long long CommonFunc::getCurMillisecond()
{
    struct cc_timeval now;   
    CCTime::gettimeofdayCocos2d(&now, NULL);   
    long long  time = ((long long)now.tv_sec) * 1000+ now.tv_usec / 1000;
    return time;
}


void CommonFunc::addChildByAction(cocos2d::gui::TouchGroup* Layer, int layerNum, float extendDuration, float shrinkDuration)
{
    Layer->setScale(0);
    CCDirector::sharedDirector()->getRunningScene()->addChild(Layer, layerNum);
    CCScaleTo* scaleTo = CCScaleTo::create(extendDuration, 1.25);
    CCScaleTo* scaleTo2 = CCScaleTo::create(shrinkDuration, 1);
    Layer->runAction(CCSequence::createWithTwoActions(scaleTo,scaleTo2));
}

void CommonFunc::replaceSceneByActoin( cocos2d::CCScene* scene )
{
    CCTransitionScene * reScene = NULL;
    reScene = CCTransitionFade::create(1.0f, scene);
    CCDirector::sharedDirector()->replaceScene(reScene);
}


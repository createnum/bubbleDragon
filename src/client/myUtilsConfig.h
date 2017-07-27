#ifndef __MY_UTILS_CONFIG_H__
#define __MY_UTILS_CONFIG_H__

#include <string>

class MyUtilsConfig{
public:
    static MyUtilsConfig* getInst(){
        if (!m_inst) {
            m_inst=new MyUtilsConfig();
            m_inst->init();
        }
        return m_inst;
    }
    std::string m_mainLogPath;
	std::string m_userDefineStageXmlPath;
protected:
    void init();
    static MyUtilsConfig *m_inst;
};

#endif
#ifndef NEWBEE__SMART_RETURN_VALUE_H
#define NEWBEE__SMART_RETURN_VALUE_H
#include "newbeeUtils/exports.h"
#include "newbeeUtils/logging.h"
//维护一个核心的value值，实现one in, one out 功能：生存期内赋且仅赋一次值。
namespace NEWBEE_UTILS{
template<class T>
class NBUAPI SmartReturnValue
{
public:
	//const int INIT_VALUE = -2;
	SmartReturnValue(Logging *log=0){
		m_log = 0;
		m_isModified = false;
	}
	SmartReturnValue(T value, Logging *log=0)
		:m_value(value),
		m_log(log)
	{
		m_isModified = true;
	}
	void setValue(T value){
		if(m_isModified){
			if(m_log){
				m_log->warnlog("multiple modified!");
			}
		}else{
			m_isModified = true;
			m_value = value;
		}
	}
	void setValue(T *pValue)
	{
		if(m_isModified){
			if(m_log){
				m_log->warnlog("multiple modified!");
			}
		}else{
			m_isModified = true;
			m_value = *pValue;
		}
	}
	T &getValue(){
		return m_value;
	}
	T &returnValue()
	{
		if(!m_isModified){
			if(m_log){
				m_log->warnlog("not modified, before return!");
			}
		}
		return m_value;
	}
	T& operator=(T &value){
		if(m_isModified){
			if(m_log){
				m_log->warnlog("multiple modified!");
			}
		}else{
			m_isModified = true;
			m_value = value;
		}
		return m_value;
	}
	~SmartReturnValue(void){};
protected:
	Logging *m_log;
	T m_value;
	bool m_isModified;
};
}
#endif //NEWBEE__SMART_RETURN_VALUE_H



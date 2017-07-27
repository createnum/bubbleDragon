#ifndef NEWBEEUTILS_NBOSAPI_H
#define NEWBEEUTILS_NBOSAPI_H
#include "exports.h"
namespace NEWBEE_UTILS{
NBUAPI void nbSleep(int millionseconds); // 参数单位：毫秒
NBUAPI int nbGetCurThreadId();
NBUAPI void nbExit(int exitNo = 1);
//class NBUAPI NBOSAPI //: public // (4)
//{
//public:
//    NBOSAPI(){//: m_complex_Object() // (3)
//        NBOSAPI_();
//    }
//    virtual ~NBOSAPI(){
//        NBOSAPI__();
//    }
//    int clear();
//protected: //simple object(including unliable pointer) - (1); liable pointer - (2); complex object - (3)
//    void NBOSAPI_();
//    int NBOSAPI__();
//}; //class NBUAPI NBOSAPI
}  // namespace NEWBEE_UTILS
#endif //NEWBEEUTILS_NBOSAPI_H


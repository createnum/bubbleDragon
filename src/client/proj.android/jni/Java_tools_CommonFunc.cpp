#include "Java_tools_CommonFunc.h"
#include <jni.h>
#include <android/log.h>

#include "platform/android/jni/JniHelper.h"
using namespace cocos2d;

std::string getSdcardRootPathJni() {
    __android_log_print(ANDROID_LOG_DEBUG, "java com.skt.sdk.tools.commonFunc", "%s", "get sdcard root path");
    std::string ret;
    JniMethodInfo t;

    if (JniHelper::getStaticMethodInfo(t, "com/skt/sdk/tools/CommonFunc", "getSdcardRootPath", "()Ljava/lang/String;")) {
        __android_log_print(ANDROID_LOG_DEBUG, "java com.skt.sdk.tools.commonFunc", "%s", "get method info success");
        jstring retFromJava = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if(retFromJava){
            const char* str = t.env->GetStringUTFChars(retFromJava, 0);
            ret = str;
            __android_log_print(ANDROID_LOG_DEBUG, "java com.skt.sdk.tools.commonFunc", "%s:%s", "sdcard root path is:", str);
            t.env->ReleaseStringUTFChars(retFromJava, str);
        }else{
            __android_log_print(ANDROID_LOG_DEBUG, "java com.skt.sdk.tools.commonFunc", "%s", "sdcard root path is null");
        }
        t.env->DeleteLocalRef(t.classID);
    }else{
        __android_log_print(ANDROID_LOG_DEBUG, "java com.skt.sdk.tools.commonFunc", "%s", "get method info failed");
    }
    return ret;
}

std::string getSdcardPathJni(const char* pszText) {
    __android_log_print(ANDROID_LOG_DEBUG, "java com.skt.sdk.tools.commonFunc", "%s:%s", "get sdcard path...", pszText);
    std::string ret;
    __android_log_print(ANDROID_LOG_DEBUG, "java com.skt.sdk.tools.commonFunc", "%s", "try init jni method info");
    JniMethodInfo t;
    __android_log_print(ANDROID_LOG_DEBUG, "java com.skt.sdk.tools.commonFunc", "%s", "init jni method info");

    if (JniHelper::getStaticMethodInfo(t, "com/skt/sdk/tools/CommonFunc", "getSdcardPath", "(Ljava/lang/String;)Ljava/lang/String;")) {
        jstring stringArg1;
        if (!pszText) {
            stringArg1 = t.env->NewStringUTF("");
        } else {
            stringArg1 = t.env->NewStringUTF(pszText);
        }

        jstring retFromJava = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID, stringArg1);
        if(retFromJava){
            const char* str = t.env->GetStringUTFChars(retFromJava, 0);
            ret = str;
            __android_log_print(ANDROID_LOG_DEBUG, "java com.skt.sdk.tools.commonFunc", "%s:%s", "get sdcard path is:", str);
            t.env->ReleaseStringUTFChars(retFromJava, str);
        }else{
            __android_log_print(ANDROID_LOG_DEBUG, "java com.skt.sdk.tools.commonFunc", "%s", "get sdcard path is null");
        }
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }else{
        __android_log_print(ANDROID_LOG_DEBUG, "java com.skt.sdk.tools.commonFunc", "%s", "get method info failed");    
    }
    return ret;
}

 
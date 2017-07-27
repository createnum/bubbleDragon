#include "AppDelegate.h"
#include "cocos2d.h"
#include "CCEventType.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>
#include "newbeeUtils/logging.h"
#include "myGame.h"

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;

extern "C"
{
    
jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    __android_log_print(ANDROID_LOG_DEBUG, "main cpp", "%s", "on load");
    JniHelper::setJavaVM(vm);

    return JNI_VERSION_1_4;
}

void Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit(JNIEnv*  env, jobject thiz, jint w, jint h)
{
    __android_log_print(ANDROID_LOG_DEBUG, "main cpp", "%s", "native init begin");
    if (!CCDirector::sharedDirector()->getOpenGLView())
    {
        __android_log_print(ANDROID_LOG_DEBUG, "main cpp", "%s", "none view");
        MyGame* myGame = new MyGame();
        myGame->init();
        __android_log_print(ANDROID_LOG_DEBUG, "main cpp", "%s", "my game inited");
        
        CCEGLView *view = CCEGLView::sharedOpenGLView();
        view->setFrameSize(w, h);

        AppDelegate *pAppDelegate = new AppDelegate();
        MyGame::getInstance()->m_mainLog->printlogs(NEWBEE_UTILS::Logging::LOG_LEVEL_DEBUG, "begin run app delegate");
        CCApplication::sharedApplication()->run();
    }
    else
    {
        __android_log_print(ANDROID_LOG_DEBUG, "main cpp", "%s", "have view");
        ccGLInvalidateStateCache();
        CCShaderCache::sharedShaderCache()->reloadDefaultShaders();
        ccDrawInit();
        CCTextureCache::reloadAllTextures();
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_COME_TO_FOREGROUND, NULL);
        CCDirector::sharedDirector()->setGLDefaultValues(); 
    }
}

}

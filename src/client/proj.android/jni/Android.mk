LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE:=NewbeeUtils
LOCAL_SRC_FILES:=../../bin/lib/android/libNewbeeUtils.a 
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := cocos2dcpp_shared
LOCAL_MODULE_FILENAME := libcocos2dcpp
LOCAL_SRC_FILES := hellocpp/main.cpp \
                   myUtilsConfig.cpp \
                   Java_tools_CommonFunc.cpp \
		   ../../Bubble.cpp \
		   ../../gameScene.cpp \
           ../../myGame.cpp \
           ../../AppDelegate.cpp \
           ../../monitoringCenter.cpp \
           ../../StartScene.cpp \
		   ../../stageMapDict.cpp \
		   ../../stageMapInfo.cpp \
		   ../../CommonFunc.cpp \
		   ../../md5.cpp \
		   ../../uiBuyPropertyScene.cpp \
		   ../../uiHelpScene.cpp \
		   ../../uiPassGameScene.cpp \
		   ../../uiStoreScene.cpp \
		   ../../stageSelectScene.cpp \
		   ../../storeBuyItenDict.cpp \
		   ../../XMLTools.cpp \
		   ../../uiDeblockingScene.cpp \
			../../HelpItemDict.cpp \
		   ../../simulatorGameScene.cpp \
		   ../../AnimationTools.cpp \
		   ../../ToolsGuideLine.cpp \
		   ../../GamePay.cpp \
		   ../../uiCombo.cpp \
		   ../../uiPauseScene.cpp \
		   ../../uiResurrectionScene.cpp 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../.. $(LOCAL_PATH)/../../bin

LOCAL_STATIC_LIBRARIES := NewbeeUtils

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
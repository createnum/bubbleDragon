NDK_TOOLCHAIN_VERSION := 4.8
APP_STL := gnustl_static
APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -DCOCOS2D_DEBUG=1 -DUNIX -Wno-error=format-security
APP_CPPFLAGS += -std=c++11
after :=LOCAL_C_INCLUDES += ${ANDROID_NDK}/sources/cxx-stl/gnu-libstdc++/4.8/include

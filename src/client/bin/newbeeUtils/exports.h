#ifndef NEWBEE_UTILS_EXPORT_H
#define NEWBEE_UTILS_EXPORT_H
#ifdef UNIX
#define NEWBEE_UTILS_API /*extern "C"*/ 
#else
#define NEWBEE_UTILS_API /*extern "C"*/ __declspec(dllexport)
#endif
#define NBUAPI NEWBEE_UTILS_API

#ifdef UNIX
#define DLL_EXPORT 
#else
#ifdef NB_DLL_EXPORT // 导出类静态成员变量需要，原因待研究
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif
#endif

#endif //NEWBEE_UTILS_EXPORT_H

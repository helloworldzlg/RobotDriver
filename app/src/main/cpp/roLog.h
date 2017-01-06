//
// Created by zlg on 16-12-15.
//

#ifndef TESTANDROIDLOG_ROLOG_H
#define TESTANDROIDLOG_ROLOG_H

#define _LINUX_ANDROID
//#define ROLOG_TAG

#ifdef _LINUX_ANDROID
#include <android/log.h>
#include <pthread.h>

#ifndef ROLOG_TAG
#define ROLOG_TAG   "DRV_LOG"
#endif

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, ROLOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, ROLOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, ROLOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, ROLOG_TAG, __VA_ARGS__)

#else
#define LOGD(...) printf("ROLOG Debug\t"); printf(__VA_ARGS__); printf("\n")
#define LOGI(...) printf("ROLOG Info\t"); printf(__VA_ARGS__); printf("\n")
#define LOGW(...) printf("ROLOG Warn\t"); printf(__VA_ARGS__); printf("\n")
#define LOGE(...) printf("ROLOG Error\t"); printf(__VA_ARGS__); printf("\n")
#endif //_LINUX_ANDROID


#ifdef _LINUX_ANDROID

#ifdef _ROLOG_DEBUG
#define ROLOGD(fmt, args...) \
{ \
    const char * p___Name = strrchr (__FILE__, '/'); \
    p___Name = ((p___Name == NULL) ? __FILE__ : (p___Name + 1)); \
    LOGD ("[Debug] THD %08X:   %s  %s  %d    " fmt "\n", (int)pthread_self (), p___Name, __FUNCTION__, __LINE__, ## args); \
}
#else
#define ROLOGD(fmt, args...)
#endif //_ROLOG_DEBUG


#ifdef _ROLOG_INFO
#define ROLOGI(fmt, args...) \
{ \
    const char * p___Name = strrchr (__FILE__, '/'); \
    p___Name = ((p___Name == NULL) ? __FILE__ : (p___Name + 1)); \
    LOGI ("[Info]  THD %08X:   %s  %s  %d    " fmt "\n", (int)pthread_self (), p___Name, __FUNCTION__, __LINE__, ## args); \
}
#else
#define ROLOGI(fmt, args...)
#endif //_ROLOG_INFO


#ifdef _ROLOG_WARNING
#define ROLOGW(fmt, args...) \
{ \
    const char * p___Name = strrchr (__FILE__, '/'); \
    p___Name = ((p___Name == NULL) ? __FILE__ : (p___Name + 1)); \
    LOGW ("[Warn]  THD %08X:   %s  %s  %d    " fmt "\n", (int)pthread_self (), p___Name, __FUNCTION__, __LINE__, ## args); \
}
#else
#define ROLOGW(fmt, args...)
#endif //_ROLOG_WARNING


#define ROLOGE(fmt, args...) \
{ \
    const char * p___Name = strrchr (__FILE__, '/'); \
    p___Name = ((p___Name == NULL) ? __FILE__ : (p___Name + 1)); \
    LOGE ("[Error] THD %08X:   %s  %s  %d    " fmt "\n", (int)pthread_self (), p___Name, __FUNCTION__, __LINE__, ## args); \
}
#else
#define ROLOGD /*LOGD*/
#define ROLOGI LOGI
#define ROLOGW LOGW
#define ROLOGE LOGE
#endif //_LINUX_ANDROID

#endif //TESTANDROIDLOG_ROLOG_H

//
// Created by zlg on 17-1-5.
//

#include <jni.h>
#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include "com_robot_et_core_hardware_device_RobotDevice.h"

static int gRobotTouchDevId;
static pthread_t gRobotTouchThreadId;
static RobotDevice_Thread_S ThreadPara;
static JavaVM*			g_jvm = 0;
static JNIEnv*			g_jEnv = 0;
static jclass			g_jClass = 0;
static jmethodID       g_jmethodId = 0;

void* RobotTouchFunc(void* pThreadPara)
{
	int i;
    int touchId;
    LOGD("RobotTouchFunc");

    (*g_jvm)->AttachCurrentThread(g_jvm, &g_jEnv, NULL);

    while (1) {
        touchId = ioctl(gRobotTouchDevId, 1, 0);
        for(i = 0; i < 4; i++) {
            if (((touchId >> i) & 0x1) != 0) {
                (*g_jEnv)->CallStaticVoidMethod(g_jEnv, g_jClass, g_jmethodId, i);
                usleep(200000);
                break;
            }
        }
    }

    (*g_jvm)->DetachCurrentThread(g_jvm);

    pthread_exit(NULL);
}

int RobotTouchInit(JNIEnv *env, jclass cls)
{
    int ret;
    jclass l_clazz = NULL;
    LOGD("RobotTouchInit");

    gRobotTouchDevId = open("/dev/robot_touch", O_RDWR);
    if (gRobotTouchDevId < 0) {
        LOGD("robot_touch open failed!!!");
        return ROBOT_TOUCH_OPEN_DEV_ERR;
    }

    ret =  (*env)->GetJavaVM(env, &g_jvm);
    if (ret != 0) {
        return ROBOT_TOUCH_GET_JAVAVM_ERR;
    }

    l_clazz = (jclass)(*env)->NewGlobalRef(env, cls);
    if (l_clazz == NULL) {
        return ROBOT_TOUCH_FIND_CLASS_ERR;
    }

    jmethodID method = (*env)->GetStaticMethodID(env, cls, "callBack_Touch", "(I)V");
    if (method == NULL) {
        return ROBOT_TOUCH_CALLBACK_ERR;
    }

    g_jClass = l_clazz;
    g_jmethodId = method;

    ret = pthread_create(&gRobotTouchThreadId, NULL, RobotTouchFunc, NULL);
    if (ret != 0) {
        return ROBOT_TOUCH_THREAD_CREATE_ERR;
    }

    return 0;
}

int RobotTouchUnInit(JNIEnv *env, jclass cls)
{
    return 0;
}
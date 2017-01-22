//
// Created by zlg on 17-1-10.
//

#include <jni.h>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include "roLog.h"
#include "com_robot_et_core_hardware_device_RobotDevice.h"

static int gRobotGpioDevId;
static pthread_t gRobotGpioThreadId;

static JavaVM*			g_jvm = 0;
static JNIEnv*			g_jEnv = 0;
static jclass			g_jClass = 0;
static jmethodID       g_jmethodId = 0;

void *RobotGpioFunc(void* pThreadPara)
{
    int ret;
    char readVal;
    ROLOGD("RobotTouchFunc");

    (*g_jvm)->AttachCurrentThread(g_jvm, &g_jEnv, NULL);

    while (1) {
        ret = read(gRobotGpioDevId, &readVal, 1);
        if (ret > 0) {
            (*g_jEnv)->CallStaticVoidMethod(g_jEnv, g_jClass, g_jmethodId, readVal);
            usleep(200000);
        }
    }

    (*g_jvm)->DetachCurrentThread(g_jvm);

    pthread_exit(NULL);
}

int RobotGpioInit(JNIEnv *env, jclass cls)
{
    int ret;
    jclass l_clazz = NULL;
    LOGI("RobotGpioInit");

    gRobotGpioDevId = open("/dev/robot_gpio", O_RDWR);
    if (gRobotGpioDevId < 0) {
        LOGI("robot_gpio open failed!!!");
        return ROBOT_GPIO_OPEN_DEV_ERR;
    }

    ret =  (*env)->GetJavaVM(env, &g_jvm);
    if (ret != 0) {
        return ROBOT_GPIO_GET_JAVAVM_ERR;
    }

    l_clazz = (jclass)(*env)->NewGlobalRef(env, cls);
    if (l_clazz == NULL) {
        return ROBOT_GPIO_FIND_CLASS_ERR;
    }

    jmethodID method = (*env)->GetStaticMethodID(env, cls, "callBack_Key", "(I)V");
    if (method == NULL) {
        return ROBOT_GPIO_CALLBACK_ERR;
    }

    g_jClass = l_clazz;
    g_jmethodId = method;

    ret = pthread_create(&gRobotGpioThreadId, NULL, RobotGpioFunc, NULL);
    if (ret != 0) {
        return ROBOT_GPIO_THREAD_CREATE_ERR;
    }

    return 0;
}

int RobotGpioUnInit(JNIEnv *env, jclass cls)
{
    return 0;
}

int RobotGpioSetDev(JNIEnv *env, jclass cls, jbyte* data, jsize dataLen)
{
    int ret;

    if (0 == dataLen) {
        return ROBOT_GPIO_SET_PARAM_ERR;
    }

    ret = write(gRobotGpioDevId, &data[0], 1);
    if (ret != 0) {
        return ROBOT_GPIO_WRITE_DEV_ERR;
    }
    return 0;
}
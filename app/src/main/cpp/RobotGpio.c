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
static jmethodID       g_jKeyMethodId = 0;
static jmethodID       g_jInfraredMethodId = 0;

typedef enum tagROBOT_GPIO_ID_E {
    GPIO_BODY_DETECT = 0,
    GPIO_ENTER_KEY,
    GPIO_INVALID_ID,
}ROBOT_GPIO_ID_E;

void *RobotGpioFunc(void* pThreadPara)
{
    int ret;
    char readVal;
    char gpioId;
    ROLOGD("RobotGpioFunc");

    (*g_jvm)->AttachCurrentThread(g_jvm, &g_jEnv, NULL);

    while (1) {
        ret = read(gRobotGpioDevId, &readVal, 1);
        gpioId = ((readVal >> 4) & 0xF);
        //ROLOGI("readVal = %d, gpioId = %d", readVal, gpioId);
        switch (gpioId) {
            case GPIO_BODY_DETECT:
                (*g_jEnv)->CallStaticVoidMethod(g_jEnv, g_jClass, g_jInfraredMethodId, readVal & 0xF);
                usleep(3000000);
                break;

            case GPIO_ENTER_KEY:
                (*g_jEnv)->CallStaticVoidMethod(g_jEnv, g_jClass, g_jKeyMethodId, readVal & 0xF);
                usleep(200000);
                break;

            default:
                break;
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
        ROLOGI("robot_gpio open failed!!!, ret = %d", gRobotGpioDevId);
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

    jmethodID key_method = (*env)->GetStaticMethodID(env, cls, "callBack_Key", "(I)V");
    if (key_method == NULL) {
        return ROBOT_GPIO_CALLBACK_ERR;
    }

    jmethodID InfraredMethodId = (*env)->GetStaticMethodID(env, cls, "callBack_Infrared", "(I)V");
    if (InfraredMethodId == NULL) {
        return ROBOT_GPIO_CALLBACK_ERR;
    }

    g_jClass = l_clazz;
    g_jKeyMethodId = key_method;
    g_jInfraredMethodId = InfraredMethodId;

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
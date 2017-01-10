//
// Created by zlg on 17-1-10.
//

#include <jni.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "roLog.h"
#include "com_robot_et_drive_RobotDriver.h"

static int gRobotVoiceDevId;
static pthread_t gRobotVoiceThreadId;
RobotDriver_Thread_S ThreadPara;
static JavaVM*			g_jvm = 0;
static JNIEnv*			g_jEnv = 0;
static jclass			g_jClass = 0;
static jmethodID       g_jmethodId = 0;

void* RobotVoiceFunc(void* pThreadPara)
{
    int degree;
    LOGD("RobotVoiceFunc");

    (*g_jvm)->AttachCurrentThread(g_jvm, &g_jEnv, NULL);

    while (1) {
        degree = ioctl(gRobotVoiceDevId, 0, 0);

        (*g_jEnv)->CallStaticVoidMethod(g_jEnv, g_jClass, g_jmethodId, degree);
        usleep(10000);
    }

    (*g_jvm)->DetachCurrentThread(g_jvm);

    pthread_exit(NULL);
}

int RobotVoiceInit(JNIEnv *env, jclass cls)
{
    int ret;
    jclass l_clazz = NULL;
    LOGD("RobotVoiceInit");

    gRobotVoiceDevId = open("/dev/robot_voice", O_RDWR);
    if (gRobotVoiceDevId < 0) {
        printf("robot_voice open failed!!! fd = %d\n", gRobotVoiceDevId);
        return ROBOT_VOICE_OPEN_DEV_ERR;
    }

    ret =  (*env)->GetJavaVM(env, &g_jvm);
    if (ret != 0) {
        return ROBOT_VOICE_GET_JAVAVM_ERR;
    }

    l_clazz = (jclass)(*env)->NewGlobalRef(env, cls);
    if (l_clazz == NULL) {
        return ROBOT_VOICE_FIND_CLASS_ERR;
    }

    jmethodID method = (*env)->GetStaticMethodID(env, cls, "callBackAwakenAngle", "(I)V");
    if (method == NULL) {
        return ROBOT_VOICE_CALLBACK_ERR;
    }

    g_jClass = l_clazz;
    g_jmethodId = method;

    ret = pthread_create(&gRobotVoiceThreadId, NULL, RobotVoiceFunc, NULL);
    if (ret != 0) {
        return ROBOT_VOICE_THREAD_CREATE_ERR;
    }

    return 0;
}

int RobotVoiceUnInit(JNIEnv *env, jclass cls)
{
    return 0;
}

int set_dev_status(JNIEnv *env, jclass cls, jbyteArray data)
{
    int ret;

    jbyte *arrayBody = (*env)->GetByteArrayElements(env, data, 0);
    jsize ArrayLen = (*env)->GetArrayLength(env, data);
    if (0 == ArrayLen) {
        return ROBOT_VOICE_SET_PARAM_ERR;
    }

    ret = ioctl(gRobotVoiceDevId, 1, arrayBody[0]);
    if (ret != 0) {
        return ROBOT_VOICE_SET_GAIN_DIR_ERR;
    }

    return 0;
}
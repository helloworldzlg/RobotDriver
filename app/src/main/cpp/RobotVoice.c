//
// Created by zlg on 17-1-10.
//

#include <jni.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include "roLog.h"
#include "com_robot_et_core_hardware_device_RobotDevice.h"

static int gRobotVoiceDevId;
static pthread_t gRobotVoiceThreadId;
static int g_robotvoice_thread_status = 1;
RobotDevice_Thread_S ThreadPara;
static JavaVM*			g_jvm = 0;
static JNIEnv*			g_jEnv = 0;
static jclass			g_jClass = 0;
static jmethodID       g_jmethodId = 0;

void* RobotVoiceFunc(void* pThreadPara)
{
    int degree;
    LOGI("RobotVoiceFunc");

    (*g_jvm)->AttachCurrentThread(g_jvm, &g_jEnv, NULL);

    while (g_robotvoice_thread_status) {
        degree = ioctl(gRobotVoiceDevId, 0, 0);

        (*g_jEnv)->CallStaticVoidMethod(g_jEnv, g_jClass, g_jmethodId, degree);
    }

    (*g_jvm)->DetachCurrentThread(g_jvm);

    pthread_exit(NULL);
}

int RobotVoiceInit(JNIEnv *env, jclass cls)
{
    int ret;
    jclass l_clazz = NULL;
    LOGI("RobotVoiceInit");

    gRobotVoiceDevId = open("/dev/robot_voice", O_RDWR);
    if (gRobotVoiceDevId < 0) {
        LOGI("robot_voice open failed!!!");
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

    jmethodID method = (*env)->GetStaticMethodID(env, cls, "callBack_AwakenAngle", "(I)V");
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

/* 语音板设备属性设置，此接口当前只用来设置声音的增益方向，后续功能扩展请增加注释修改
 * 参数说明，APP层传下来的data：data[0] 声音增益的方向，当前应该为0
 * */
int RobotVoiceSetDev(JNIEnv *env, jclass cls, jbyte* data, jsize dataLen)
{
    int ret;

    if (0 == dataLen) {
        return ROBOT_VOICE_SET_PARAM_ERR;
    }

    /* param2 对应驱动ioctl接口 cmd: 1 设置声音增益方向 */
    ret = ioctl(gRobotVoiceDevId, 1, data[0]);
    if (ret != 0) {
        return ROBOT_VOICE_SET_GAIN_DIR_ERR;
    }

    return 0;
}
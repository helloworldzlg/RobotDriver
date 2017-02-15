//
// Created by zlg on 17-2-13.
//

#include <jni.h>
#include <fcntl.h>
#include "roLog.h"
#include "com_robot_et_core_hardware_device_RobotDevice.h"

static int gRobotNoisyDetectId;
static pthread_t gRobotNoisyDetectThreadId;
static JavaVM*			g_jvm = 0;
static JNIEnv*			g_jEnv = 0;
static jclass			g_jClass = 0;
static jmethodID       g_jmethodId = 0;
static unsigned int g_NoisyDetectVal = 700;

unsigned int getAverage(unsigned int *pData, int len)
{
    int i;
    unsigned int sum = 0;
    for (i = 0; i < len; i++) {
        sum += pData[i];
    }
    return sum/len;
}

void* RobotNoisyDetectFunc(void* pThreadPara)
{
    int readVal;
    char buff[4];
    unsigned int average;
    static unsigned int count = 0;
    unsigned int data[] = {700, 700, 700, 700, 700};
    int arraySize = sizeof(data)/sizeof(data[0]);

    LOGD("RobotNoisyDetectFunc");

    (*g_jvm)->AttachCurrentThread(g_jvm, &g_jEnv, NULL);

    while (1) {
        readVal = read(gRobotNoisyDetectId, &buff, 4);
        data[count++%arraySize] = readVal;
        average = getAverage(data, arraySize);
        if (average < g_NoisyDetectVal) {
            LOGD("noisy checked!!!");
            (*g_jEnv)->CallStaticVoidMethod(g_jEnv, g_jClass, g_jmethodId, average);
        }
        usleep(500);
    }

    (*g_jvm)->DetachCurrentThread(g_jvm);

    pthread_exit(NULL);
}

int RobotNoisyDetectInit(JNIEnv *env, jclass cls)
{
    int ret;
    jclass l_clazz = NULL;
    LOGD("RobotNoisyDetectInit");

    gRobotNoisyDetectId = open("/dev/robot_adc2", O_RDWR);
    if (gRobotNoisyDetectId < 0) {
        LOGD("adc2 open failed!!!");
        return ROBOT_NOISY_DETECT_OPEN_ERR;
    }

    ret =  (*env)->GetJavaVM(env, &g_jvm);
    if (ret != 0) {
        return ROBOT_NOISY_DETECT_GET_JAVAVM_ERR;
    }

    l_clazz = (jclass)(*env)->NewGlobalRef(env, cls);
    if (l_clazz == NULL) {
        return ROBOT_NOISY_DETECT_FIND_CLASS_ERR;
    }

    jmethodID method = (*env)->GetStaticMethodID(env, cls, "callBack_Noisy", "(I)V");
    if (method == NULL) {
        return ROBOT_NOISY_DETECT_CALLBACK_ERR;
    }

    g_jClass = l_clazz;
    g_jmethodId = method;

    ret = pthread_create(&gRobotNoisyDetectThreadId, NULL, RobotNoisyDetectFunc, NULL);
    if (ret != 0) {
        return ROBOT_NOISY_DETECT_THREAD_CREATE_ERR;
    }

    return 0;
}

int RobotNoisyDetectUnInit(JNIEnv *env, jclass cls)
{
    return 0;
}

int RobotNoisyDetectSet(JNIEnv *env, jclass cls, jbyte* data, jint dataLen)
{
    if (dataLen != 4) {
        return ROBOT_NOISY_DETECT_SET_PARA_ERR;
    }

    g_NoisyDetectVal = ((data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0]);
    return 0;
}
//
// Created by zlg on 17-1-13.
//

#include <jni.h>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include "roLog.h"
#include "com_robot_et_core_hardware_device_RobotDevice.h"

typedef enum {
    ROBOT_LEAR_LIGHT = 0,  /* 左耳朵灯 */
    ROBOT_REAR_LIGHT,      /* 右耳朵灯 */
    ROBOT_BREATH_LIGHT,    /* 胸口呼吸灯 */
    ROBOT_LIGHT_MAX_NUM,
} ROBOT_LIGHT_ID_E;

typedef enum {
    ROBOT_LIGHT_COLOR_RED = 0,
    ROBOT_LIGHT_COLOR_GREEN,
    ROBOT_LIGHT_COLOR_BLUE,
    ROBOT_LIGHT_COLOR_WHITE,
    ROBOT_LIGHT_COLOR_INVALID,
};

#define LIGHT_DEVICE_LED_NUM           (6)

static int gRobotLightId[ROBOT_LIGHT_MAX_NUM];
static pthread_t gRobotLightThreadId;
static unsigned int gRobotLightThreadFlag = 0;
static int gRobotBreathLightColor;

void RobotLightOff(int lightId);

int RobotLightInit(JNIEnv *env, jclass cls)
{
    LOGI("RobotLightInit");

    gRobotLightId[ROBOT_LEAR_LIGHT] = open("/dev/robot_lear_light", O_RDWR);
    if (gRobotLightId[ROBOT_LEAR_LIGHT] < 0) {
        LOGI("Open /dev/robot_lear_light Error!");
        return gRobotLightId[ROBOT_LEAR_LIGHT];
    }

    gRobotLightId[ROBOT_REAR_LIGHT] = open("/dev/robot_rear_light", O_RDWR);
    if (gRobotLightId[ROBOT_REAR_LIGHT] < 0) {
        LOGI("Open /dev/robot_rear_light Error!");
        return gRobotLightId[ROBOT_REAR_LIGHT];
    }

    gRobotLightId[ROBOT_BREATH_LIGHT] = open("/dev/robot_breath_light", O_RDWR);
    if (gRobotLightId[ROBOT_BREATH_LIGHT] < 0) {
        LOGI("Open /dev/robot_breath_light Error!");
        return gRobotLightId[ROBOT_BREATH_LIGHT];
    }

    RobotLightOff((int)ROBOT_LEAR_LIGHT);
    RobotLightOff((int)ROBOT_REAR_LIGHT);
    RobotLightOff((int)ROBOT_BREATH_LIGHT);

    return 0;
}

int RobotLightUnInit(JNIEnv *env, jclass cls)
{
    return 0;
}

void RobotLightOff(int lightId)
{
    int ledIndex;

    for (ledIndex = 0; ledIndex < LIGHT_DEVICE_LED_NUM; ledIndex++) {
        ioctl(gRobotLightId[lightId], 0, ((ledIndex << 12) | (0 << 8) | (0)) );
    }
    return;
}

int RobotLightDevSet(JNIEnv *env, jclass cls, jbyte* data, jsize dataLen)
{
    int ret;
    int ledIndex;
    unsigned char lightId;
    unsigned char lightCmd;
    unsigned char lightVal;
    unsigned char lightColor;

    LOGI("RobotLightDevSet");

    /* byte[0]: 控制灯的ID,参见 ROBOT_LIGHT_ID_E */
    /* byte[1]: 控制灯的命令, 0:熄灭 1:点亮*/
    /* byte[2]: 控制灯的颜色, 0:红色 1:绿色 2:蓝色 3:白色 */
    lightId = data[0];
    lightCmd = data[1];
    lightColor = data[2];

    if ( (lightId >= ROBOT_LIGHT_MAX_NUM) ||
            ((lightCmd != 0) && (lightCmd != 1)) ||
                ((lightColor != 0) && (lightColor != 1) && (lightColor != 2) && (lightColor != 3)) ) {
        return ROBOT_LIGHT_SET_PARAM_ERR;
    }

    lightVal = ((0 == lightCmd) ? (0) : (0x80));

    for (ledIndex = 0; ledIndex < LIGHT_DEVICE_LED_NUM; ledIndex++) {
        ioctl(gRobotLightId[lightId], 0, ((ledIndex << 12) | (lightColor << 8) | (lightVal)) );
    }

    return 0;
}

void *RobotLightFunc(void* pThreadPara)
{
    int i,j;
    int lightVal;
    int arraySize;
    char data[] = {0x0, 0x20, 0x40, 0x60, 0x80, 0xA0, 0x80, 0x60, 0x40, 0x20, 0x0};
    arraySize = sizeof(data)/sizeof(data[0]);

    while (gRobotLightThreadFlag) {
        for (i = 0; i < arraySize; i++) {
            for (j = 0; j < LIGHT_DEVICE_LED_NUM; j++) {
                lightVal = ((j << 12) | (gRobotBreathLightColor << 8) | (data[i]));

                ioctl(gRobotLightId[ROBOT_BREATH_LIGHT], 0, lightVal);
            }
            usleep(150000);

            /* 保证及时退出 */
            if (!gRobotLightThreadFlag) {
                break;
            }
        }
    }

    pthread_exit(NULL);
}

JNIEXPORT jint JNICALL Java_com_robot_et_core_hardware_device_RobotDevice_setLightStatusStart
        (JNIEnv *env, jclass cls, jint lightColor)
{
    int ret;

    if (lightColor >= ROBOT_LIGHT_COLOR_INVALID) {
        return ROBOT_LIGHT_BREATH_COLOR_ERR;
    }

    /* 保证此接口可重入 */
    if (gRobotLightThreadFlag) {
        return 0;
    }

    gRobotBreathLightColor = lightColor;
    gRobotLightThreadFlag = 1;
    ret = pthread_create(&gRobotLightThreadId, NULL, RobotLightFunc, NULL);
    if (ret != 0) {
        return ROBOT_LIGHT_THREAD_CREATE_ERR;
    }

    return 0;
}

JNIEXPORT jint JNICALL Java_com_robot_et_core_hardware_device_RobotDevice_setLightStatusEnd
        (JNIEnv *env, jclass cls)
{
    /* 保证此接口可重入 */
    if (!gRobotLightThreadFlag) {
        return 0;
    }

    gRobotLightThreadFlag = 0;

    return 0;
}
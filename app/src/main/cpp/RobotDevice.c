//
// Created by zlg on 17-1-5.
//

#include <stdint.h>
#include "com_robot_et_core_hardware_device_RobotDevice.h"

#define DEVICE_LOCAL_BUFFER_SIZE             (1024)

/*
 * 此表格中的顺序不要随便调整，调整需要通知APP侧dev Id的变化，否则会出错
 * */
Robot_Device_Func_S gRobotDeviceFunc[] = {
        {ROBOT_VOICE_DEV, RobotVoiceInit, RobotVoiceUnInit, RobotVoiceSetDev},
        {ROBOT_TOUCH_DEV, RobotTouchInit, RobotTouchUnInit, NULL},
        {ROBOT_LIGHT_DEV, RobotLightInit, RobotLightUnInit, RobotLightDevSet},
        {ROBOT_SERIALPORT_DEV, RobotSerialPortInit, RobotSerialPortUnInit, RobotSerialPortSet},
        {ROBOT_GPIO_DEV, RobotGpioInit, RobotGpioUnInit, RobotGpioSetDev},
        {ROBOT_NOISY_DETECT_DEV, RobotNoisyDetectInit, RobotNoisyDetectUnInit},
};

JNIEXPORT jint JNICALL Java_com_robot_et_core_hardware_device_RobotDevice_initDevice
        (JNIEnv *env, jclass cls)
{
    int i;
    int ret;
    int array_size = sizeof(gRobotDeviceFunc)/sizeof(Robot_Device_Func_S);

    for (i = 0; i < array_size; i++) {
        if (gRobotDeviceFunc[i].dev_init != NULL) {
            ret = gRobotDeviceFunc[i].dev_init(env, cls);
            if (ret != 0) {
                LOGI("dev_init err!!!");
            }
        }
    }
    return 0;
}

JNIEXPORT jint JNICALL Java_com_robot_et_core_hardware_device_RobotDevice_unInitDevice
        (JNIEnv *env, jclass cls)
{
    int i;
    int ret;
    int array_size = sizeof(gRobotDeviceFunc)/sizeof(Robot_Device_Func_S);

    for (i = 0; i < array_size; i++) {
        if (gRobotDeviceFunc[i].dev_unInit != NULL) {
            ret = gRobotDeviceFunc[i].dev_unInit(env, cls);
            if (ret != 0) {
                LOGD("dev_unInit err!!!");
            }
        }
    }
    return 0;
}

JNIEXPORT jint JNICALL Java_com_robot_et_core_hardware_device_RobotDevice_setDeviceStatus
        (JNIEnv *env, jclass cls, jint devId, jbyteArray data)
{
    int ret;
    unsigned int dataLen;
    unsigned char buffer[DEVICE_LOCAL_BUFFER_SIZE];

    LOGI("setDeviceStatus");

    if ((data == NULL) || (devId >= INVALID_ROBOT_DEV_ID)) {
        LOGI("robot device input data == null");
        return ROBOT_DEVICE_INPUT_PARAM_ERR;
    }

    memset(buffer, 0, sizeof(buffer));
    dataLen = (*env)->GetArrayLength(env, data);
    (*env)->GetByteArrayRegion(env, data, 0, dataLen, buffer);

    if (dataLen > DEVICE_LOCAL_BUFFER_SIZE) {
        LOGI("robot device local buffer len err");
        return ROBOT_DEVICE_LOCAL_BUFFER_LEN_ERR;
    }

    if (gRobotDeviceFunc[devId].set_dev_status != NULL) {
        ret = gRobotDeviceFunc[devId].set_dev_status(env, cls, buffer, dataLen);
        if (ret != 0) {
            LOGI("set_dev_status error");
            return ret;
        }
    }

    return 0;
}
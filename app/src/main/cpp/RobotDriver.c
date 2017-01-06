//
// Created by zlg on 17-1-5.
//

#include <stdint.h>
#include "com_robot_et_drive_RobotDriver.h"

Robot_Driver_Func_S gRobotDriverFunc[] = {
        {RobotTouchInit, RobotTouchUnInit, NULL},
};

JNIEXPORT jint JNICALL Java_com_robot_et_drive_RobotDriver_initDrive
        (JNIEnv *env, jclass cls)
{
    int i;
    int ret;
    int array_size = sizeof(gRobotDriverFunc)/sizeof(Robot_Driver_Func_S);

    for (i = 0; i < array_size; i++) {
        if (gRobotDriverFunc[i].dev_init != NULL) {
            ret = gRobotDriverFunc[i].dev_init(env, cls);
            if (ret != 0) {
                LOGD("dev_init err");
            }
        }
    }
    return 0;
}

JNIEXPORT jint JNICALL Java_com_robot_et_drive_RobotDriver_unInitDrive
        (JNIEnv *env, jclass cls)
{
    int i;
    int ret;
    int array_size = sizeof(gRobotDriverFunc)/sizeof(Robot_Driver_Func_S);

    for (i = 0; i < array_size; i++) {
        if (gRobotDriverFunc[i].dev_unInit != NULL) {
            ret = gRobotDriverFunc[i].dev_unInit(env, cls);
            if (ret != 0) {
                LOGD("dev_init err");
            }
        }
    }
    return 0;
}

JNIEXPORT jint JNICALL Java_com_robot_et_drive_RobotDriver_setDeviceStatus
        (JNIEnv *env, jclass cls, jint devId, jbyteArray data)
{
    int i;
    int array_size = sizeof(gRobotDriverFunc)/sizeof(Robot_Driver_Func_S);

    for (i = 0; i < array_size; i++) {
        if (gRobotDriverFunc[i].set_dev_status != NULL) {
            gRobotDriverFunc[i].set_dev_status(env, cls, data);
        }
    }
    return 0;
}

JNIEXPORT jint JNICALL Java_com_robot_et_drive_RobotDriver_setLightStatusStart
        (JNIEnv *env, jclass cls, jint lightColor)
{
    return 0;
}

JNIEXPORT jint JNICALL Java_com_robot_et_drive_RobotDriver_setLightStatusEnd
        (JNIEnv *env, jclass cls)
{
    return 0;
}
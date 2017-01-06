/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
#include "roLog.h"

/* Header for class com_robot_et_drive_RobotDriver */

#ifndef _Included_com_robot_et_drive_RobotDriver
#define _Included_com_robot_et_drive_RobotDriver
#ifdef __cplusplus
extern "C" {
#endif

typedef struct tagRobot_Driver_Func_S {
    int (* dev_init)(JNIEnv *env, jclass cls);
    int (* dev_unInit)(JNIEnv *env, jclass cls);
    int (* set_dev_status)(JNIEnv *env, jclass cls, jbyteArray data);
}Robot_Driver_Func_S;

typedef struct tagRobotDriver_Thread_S {
    JNIEnv *env;
    jclass cls;
    jmethodID method;
} RobotDriver_Thread_S;

typedef enum tagRobot_Driver_ErrId_E {
    ROBOT_TOUCH_OPEN_DEV_ERR = 0x10,
    ROBOT_TOUCH_GET_JAVAVM_ERR,
    ROBOT_TOUCH_FIND_CLASS_ERR,
    ROBOT_TOUCH_CALLBACK_ERR,
    ROBOT_TOUCH_THREAD_CREATE_ERR,
}Robot_Driver_ErrId_E;

/*
 * Class:     com_robot_et_drive_RobotDriver
 * Method:    initDrive
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_robot_et_drive_RobotDriver_initDrive
  (JNIEnv *, jclass);

/*
 * Class:     com_robot_et_drive_RobotDriver
 * Method:    unInitDrive
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_robot_et_drive_RobotDriver_unInitDrive
  (JNIEnv *, jclass);

/*
 * Class:     com_robot_et_drive_RobotDriver
 * Method:    setDeviceStatus
 * Signature: (I[B)I
 */
JNIEXPORT jint JNICALL Java_com_robot_et_drive_RobotDriver_setDeviceStatus
  (JNIEnv *, jclass, jint, jbyteArray);

/*
 * Class:     com_robot_et_drive_RobotDriver
 * Method:    setLightStatusStart
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_robot_et_drive_RobotDriver_setLightStatusStart
  (JNIEnv *, jclass, jint);

/*
 * Class:     com_robot_et_drive_RobotDriver
 * Method:    setLightStatusEnd
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_robot_et_drive_RobotDriver_setLightStatusEnd
  (JNIEnv *, jclass);

extern int RobotTouchInit(JNIEnv *env, jclass cls);
extern int RobotTouchUnInit(JNIEnv *env, jclass cls);

#ifdef __cplusplus
}
#endif
#endif

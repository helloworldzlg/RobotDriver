//
// Created by zlg on 17-1-16.
//

#include <jni.h>
#include <fcntl.h>
#include "roLog.h"
#include <termios.h>
#include "com_robot_et_core_hardware_device_RobotDevice.h"

static int gRobotSerialPortId;
static pthread_t gRobotSerialPortThreadId;

typedef struct tagROBOT_SERIALPORT_FRAME_S {
    unsigned char sync_flag;
    unsigned char data_len;
    unsigned char protocol_mode;
    unsigned char request_cmd;
    unsigned char joint_id;
    unsigned short rotation_degree;
    unsigned short plan_time;
    unsigned char check_code;
}ROBOT_SERIALPORT_FRAME_S;

void *RobotSerialPortFunc(void* pThreadPara)
{
    int ret;
    char buf[1024];

    while (1) {
        memset(buf, '/0', sizeof(buf));
        ret = read(gRobotSerialPortId, &buf, sizeof(buf));
        if (ret < 0) {
            LOGI("serial_port read fail!!!");
            return ROBOT_SERIALPORT_READ_ERR;
        }
    }

    pthread_exit(NULL);
}

int RobotSerialPortInit(JNIEnv *env, jclass cls)
{
    int ret;
    struct termios  oldtio, newtio;

    gRobotSerialPortId = open("/dev/ttyS3", O_RDWR);
    if (gRobotSerialPortId < 0) {
        LOGI("open /dev/ttyS3 error!!!");
        return ROBOT_SERIALPORT_OPEN_ERR;
    }

    /* 获取当前操作模式参数 */
    tcgetattr(gRobotSerialPortId, &oldtio);
    memset(&newtio, 0, sizeof(newtio));

    /* 波特率=115200 数据位=8 使能数据接收 */
    newtio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;

    tcflush(gRobotSerialPortId, TCIFLUSH);
    tcsetattr(gRobotSerialPortId, TCSANOW, &newtio);

    ret = pthread_create(&gRobotSerialPortThreadId, NULL, RobotSerialPortFunc, NULL);
    if (ret != 0) {
        return ROBOT_SERIALPORT_THREAD_CREATE_ERR;
    }

    return 0;
}

int RobotSerialPortUnInit(JNIEnv *env, jclass cls)
{
    return 0;
}

int RobotSerialPortSet(JNIEnv *env, jclass cls, jbyte* data, jint dataLen)
{
    int ret;
    ROBOT_SERIALPORT_FRAME_S mSerialFrame;

    if ((0 == dataLen) || (dataLen > 5)) {
        return ROBOT_SERIALPORT_SET_PARAM_ERR;
    }

    memset(&mSerialFrame, 0, sizeof(mSerialFrame));
    /* 按照协议格式固定填写 */
    mSerialFrame.sync_flag     = 0x10;
    mSerialFrame.data_len      = 0x07;
    mSerialFrame.protocol_mode = 0xF9;
    mSerialFrame.request_cmd   = 0x30;

    /* 从APP侧获取 */
    mSerialFrame.joint_id        = data[0];
    mSerialFrame.rotation_degree = ((data[2] << 8) | data[1]);
    mSerialFrame.plan_time       = ((data[4] << 8) | data[3]);

    ret = write(gRobotSerialPortId, (void*)&mSerialFrame, dataLen);
    if (ret < 0) {
        LOGI("serial_port write fail!!!");
        return ROBOT_SERIALPORT_WRITE_ERR;
    }
    return 0;
}
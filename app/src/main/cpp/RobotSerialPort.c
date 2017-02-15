//
// Created by zlg on 17-1-16.
//

#include <jni.h>
#include <fcntl.h>
#include "roLog.h"
#include <termios.h>
#include <pthread.h>
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
    int i;
    unsigned char mCheckCode = 0;
    unsigned char mTxData[64]; /* 注意下面的参数检查 */
    unsigned char count = 0;

    ROLOGI("dataLen = %d", dataLen);

    if ((0 == dataLen) || (dataLen > 61)) {
        return ROBOT_SERIALPORT_SET_PARAM_ERR;
    }

    memset(&mTxData, 0, sizeof(mTxData));

    /* 按照协议格式固定填写 */
    mTxData[count++] = 0x10;  /* sync_flag */
    mTxData[count++] = 0x1 + dataLen;  /* data_len */
    mTxData[count++] = 0xF9;  /* protocol_mode */

    /* 从APP侧获取 */
    for (i = 0; i < dataLen; i++) {
        mTxData[count++] = data[i];
    }

    for (i = 0; i < count; i++) {
        mCheckCode ^= mTxData[i];
    }

    mTxData[count++] = mCheckCode;

    ret = write(gRobotSerialPortId, (void*)&mTxData, count);
    if (ret < 0) {
        LOGI("serial_port write fail!!!");
        return ROBOT_SERIALPORT_WRITE_ERR;
    }
    LOGI("serial_port write finished!!!");
    return 0;
}
package com.robot.et.drive;

/**
 * Created by zlg on 17-1-5.
 */

public class RobotDriver {
    static {
        System.loadLibrary("RobotDriver");
    }

//    public static void setDriveCallBack(DriveCallBack callBack) {
//
//    }

    /**
     * 唤醒角度的回调
     * @param awakenAngle 唤醒角度
     */
    public static void callBackAwakenAngle(int awakenAngle) {
        return;
    }

    /**
     * 触摸的回调
     * @param touchId 触摸ID
     */
    public static void callBackTouch(int touchId) {
        System.out.println("touchid = "+touchId);
        return;
    }

    /**
     * 按键的回调
     * @param keyId 按键ID
     */
    public static void callBackKey(int keyId) {
        return;
    }

    /**
     * 红外感应的回调
     * @param infraredValue 红外值
     */
    public static void callBackInfrared(int infraredValue) {
        return;
    }

    /**
     * 雷达数据的回调
     * @param radarData 雷达数据
     */
    public static void callBackRadar(byte[] radarData) {
        return;
    }

    // 初始化驱动
    public static native int initDrive();

    // 销毁驱动
    public static native int unInitDrive();

    /**
     * 设置设备的状态
     * @param deviceId 设备ID
     * @param data 控制设备的数据
     * @return
     */
    public static native int setDeviceStatus(int deviceId, byte[] data);

    /**
     * 用于持续控制的开始
     * @param lightColor
     * @return
     */
    public static native int setLightStatusStart(int lightColor);

    /**
     * 用于持续控制的结束
     * @return
     */
    public static native int setLightStatusEnd();
}

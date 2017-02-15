package com.robot.et.core.hardware.device;

import android.util.Log;
//import android.os.IDeviceCallBack;

public class RobotDevice {
	private static final String TAG = "RobotDevice";
	//private static IDeviceCallBack mCallBack;
	
	static {
		System.loadLibrary("RobotDevice");
	}
	
//	public static void setDeviceCallBack(IDeviceCallBack callBack) {
//        mCallBack = callBack;
//    }
    
    public static void callBack_AwakenAngle(int awakenAngle) {
        Log.i(TAG, "callBackAwakenAngle " + awakenAngle);
//        try {
//			if (mCallBack != null)
//				mCallBack.callBackAwakenAngle(awakenAngle);
//		} catch(Exception e) {
//			Log.i(TAG, "Exception===" + e);
//		}
    }
    
    public static void callBack_Touch(int touchId) {
        Log.i(TAG, "callBackTouch " + touchId);
//        try {
//			if (mCallBack != null)
//				mCallBack.callBackTouch(touchId);
//		} catch(Exception e) {
//			Log.i(TAG, "Exception===" + e);
//		}
    }
    
    public static void callBack_Key(int keyId) {
        Log.i(TAG, "callBackKey");
//        try {
//			if (mCallBack != null)
//				mCallBack.callBackKey(keyId);
//		} catch(Exception e) {
//			Log.i(TAG, "Exception===" + e);
//		}
    }
    
    public static void callBack_Infrared(int infraredValue) {
        Log.i(TAG, "callBackInfrared");
//        try {
//			if (mCallBack != null)
//				mCallBack.callBackInfrared(infraredValue);
//		} catch(Exception e) {
//			Log.i(TAG, "Exception===" + e);
//		}
    }   
    
    public static void callBack_Radar(byte[] radarData) {
        Log.i(TAG, "callBackRadar");
//        try {
//			if (mCallBack != null)
//				mCallBack.callBackRadar(radarData);
//		} catch(Exception e) {
//			Log.i(TAG, "Exception===" + e);
//		}
    }

    public static void callBack_Noisy(int volume) {
        Log.i(TAG, "callBackNoisy");
//        try {
//            if (mCallBack != null)
//                mCallBack.callBackNoisy(volume);
//        } catch(Exception e) {
//            Log.i(TAG, "Exception===" + e);
//        }
    }
    
    public static native int initDevice();
    
    public static native int unInitDevice();
    
    public static native int setDeviceStatus(int deviceId, byte[] data);
    
    public static native int setLightStatusStart(int lightColor);
    
    public static native int setLightStatusEnd();
}

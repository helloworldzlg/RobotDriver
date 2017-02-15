package com.robot.et.visiontest;

import android.os.IVisionCallBack;
import android.graphics.Bitmap;

/**
 * Created by houdeming on 2017/1/10.
 */

public class VisionListener extends IVisionCallBack.Stub {
    /**
     * 视觉学习的摄像头打开结束
     */
    @Override 
    public void learnOpenEnd() {};

    /**
     * 视觉学习发出的警告
     *
     * @param id 警告id
     */
    @Override 
    public void learnWaring(int id) {};

    /**
     * 学习结束
     */
    @Override 
    public void learnEnd() {};

    /**
     * 学习识别结束
     *
     * @param name 物体名字
     * @param conf 识别的确信度
     */
    @Override 
    public void learnRecogniseEnd(String name, int conf) {};

    /**
     * 人体的位置
     *
     * @param centerX X坐标
     * @param centerY Y坐标
     * @param centerZ Z坐标
     */
    @Override 
    public void bodyPosition(float centerX, float centerY, float centerZ) {};

    /**
     * 获取视觉图像的信息
     *
     * @param width    宽
     * @param height   高
     * @param dataType 格式
     */
    @Override 
    public void getVisionImgInfo(int width, int height, int dataType) {};

    /**
     * 获取bitmap
     *
     * @param bitmap
     */
    @Override
    public void getImgBitmap(byte[] datas) { };

    /**
     * 获取视觉反馈的光线
     *
     * @param imgAnalyId
     * @param isSuccess
     */
    @Override 
    public void imgAnaly(int imgAnalyId, boolean isSuccess) {};

    /**
     * 获取进度
     * @param percent
     */
    @Override 
    public void getVisionProgress(int percent) {};

    /**
     * 人脸检测
     * @param
     */
    @Override 
    public void onFacePos(float centerX, float centerY, float centerZ) {};
}

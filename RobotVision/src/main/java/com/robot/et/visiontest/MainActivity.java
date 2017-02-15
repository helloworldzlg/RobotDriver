package com.robot.et.visiontest;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.IRobotVision;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.os.Handler;
import android.os.Message;

import java.io.ByteArrayOutputStream;
import java.util.Timer;
import java.util.TimerTask;

public class MainActivity extends Activity implements View.OnClickListener {

    private static final String TAG = "visionHand";
    private IRobotVision mDeviceManager;
    private ImageView vbitmap;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button voiceBtn = (Button) findViewById(R.id.btn_openlearn);
        Button learnBtn = (Button) findViewById(R.id.btn_learn);
        Button closeBtn = (Button) findViewById(R.id.btn_learnclose);


        ((Button) findViewById(R.id.btn_openbody)).setOnClickListener(this);
        ((Button) findViewById(R.id.btn_closebody)).setOnClickListener(this);
        ((Button) findViewById(R.id.btn_getbody)).setOnClickListener(this);

        ((Button) findViewById(R.id.btn_openface)).setOnClickListener(this);
        ((Button) findViewById(R.id.btn_closeface)).setOnClickListener(this);
        ((Button) findViewById(R.id.btn_getface)).setOnClickListener(this);


        ((Button) findViewById(R.id.btn_getimginfo)).setOnClickListener(this);
        ((Button) findViewById(R.id.btn_getbitmap)).setOnClickListener(this);


        voiceBtn.setOnClickListener(this);
        learnBtn.setOnClickListener(this);
        closeBtn.setOnClickListener(this);


        vbitmap = (ImageView) findViewById(R.id.vbitmap);

        mDeviceManager = IRobotVision.Stub.asInterface(ServiceManager.getService(Context.VISION_SERVICE));

        try {
            mDeviceManager.registerListener(visionCallBack);
            int ret = mDeviceManager.visionInit();
            Log.i(TAG, "mDeviceManager.visionInit ret = " + ret);
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        Log.i(TAG, "come out onCreate() ");

    }

    private Timer imgTimer;

    private void getImg() {
        imgTimer = new Timer();
        imgTimer.schedule(new TimerTask() {
            @Override
            public void run() {
                handler.sendEmptyMessage(1);
            }
        }, 0, 80);
    }

    private Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            switch (msg.what) {
                case 1:
                    try {
                        mDeviceManager.getVisionImgBitmap(1);
                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }
                    break;
            }
        }
    };


    @Override
    public void onClick(View view) {
        switch (view.getId()) {

            case R.id.btn_openlearn:
                try {
                    mDeviceManager.visionLearnOpen();
                } catch (RemoteException e) {
                    Log.i(TAG, "openLearn() RemoteException");
                }
                break;

            case R.id.btn_learn:
                try {
                    mDeviceManager.objLearnStartLearn("苹果");
                } catch (RemoteException e) {
                    Log.i(TAG, "RemoteException");
                }
                break;

            case R.id.btn_learnclose:
                try {
                    mDeviceManager.visionLearnClose();
                } catch (RemoteException e) {
                    Log.i(TAG, "RemoteException");
                }
                break;

            case R.id.btn_openbody:
                try {
                    mDeviceManager.bodyDetectOpen();
                } catch (RemoteException e) {
                    Log.i(TAG, "RemoteException");
                }
                break;

            case R.id.btn_closebody:
                try {
                    mDeviceManager.bodyDetectClose();
                } catch (RemoteException e) {
                    Log.i(TAG, "RemoteException");
                }
                break;

            case R.id.btn_getbody:
                try {
                    mDeviceManager.bodyDetectGetPos();
                } catch (RemoteException e) {
                    Log.i(TAG, "RemoteException");
                }
                break;

            case R.id.btn_openface:
                try {
                    mDeviceManager.faceDetectOpen();
                } catch (RemoteException e) {
                    Log.i(TAG, "RemoteException");
                }
                break;

            case R.id.btn_closeface:
                try {
                    mDeviceManager.faceDetectClose();
                } catch (RemoteException e) {
                    Log.i(TAG, "RemoteException");
                }
                break;

            case R.id.btn_getface:
                try {
                    mDeviceManager.faceDetectGetPos();
                } catch (RemoteException e) {
                    Log.i(TAG, "RemoteException");
                }
                break;


            case R.id.btn_getimginfo:
                Log.i(TAG, "get img info button");
                try {
                    mDeviceManager.getVisionImgInfo();
                } catch (RemoteException e) {
                    Log.i(TAG, "RemoteException");
                }
                break;

            case R.id.btn_getbitmap:
                Log.i(TAG, "get bit map button");
//                try {
//                    mDeviceManager.getVisionImgBitmap(0);
//                } catch (RemoteException e) {
//                    Log.i(TAG, "RemoteException");
//                }
                getImg();

                break;

            case R.id.btn_init:
                try {
                    int ret = mDeviceManager.visionInit();
                    Log.i(TAG, "mDeviceManager.visionInit ret = " + ret);
                } catch (RemoteException e) {
                    Log.i(TAG, "RemoteException");
                }
                break;

            case R.id.btn_uninit:
                try {
                    mDeviceManager.visionUninit();
                    Log.i(TAG, "mDeviceManager visionUninit");
                } catch (RemoteException e) {
                    Log.i(TAG, "RemoteException");
                }
                break;

        }
    }

    private VisionListener visionCallBack = new VisionListener() {
        @Override
        public void learnOpenEnd() {
            Log.i(TAG, "callback  learnOpenEnd()");
        }

        @Override
        public void learnWaring(int id) {
            Log.i(TAG, "callback  learnWaring() id==" + id);
        }

        @Override
        public void learnEnd() {
            Log.i(TAG, "callback  learnEnd()");
        }

        @Override
        public void learnRecogniseEnd(String name, int conf) {
            Log.i(TAG, "callback  learnRecogniseEnd() name = " + name + ", conf = " + conf);
        }

        @Override
        public void bodyPosition(float centerX, float centerY, float centerZ) {
            Log.i(TAG, "callback  bodyPosition() centerX = " + centerX + ", centerY = " + centerY + ", centerZ = " + centerZ);
        }

        @Override
        public void getVisionImgInfo(int width, int height, int dataType) {
            Log.i(TAG, "callback  getVisionImgInfo() width = " + width + ", height = " + height + ", dataType = " + dataType);

            if (width != 0 && height != 0) {
                Bitmap mBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
                ByteArrayOutputStream baos = new ByteArrayOutputStream();
                mBitmap.compress(Bitmap.CompressFormat.PNG, 100, baos);
                byte[] datas = baos.toByteArray();
                
				try {
					mDeviceManager.setVisionImgByte(datas);
					Log.i(TAG, "mDeviceManager setVisionImgByte");
				} catch (RemoteException e) {
					Log.i(TAG, "RemoteException");
				}
                
//                int length =  baos.toByteArray().length;
//                byte[] Abitmap = new byte[length];

                Log.i(TAG, "callback  getVisionImgInfo setImgBitmap() datas.length = " + datas.length );
            }
        }

        @Override
        public void imgAnaly(int imgAnalyId, boolean isSuccess) {
            Log.i(TAG, "callback  imgAnaly() imgAnalyId = " + imgAnalyId + ", isSuccess = " + isSuccess);
        }

        @Override
        public void getVisionProgress(int percent) {
            Log.i(TAG, "callback  getVisionProgress() percent = " + percent);
        }

        @Override
        public void onFacePos(float centerX, float centerY, float centerZ) {
            Log.i(TAG, "callback  onFacePos() centerX = " + centerX + ", centerY = " + centerY + ", centerZ = " + centerZ);
        }

        @Override
        public void getImgBitmap(byte[] datas) {
            Log.i(TAG, "callback  getImgBitmap");
            if (datas == null) {
                Log.i(TAG, "getImgBitmap = null ");
                return;
            }

            Bitmap bitmap = null;
            int length = datas.length;
            Log.i(TAG, "callback  length==" + length);
            if (datas != null && length != 0) {
                bitmap = BitmapFactory.decodeByteArray(datas, 0, length);
                Log.i(TAG, "callback  getImgBitmap  2 ");
            }

            vbitmap.setImageBitmap(bitmap);
            Log.i(TAG, "callback  getImgBitmap  3 ");
        }

    };


}

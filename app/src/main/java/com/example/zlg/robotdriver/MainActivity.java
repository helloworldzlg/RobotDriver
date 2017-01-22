package com.example.zlg.robotdriver;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import com.robot.et.core.hardware.device.RobotDevice;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        int ret;

//        byte[] data = new byte[3];
//        data[0] = 1;
//        data[1] = 1;
//        data[2] = 2;
//        ret = RobotDevice.initDevice();
//        System.out.printf("initDevice ret = %d\n", ret);
//        ret = RobotDevice.setDeviceStatus(2, data);
//        System.out.printf("setDeviceStatus ret = %d\n", ret);
//
//        ret = RobotDevice.setLightStatusStart(2);
//        System.out.printf("setLightStatusStart ret = %d\n", ret);
//
//        ret = RobotDevice.setLightStatusEnd();
//        System.out.printf("setLightStatusEnd ret = %d\n", ret);

        byte[] data = new byte[5];
        data[0] = 1;
        data[1] = 1;
        data[2] = 2;
        data[3] = 2;
        data[4] = 2;
        ret = RobotDevice.initDevice();
        System.out.printf("initDevice ret = %d\n", ret);
        ret = RobotDevice.setDeviceStatus(3, data);
        System.out.printf("setDeviceStatus ret = %d\n", ret);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}

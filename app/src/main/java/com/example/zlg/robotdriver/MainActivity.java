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

        byte[] data = new byte[10];
        data[0] = 0x34;
        data[1] = 0x35;
        data[2] = 0x36;
        data[3] = 0x37;
        data[4] = 0x38;
        data[5] = 0x39;
        data[6] = 0x40;
        data[7] = 0x41;
        data[8] = 0x42;
        data[9] = 0x43;

        ret = RobotDevice.initDevice();
        System.out.printf("initDevice ret = %d\n", ret);

        //ret = RobotDevice.setDeviceStatus(3, data);
        //System.out.printf("setDeviceStatus ret = %d\n", ret);

        //ret = RobotDevice.setLightStatusStart(2);
        //System.out.printf("setLightStatusStart ret = %d\n", ret);

        //ret = RobotDevice.setLightStatusEnd();
        //System.out.printf("setLightStatusEnd ret = %d\n", ret);

//        byte[] data = new byte[5];
//        data[0] = 1;
//        data[1] = 1;
//        data[2] = 2;
//        data[3] = 2;
//        data[4] = 2;
//        ret = RobotDevice.initDevice();
//        System.out.printf("initDevice ret = %d\n", ret);
//        ret = RobotDevice.setDeviceStatus(3, data);
//        System.out.printf("setDeviceStatus ret = %d\n", ret);

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

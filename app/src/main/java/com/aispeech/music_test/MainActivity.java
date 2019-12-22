package com.aispeech.music_test;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;

import com.hjq.permissions.OnPermission;
import com.hjq.permissions.XXPermissions;

import java.util.List;


public class MainActivity extends AppCompatActivity {

    private static final String TAG = MainActivity.class.getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);


    }

    @Override
    protected void onResume() {
        super.onResume();
        hasPermissions();
    }

    public static final String[] mpermissions = new String[]{
            "android.permission.READ_EXTERNAL_STORAGE",
            "android.permission.WRITE_EXTERNAL_STORAGE",
            "android.permission.RECORD_AUDIO",
            "android.permission.ACCESS_NETWORK_STATE",
            "android.permission.INTERNET",
            "android.permission.ACCESS_WIFI_STATE",
    };

    private void hasPermissions() {
        Log.d(TAG,"hasPermissions()");
        if (XXPermissions.isHasPermission(MainActivity.this,mpermissions)) {
            jumpMusicPlayActivity();
        }else {
            goPermissions();
        }
    }

    private void jumpMusicPlayActivity(){
        MusicPlayerActivity.jumpMusicPlayerActivity(this);
    }

    private void goPermissions() {
        Log.d(TAG,"goPermissions()");
        XXPermissions.with(this)
                .constantRequest()
                .permission(mpermissions)
                .request(new OnPermission() {
                    @Override
                    public void hasPermission(List<String> granted, boolean isAll) {
                        Log.d(TAG,"hasPermissions() isAll:"+isAll);
                        if (isAll){
                            jumpMusicPlayActivity();
                        }
                    }

                    @Override
                    public void noPermission(List<String> denied, boolean quick) {
                        Log.d(TAG,"noPermission() quick:"+quick);
                        if (quick)
                            XXPermissions.gotoPermissionSettings(MainActivity.this);
                        else
                            finish();
                    }
                });
    }
}

package com.aispeech.music_test;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.TextView;

import com.aispeech.ffmpeg_lib.FfmpegUtil;


public class MainActivity extends AppCompatActivity {

    FfmpegUtil mFfmpegUtil = new FfmpegUtil();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(mFfmpegUtil.stringFromJNI());
    }


}

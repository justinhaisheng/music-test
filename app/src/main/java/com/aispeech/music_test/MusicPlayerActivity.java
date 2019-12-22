package com.aispeech.music_test;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;

import com.aispeech.audio.AudioCore;

public class MusicPlayerActivity extends AppCompatActivity {

    private static final String TAG = MusicPlayerActivity.class.getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_music_player);

    }

    public static void jumpMusicPlayerActivity(Context context){
        Intent intent = new Intent(context,MusicPlayerActivity.class);
        context.startActivity(intent);
    }

    @Override
    protected void onResume() {
        super.onResume();
        AudioCore.getInstance().setAudioCallback(new AudioCore.AudioCallback() {
            @Override
            public void onPrepare() {
                Log.d(TAG,"onPrepare()");
            }

            @Override
            public void onStart() {
                Log.d(TAG,"onStart()");
            }

            @Override
            public void onProgress(float pro) {
                Log.d(TAG,"onProgress() pro:"+pro);
            }

            @Override
            public void onComplete(boolean isCom) {
                Log.d(TAG,"onComplete() isCom:"+isCom);
            }
        });
    }

    @Override
    protected void onStop() {
        super.onStop();
        AudioCore.getInstance().setAudioCallback(null);
    }

    public void start(View view){
        new Thread(new Runnable() {
            @Override
            public void run() {
                AudioCore.getInstance().start(MusicConfig.MUSIC_2);
            }
        }).start();

    }

    public void stop(View view){

    }
}

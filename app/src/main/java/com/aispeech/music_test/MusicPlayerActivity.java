package com.aispeech.music_test;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.aispeech.audio.AudioCore;

public class MusicPlayerActivity extends AppCompatActivity {

    private static final String TAG = MusicPlayerActivity.class.getSimpleName();
    //private WlPlayer mWlPlayer;
    private TextView tvTime;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_music_player);
        tvTime = findViewById(R.id.tv_time);
//        mWlPlayer = new WlPlayer();
//        mWlPlayer.setSource(MusicConfig.MUSIC_2);
//        mWlPlayer.setWlOnParparedListener(new WlOnParparedListener() {
//            @Override
//            public void onParpared() {
//                mWlPlayer.start();
//            }
//        });
    }

    public static void jumpMusicPlayerActivity(Context context){
        Intent intent = new Intent(context,MusicPlayerActivity.class);
        context.startActivity(intent);
    }

    @Override
    protected void onResume() {
        super.onResume();
        AudioCore.getInstance().setTimeCallback(new AudioCore.TimeCallback() {
            @Override
            public void onTimeback(final int currentTime, final int totalTime) {
                Log.d(TAG,"timeback() currentTime:"+currentTime+" totalTime:"+totalTime);
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        tvTime.setText(TimeUtil.secdsToDateFormat(currentTime,0)+"/"+TimeUtil.secdsToDateFormat(totalTime,0));
                    };
                });
            }
        });
        AudioCore.getInstance().setAudioCallback(new AudioCore.AudioCallback() {
            @Override
            public void onPrepare() {
                Log.d(TAG,"onPrepare()");
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        AudioCore.getInstance().start();
                    }
                }).start();

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

            @Override
            public void onResume() {
                Log.d(TAG,"onResume()");
            }

            @Override
            public void onPause() {
                Log.d(TAG,"onPause()");
            }

            @Override
            public void onStop() {
                Log.d(TAG,"onStop()");
            }

            @Override
            public void loading(boolean load) {
                Log.d(TAG,"loading() load:"+load);
            }
        });
        AudioCore.getInstance().setErrorCallback(new AudioCore.ErrorCallback() {
            @Override
            public void onError(int code, String msg) {
                Log.d(TAG,"onError() code:"+code +"msg:"+msg);
            }
        });
    }

    @Override
    protected void onStop() {
        super.onStop();
        AudioCore.getInstance().setAudioCallback(null);
        AudioCore.getInstance().setTimeCallback(null);
        AudioCore.getInstance().setErrorCallback(null);
    }

    public void start(View view){
        new Thread(new Runnable() {
            @Override
            public void run() {
                AudioCore.getInstance().parpare(MusicConfig.MUSIC_2);
            }
        }).start();

    }

    public void stop(View view){
        new Thread(new Runnable() {
            @Override
            public void run() {
                AudioCore.getInstance().stop();
            }
        }).start();
    }

    public void resume(View view) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                AudioCore.getInstance().resume();
            }
        }).start();
    }

    public void pause(View view) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                AudioCore.getInstance().pause();
            }
        }).start();
    }

    public void seek(View view) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                AudioCore.getInstance().seek(30);
            }
        }).start();
    }
}

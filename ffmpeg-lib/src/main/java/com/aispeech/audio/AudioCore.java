package com.aispeech.audio;

import android.os.SystemClock;
import android.text.TextUtils;
import android.util.Log;

/**
 * @创建者 luhaisheng
 * @创建时间 2019/12/18 21:01
 * @描述
 */
public class AudioCore {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
        System.loadLibrary("avutil-55");
        System.loadLibrary("swresample-2");
        System.loadLibrary("avcodec-57");
        System.loadLibrary("avformat-57");
        System.loadLibrary("swscale-4");
        System.loadLibrary("postproc-54");
        System.loadLibrary("avfilter-6");
        System.loadLibrary("avdevice-57");
    }

    static AudioCore  sINSTANCE;
    public String nextUrl = null;
    private AudioCore(){

    }

    public static AudioCore getInstance(){
        if (sINSTANCE == null){
            synchronized (AudioCore.class){
                if (sINSTANCE == null){
                    sINSTANCE = new AudioCore();
                }
            }
        }
        return sINSTANCE;
    }



    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native void parpare(String url);
    public native void start();
    public native void resume();
    public native void pause();
    public native void stop();
    public native void seek(int second);
    public void next(String url){
        nextUrl = url;
        stop();
    }
    /*
    *
    *@author luhaisheng
    *@time 2019/12/21 14:10
    */
    public interface AudioCallback{
        void onPrepare();
        void onStart();
        void onProgress(float pro);
        void onComplete(boolean isCom);
        void onResume();
        void onPause();
        void onStop();
        void loading(boolean load);
    }

    public interface TimeCallback{
        void onTimeback(int currentTime,int totalTime);
    }

    public interface ErrorCallback{
        void onError(int code,String msg);
    }


    TimeCallback  mTimeCallback;
    AudioCallback mAudioCallback;
    ErrorCallback mErrorCallback;

    public void setErrorCallback(ErrorCallback errorCallback){
        this.mErrorCallback = errorCallback;
    }


    public void setTimeCallback(TimeCallback timeCallback){
        this.mTimeCallback = timeCallback;
    }

    public void setAudioCallback(AudioCallback audioCallback){
        this.mAudioCallback = audioCallback;
    }

    private static final String TAG = "AudioCore";

    public void prepare_n(){
        Log.d(TAG,"prepare_n()");
        if (mAudioCallback!=null){
            mAudioCallback.onPrepare();
        }
    }

    public void start_n(){
        Log.d(TAG,"start_n()");
        if (mAudioCallback!=null){
            mAudioCallback.onStart();
        }
    }

    public void progress_n(float pro){
        Log.d(TAG,"progress_n() pro:"+pro);
        if (mAudioCallback!=null){
            mAudioCallback.onProgress(pro);
        }
    }

    public void complete_n(){
        Log.d(TAG,"complete_n() nextUrl:"+nextUrl);
        stop();

        if (mAudioCallback!=null){
            mAudioCallback.onComplete(true);
        }

        new Thread(new Runnable() {
            @Override
            public void run() {
                SystemClock.sleep(1000);

                if (!TextUtils.isEmpty(nextUrl)){
                    parpare(nextUrl);
                    nextUrl = null;
                }
            }
        }).start();

    }

    public void resume_n(){
        Log.d(TAG,"resume_n()");
        if (mAudioCallback!=null){
            mAudioCallback.onResume();
        }
    }

    public void pause_n(){
        Log.d(TAG,"pause_n()");
        if (mAudioCallback!=null){
            mAudioCallback.onPause();
        }
    }

    public void stop_n(){
        Log.d(TAG,"stop_n()");
        if (mAudioCallback!=null){
            mAudioCallback.onStop();
        }
    }

    public void load_n(boolean load){
        Log.d(TAG,"load_n() load:"+load);
        if (mAudioCallback!=null){
            mAudioCallback.loading(load);
        }
    }

    public void timeback_n(int currentTime,int duration){
        if (mTimeCallback!=null){
            mTimeCallback.onTimeback(currentTime,duration);
        }
    }

    public void errorback_n(int code,String msg){
        Log.d(TAG,"errorback_n() msg:"+msg);
        stop();
        if (mErrorCallback!=null){
            mErrorCallback.onError(code,msg);
        }
    }
}

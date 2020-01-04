package com.aispeech.audio;

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


    TimeCallback  mTimeCallback;
    AudioCallback mAudioCallback;

    public void setTimeCallback(TimeCallback timeCallback){
        this.mTimeCallback = timeCallback;
    }

    public void setAudioCallback(AudioCallback audioCallback){
        this.mAudioCallback = audioCallback;
    }

    public void prepare_n(){
        if (mAudioCallback!=null){
            mAudioCallback.onPrepare();
        }
    }

    public void start_n(){
        if (mAudioCallback!=null){
            mAudioCallback.onStart();
        }
    }

    public void progress_n(float pro){
        if (mAudioCallback!=null){
            mAudioCallback.onProgress(pro);
        }
    }

    public void complete_n(){
        if (mAudioCallback!=null){
            mAudioCallback.onComplete(true);
        }
    }

    public void resume_n(){
        if (mAudioCallback!=null){
            mAudioCallback.onResume();
        }
    }

    public void pause_n(){
        if (mAudioCallback!=null){
            mAudioCallback.onPause();
        }
    }

    public void stop_n(){
        if (mAudioCallback!=null){
            mAudioCallback.onStop();
        }
    }

    public void load_n(boolean load){
        if (mAudioCallback!=null){
            mAudioCallback.loading(load);
        }
    }

    public void timeback_n(int currentTime,int duration){
        if (mTimeCallback!=null){
            mTimeCallback.onTimeback(currentTime,duration);
        }
    }
}

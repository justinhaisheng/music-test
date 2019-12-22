//
// Created by haisheng.lu on 2019/12/21.
//

#ifndef MUSIC_TEST_AUDIO_CALLBACK_H
#define MUSIC_TEST_AUDIO_CALLBACK_H

#include <jni.h>
#include "AndroidLog.h"

#define MAIN_THREAD 0
#define CHILD_THREAD 1

class Audio_callback {
public:
    JavaVM* javaVm;
    JNIEnv* jniEnv;
    jobject jobj;

    jmethodID jmid_prepare;
    jmethodID jmid_start;
    jmethodID jmid_progress;
    jmethodID jmid_complete;
public:
    Audio_callback(JavaVM *javaVm,JNIEnv *jniEnv,jobject jobj);
    ~Audio_callback();


    void onPrepare(int t_type);
    void onStart(int t_type);
    void onProgress(int t_type,float pro);
    void onComplete(int t_type,bool isCom);
};


#endif //MUSIC_TEST_AUDIO_CALLBACK_H

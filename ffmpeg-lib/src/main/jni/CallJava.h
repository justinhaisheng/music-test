//
// Created by yangw on 2018-2-28.
//

#ifndef MYMUSIC_WLCALLJAVA_H
#define MYMUSIC_WLCALLJAVA_H

#include "jni.h"
#include <linux/stddef.h>
#include "AndroidLog.h"

#define MAIN_THREAD 0
#define CHILD_THREAD 1


class CallJava {

public:
    _JavaVM *javaVM = NULL;
    JNIEnv *jniEnv = NULL;
    jobject jobj;
    jmethodID jmid_prepare;
    jmethodID jmid_start;
    jmethodID jmid_progress;
    jmethodID jmid_complete;
    jmethodID jmid_resume;
    jmethodID jmid_pause;
    jmethodID jmid_stop;
    jmethodID jmid_load;
    jmethodID jmid_timeback;
public:
    CallJava(_JavaVM *javaVM, JNIEnv *env, jobject obj);
    ~CallJava();

    void onCallBack(jmethodID jmid,int type,...);

    void onCallPrepare(int type);
    void onCallStart(int type);
    void onCallResume(int type);
    void onCallPause(int type);
    void onCallComplete(int type);
    void onCallLoad(int type,bool load);
    void onTimeback(int type,int currentTime,int duration);
};


#endif //MYMUSIC_WLCALLJAVA_H

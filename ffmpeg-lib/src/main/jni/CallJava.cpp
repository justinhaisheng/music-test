//
// Created by yangw on 2018-2-28.
//

#include "CallJava.h"

CallJava::CallJava(_JavaVM *javaVM, JNIEnv *env, jobject obj) {

    this->javaVM = javaVM;
    this->jniEnv = env;
    this->jobj = env->NewGlobalRef(obj);
    jclass  jlz = jniEnv->GetObjectClass(jobj);
    if(!jlz)
    {
        LOGE("get jclass wrong");
        return;
    }

    jmid_prepare = env->GetMethodID(jlz, "prepare_n", "()V");
    jmid_start = env->GetMethodID(jlz, "start_n", "()V");
    jmid_progress = env->GetMethodID(jlz, "progress_n", "(F)V");
    jmid_complete = env->GetMethodID(jlz, "complete_n", "()V");
    jmid_pause = env->GetMethodID(jlz, "pause_n", "()V");
    jmid_stop = env->GetMethodID(jlz, "stop_n", "()V");
    jmid_resume = env->GetMethodID(jlz, "resume_n", "()V");
    jmid_load = env->GetMethodID(jlz, "load_n", "(Z)V");
}

void CallJava::onCallBack(jmethodID jmid,int type,...) {

    if(type == MAIN_THREAD)
    {
        va_list args;
        va_start(args, type);
        jniEnv->CallVoidMethod(jobj,jmid,args);
        va_end(args);
    }
    else if(type == CHILD_THREAD)
    {
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK)
        {
            LOGE("get child thread jnienv worng");
            return;
        }
        va_list args;
        va_start(args, type);
        jniEnv->CallVoidMethod(jobj,jmid,args);
        va_end(args);
        javaVM->DetachCurrentThread();
    }
}

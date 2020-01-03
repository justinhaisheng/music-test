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

void CallJava::onCallPrepare(int type) {

    if(type == MAIN_THREAD)
    {
        jniEnv->CallVoidMethod(jobj,jmid_prepare);
    }
    else if(type == CHILD_THREAD)
    {
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK)
        {
            LOGE("get child thread jnienv worng");
            return;
        }
        jniEnv->CallVoidMethod(jobj,jmid_prepare);
        javaVM->DetachCurrentThread();
    }
}

void CallJava::onCallStart(int type) {

    if(type == MAIN_THREAD)
    {
        jniEnv->CallVoidMethod(jobj,jmid_start);
    }
    else if(type == CHILD_THREAD)
    {
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK)
        {
            LOGE("get child thread jnienv worng");
            return;
        }
        jniEnv->CallVoidMethod(jobj,jmid_start);
        javaVM->DetachCurrentThread();
    }
}

void CallJava::onCallResume(int type) {

    if(type == MAIN_THREAD)
    {
        jniEnv->CallVoidMethod(jobj,jmid_resume);
    }
    else if(type == CHILD_THREAD)
    {
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK)
        {
            LOGE("get child thread jnienv worng");
            return;
        }
        jniEnv->CallVoidMethod(jobj,jmid_resume);
        //思考为什么这里调用DetachCurrentThread 会崩溃

     //   javaVM->DetachCurrentThread();
    }
}

void CallJava::onCallPause(int type) {

    if(type == MAIN_THREAD)
    {
        jniEnv->CallVoidMethod(jobj,jmid_pause);
    }
    else if(type == CHILD_THREAD)
    {
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK)
        {
            LOGE("get child thread jnienv worng");
            return;
        }
        jniEnv->CallVoidMethod(jobj,jmid_pause);
       // javaVM->DetachCurrentThread();
    }
}

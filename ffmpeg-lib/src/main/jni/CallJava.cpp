//
// Created by yangw on 2018-2-28.
//

#include "CallJava.h"

CallJava::CallJava(_JavaVM *javaVM, JNIEnv *env, jobject obj) {
    LOGD("CallJava");
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
    jmid_timeback = env->GetMethodID(jlz, "timeback_n", "(II)V");
    jmid_errorback = env->GetMethodID(jlz, "errorback_n", "(ILjava/lang/String;)V");
    env->DeleteLocalRef(jlz);
}

CallJava::~CallJava(){
    LOGD("~CallJava");
    JNIEnv *jniEnv;
    if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK)
    {
        LOGE("get child thread jnienv worng");
        return;
    }
    jniEnv->DeleteGlobalRef(jobj);
  //  javaVM->DetachCurrentThread();
}


void CallJava::onCallLoad(int type,bool load) {
    LOGD("onCallBack");
    if(type == MAIN_THREAD)
    {
        jniEnv->CallVoidMethod(jobj,jmid_load,load);
    }
    else if(type == CHILD_THREAD)
    {
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK)
        {
            LOGE("get child thread jnienv worng");
            return;
        }

        jniEnv->CallVoidMethod(jobj,jmid_load,load);

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
void CallJava::onTimeback(int type,int currentTime, int duration){

    if(type == MAIN_THREAD)
    {
        jniEnv->CallVoidMethod(jobj,jmid_timeback,currentTime,duration);
    }
    else if(type == CHILD_THREAD)
    {
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK)
        {
            LOGE("get child thread jnienv worng");
            return;
        }
        jniEnv->CallVoidMethod(jobj,jmid_timeback,currentTime,duration);
        javaVM->DetachCurrentThread();
    }
}

void CallJava::onErrorback(int type,int code,char* msg){
    if(type == MAIN_THREAD)
    {
        jstring jmsg = jniEnv->NewStringUTF(msg);
        jniEnv->CallVoidMethod(jobj, jmid_errorback, code, jmsg);
        jniEnv->DeleteLocalRef(jmsg);
    }
    else if(type == CHILD_THREAD)
    {
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK)
        {
            LOGE("get child thread jnienv worng");
            return;
        }
        jstring jmsg = jniEnv->NewStringUTF(msg);
        jniEnv->CallVoidMethod(jobj, jmid_errorback, code, jmsg);
        jniEnv->DeleteLocalRef(jmsg);
       // jniEnv->CallVoidMethod(jobj,jmid_errorback,code,msg);
        javaVM->DetachCurrentThread();
    }
}

void CallJava::onCallComplete(int type){
    if(type == MAIN_THREAD)
    {
        jniEnv->CallVoidMethod(jobj,jmid_complete);
    }
    else if(type == CHILD_THREAD)
    {
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK)
        {
            LOGE("get child thread jnienv worng");
            return;
        }
        jniEnv->CallVoidMethod(jobj,jmid_complete);
        // javaVM->DetachCurrentThread();
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

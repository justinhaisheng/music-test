//
// Created by haisheng.lu on 2019/12/21.
//

#include "Audio_callback.h"


Audio_callback::Audio_callback(JavaVM *javaVm,JNIEnv *jniEnv,jobject jobj){
    LOGD("Audio_callback");
    this->javaVm = javaVm;
    this->jniEnv = jniEnv;
    this->jobj = jobj;

    jclass cls = jniEnv->GetObjectClass(jobj);
    if (!cls){
        LOGE("cls == NULL");
        return ;
    }
    this->jmid_prepare = jniEnv->GetMethodID(cls,"prepare","()V");
    this->jmid_start = jniEnv->GetMethodID(cls,"start","()V");
    this->jmid_progress = jniEnv->GetMethodID(cls,"progress","(F)V");
    this->jmid_complete = jniEnv->GetMethodID(cls,"complete","(Z)V");

    if (!jmid_prepare){
        LOGE("no prepare");
    }
}

Audio_callback::~Audio_callback(){
    LOGD("~Audio_callback");
}













//回调Prepare
void Audio_callback::onPrepare(int t_type){
    LOGD("onPrepare %d",t_type);
    if (t_type == MAIN_THREAD){
        this->jniEnv->CallVoidMethod(this->jobj,this->jmid_prepare);
    }else{
        JNIEnv* env;
        this->javaVm->AttachCurrentThread(&env,0);
        env->CallVoidMethod(this->jobj,this->jmid_prepare);
        this->javaVm->DetachCurrentThread();
    }
}
//回到start
void Audio_callback::onStart(int t_type){
    if (t_type == MAIN_THREAD){
        this->jniEnv->CallVoidMethod(this->jobj,this->jmid_start);
    }else{
        JNIEnv* env;
        this->javaVm->AttachCurrentThread(&env,0);
        env->CallVoidMethod(this->jobj,this->jmid_start);
        this->javaVm->DetachCurrentThread();
    }
}
//回调progress pro 进度值
void Audio_callback::onProgress(int t_type,float pro){
    if (t_type == MAIN_THREAD){
        this->jniEnv->CallVoidMethod(this->jobj,this->jmid_prepare,pro);
    }else{
        JNIEnv* env;
        this->javaVm->AttachCurrentThread(&env,0);
        env->CallVoidMethod(this->jobj,this->jmid_prepare,pro);
        this->javaVm->DetachCurrentThread();
    }
}
//回调complete isCom true 主动播放完成
void Audio_callback::onComplete(int t_type,bool isCom){
    if (t_type == MAIN_THREAD){
        this->jniEnv->CallVoidMethod(this->jobj,this->jmid_complete,isCom);
    }else{
        JNIEnv* env;
        this->javaVm->AttachCurrentThread(&env,0);
        env->CallVoidMethod(this->jobj,this->jmid_complete,isCom);
        this->javaVm->DetachCurrentThread();
    }
}

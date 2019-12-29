//
// Created by yangw on 2018-2-28.
//

#include "CallJava.h"

CallJava::CallJava(_JavaVM *javaVM, JNIEnv *env, jobject obj) {

    this->javaVM = javaVM;
    this->jniEnv = env;
    //this->jobj = *obj;
    this->jobj = env->NewGlobalRef(obj);

    jclass  jlz = jniEnv->GetObjectClass(jobj);
    if(!jlz)
    {

            LOGE("get jclass wrong");

        return;
    }

    jmid_parpared = env->GetMethodID(jlz, "prepare", "()V");




}

void CallJava::onCallParpared(int type) {

    if(type == MAIN_THREAD)
    {
        jniEnv->CallVoidMethod(jobj, jmid_parpared);
    }
    else if(type == CHILD_THREAD)
    {
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK)
        {

                LOGE("get child thread jnienv worng");

            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_parpared);
        javaVM->DetachCurrentThread();
    }

}
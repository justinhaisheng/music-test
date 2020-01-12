
#include "com_aispeech_audio_AudioCore.h"
#include "FFmpegCore.h"
#include <string.h>
JavaVM* javaVm ;
FFmpegCore* core = NULL;
CallJava* callJava =NULL;
Playstatus* playStatus = NULL;

bool next = true;


extern "C"
JNIEXPORT void JNICALL Java_com_aispeech_audio_AudioCore_start
        (JNIEnv *env, jobject jobj){
    if (core && playStatus && !playStatus->exit){
        core->start();
    }
}

extern "C"
JNIEXPORT void JNICALL Java_com_aispeech_audio_AudioCore_parpare
        (JNIEnv *env, jobject jobj, jstring jstr){

    const char* url = env->GetStringUTFChars(jstr, 0);
    LOGD("Java_com_aispeech_audio_AudioCore_start %s",url);
    if (core == NULL){
        if (callJava == NULL){
            callJava = new CallJava(javaVm,env,jobj);
        }
        playStatus = new Playstatus();
        core = new FFmpegCore(playStatus,callJava,url);
    }
    env->ReleaseStringUTFChars(jstr,url);
    core->prepare();
}

extern "C"
JNIEXPORT void JNICALL Java_com_aispeech_audio_AudioCore_resume
        (JNIEnv *env, jobject jobj){
    core->resume();
}

extern "C"
JNIEXPORT void JNICALL Java_com_aispeech_audio_AudioCore_pause
        (JNIEnv *env, jobject jobj){
    core->pause();
}

extern "C"
JNIEXPORT void JNICALL Java_com_aispeech_audio_AudioCore_stop
        (JNIEnv *env, jobject jobj){

    if (!next){
        LOGE("next false");
        return;
    }
    next = false;

    if (core){
        core->release();
        delete(core);
        core = NULL;
    }

    if (callJava){
        delete(callJava);
        callJava = NULL;
    }
    if (playStatus){
        delete(playStatus);
        playStatus = NULL;
    }
    next = true;
}

extern "C"
JNIEXPORT void JNICALL Java_com_aispeech_audio_AudioCore_seek
        (JNIEnv *env, jobject jobj, jint jsecond){
    if (core){
        core->seek(jsecond);
    }
}

extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void* reserved){
    LOGD("JNI_OnLoad");
    javaVm = vm;
    JNIEnv* env;
    if (vm->GetEnv((void **)&env,JNI_VERSION_1_6)!=JNI_OK){
        LOGE("JNI_OnLoad error");
        return -1;
    }
    return JNI_VERSION_1_6;
}

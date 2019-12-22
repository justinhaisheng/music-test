
#include "com_aispeech_audio_AudioCore.h"
#include "FFmpeg_util.h"
#include <string.h>
JavaVM* javaVm ;
FFmpeg_util* fFmpegUtil = NULL;
Audio_callback* audioCallback =NULL;
Play_status* playStatus = NULL;
extern "C"
JNIEXPORT void JNICALL Java_com_aispeech_audio_AudioCore_start
        (JNIEnv *env, jobject jobj, jstring jstr){

     const char* url = env->GetStringUTFChars(jstr, 0);
    LOGD("Java_com_aispeech_audio_AudioCore_start %s",url);
    if (fFmpegUtil == NULL){
        if (audioCallback == NULL){
            audioCallback = new Audio_callback(javaVm,env,env->NewGlobalRef(jobj));
        }
        playStatus = new Play_status();
        fFmpegUtil = new FFmpeg_util(playStatus,url,audioCallback);

    }
    env->ReleaseStringUTFChars(jstr,url);

    fFmpegUtil->prepare();
}

extern "C"
JNIEXPORT void JNICALL Java_com_aispeech_audio_AudioCore_stop
        (JNIEnv *env, jobject jobj){

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

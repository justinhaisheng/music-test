//
// Created by yangw on 2018-2-28.
//

#ifndef MYMUSIC_WLAUDIO_H
#define MYMUSIC_WLAUDIO_H

#include "PlayQueue.h"
#include "Playstatus.h"
#include "CallJava.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include <libswresample/swresample.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
};

class Audio {

public:
    int streamIndex = -1;
    AVCodecContext *avCodecContext = NULL;
    AVCodecParameters *codecpar = NULL;
    CallJava *callJava = NULL;
    PlayQueue *queue = NULL;
    Playstatus *playstatus = NULL;


    pthread_t thread_play;
    AVPacket *avPacket = NULL;
    AVFrame *avFrame = NULL;
    //int ret = 0;
    uint8_t *buffer = NULL;
    //int data_size = 0;
    int sample_rate;

    int duration = 0;//总时长
    AVRational time_base;//AVframe 的时间基
    double cureent_clock;//
    double now_time;//当前frame时间
    double last_time;//上一次调用的时间




    // 引擎接口
    SLObjectItf engineObject = NULL;
    SLEngineItf engineEngine = NULL;

    //混音器
    SLObjectItf outputMixObject = NULL;
    SLEnvironmentalReverbItf outputMixEnvironmentalReverb = NULL;
    SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

    //pcm
    SLObjectItf pcmPlayerObject = NULL;
    SLPlayItf pcmPlayerPlay = NULL;

    //缓冲器队列接口
    SLAndroidSimpleBufferQueueItf pcmBufferQueue = NULL;
public:
    Audio(Playstatus *playstatus,int sample_rate,CallJava *callJava);
    ~Audio();

    void play();
    void resume();
    void pause();
    void stop();
    void initOpenSLES();
    int resampleAudio();
    int getCurrentSampleRateForOpensles(int sample_rate);
};


#endif //MYMUSIC_WLAUDIO_H

//
// Created by yangw on 2018-2-28.
//

#ifndef MYMUSIC_WLAUDIO_H
#define MYMUSIC_WLAUDIO_H

#include "PlayQueue.h"
#include "Playstatus.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include <libswresample/swresample.h>
};

class Audio {

public:
    int streamIndex = -1;
    AVCodecContext *avCodecContext = NULL;
    AVCodecParameters *codecpar = NULL;
    PlayQueue *queue = NULL;
    Playstatus *playstatus = NULL;

    pthread_t thread_play;
    AVPacket *avPacket = NULL;
    AVFrame *avFrame = NULL;
    //int ret = 0;
    uint8_t *buffer = NULL;
    //int data_size = 0;



public:
    Audio(Playstatus *playstatus);
    ~Audio();

    void play();
    int resampleAudio();


};


#endif //MYMUSIC_WLAUDIO_H

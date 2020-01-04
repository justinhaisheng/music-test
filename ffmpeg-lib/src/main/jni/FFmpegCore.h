//
// Created by yangw on 2018-2-28.
//

#ifndef MYMUSIC_WLFFMPEG_H
#define MYMUSIC_WLFFMPEG_H

#include "CallJava.h"
#include "pthread.h"
#include "Audio.h"
#include "Playstatus.h"

extern "C"
{
#include "libavformat/avformat.h"
};


class FFmpegCore {

public:
    CallJava *callJava = NULL;
    char* url = NULL;
    pthread_t decodeThread;
    AVFormatContext *pFormatCtx = NULL;
    Audio *audio = NULL;
    Playstatus *playstatus = NULL;



public:
    FFmpegCore(Playstatus *playstatus, CallJava *callJava, const char *url);
    ~FFmpegCore();

    void prepare();
    void decodeFFmpegThread();
    void start();
    void pause();
    void resume();
    void stop();
};


#endif //MYMUSIC_WLFFMPEG_H

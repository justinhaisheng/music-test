//
// Created by haisheng.lu on 2019/12/21.
//

#ifndef MUSIC_TEST_FFMPEG_UTIL_H
#define MUSIC_TEST_FFMPEG_UTIL_H

#include <pthread.h>
#include "Audio_callback.h"
#include "Audio.h"
extern "C"{
#include "include/libavformat/avformat.h"
};


//解码工具
class FFmpeg_util {


public:

    char* url=NULL;
    Audio_callback* callback=NULL;
    Audio* audio =NULL;

    AVFormatContext* avFormatContext = NULL;
    pthread_t codec_t;



    FFmpeg_util(const char* url,Audio_callback* callback);
    ~FFmpeg_util();
    FFmpeg_util(const FFmpeg_util& fFmpegUtil);
    void prepare();
};


#endif //MUSIC_TEST_FFMPEG_UTIL_H

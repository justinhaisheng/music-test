//
// Created by haisheng.lu on 2019/12/21.
//

#ifndef MUSIC_TEST_AUDIO_H
#define MUSIC_TEST_AUDIO_H
extern "C"{
#include "include/libavcodec/avcodec.h"
};

#include "AndroidLog.h"
#include "Play_status.h"
#include "Play_queue.h"

//保存解码音频的信息
class Audio {
private:
    int streamIndex = -1;//码流id
    AVCodecContext* avCodecContext =NULL;//编码器上下文
    AVCodecParameters* parameters = NULL;//编码器参数

public:
    Audio(Play_status* play_status);
    ~Audio();
    Play_status* play_status = NULL;
    Play_queue* queue =NULL;
    int getStreamIndex() const;

    void setStreamIndex(int streamIndex);

    AVCodecContext *getAvCodecContext() const;

    void setAvCodecContext(AVCodecContext *avCodecContext);

    AVCodecParameters *getParameters();

    void setParameters(AVCodecParameters *parameters);
};


#endif //MUSIC_TEST_AUDIO_H

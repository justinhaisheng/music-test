//
// Created by haisheng.lu on 2019/12/22.
//

#ifndef MUSIC_TEST_PLAY_QUEUE_H
#define MUSIC_TEST_PLAY_QUEUE_H

#include <queue>
#include <pthread.h>
#include "AndroidLog.h"
#include "Play_status.h"

extern "C"
{
#include "libavcodec/avcodec.h"
};


class Play_queue {
public:
    std::queue<AVPacket *> queuePacket;
    pthread_mutex_t mute;
    pthread_cond_t cond;

    Play_status* playStatus =NULL;

    Play_queue(Play_status* playStatus);
    ~Play_queue();

    int putAvpacket(AVPacket *packet);
    int getAvpacket(AVPacket *packet);

    int getQueueSize();
};


#endif //MUSIC_TEST_PLAY_QUEUE_H

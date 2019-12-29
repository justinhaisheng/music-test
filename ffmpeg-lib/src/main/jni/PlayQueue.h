//
// Created by yangw on 2018-3-6.
//

#ifndef MUSIC_TEST_PLAYQUEUE_H
#define MUSIC_TEST_PLAYQUEUE_H

#include <queue>
#include <pthread.h>
#include "AndroidLog.h"
#include "Playstatus.h"

extern "C"
{
#include "libavcodec/avcodec.h"
};


class PlayQueue {

public:
    std::queue<AVPacket *> queuePacket;
    pthread_mutex_t mutexPacket;
    pthread_cond_t condPacket;
    Playstatus *playstatus = NULL;

public:

    PlayQueue(Playstatus *playstatus);
    ~PlayQueue();

    int putAvpacket(AVPacket *packet);
    int getAvpacket(AVPacket *packet);

    int getQueueSize();




};


#endif //MYMUSIC_WLQUEUE_H

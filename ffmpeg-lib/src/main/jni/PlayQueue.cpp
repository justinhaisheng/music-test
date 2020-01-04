//
// Created by yangw on 2018-3-6.
//

#include "PlayQueue.h"

PlayQueue::PlayQueue(Playstatus *playstatus) {
    this->playstatus = playstatus;
    pthread_mutex_init(&mutexPacket, NULL);
    pthread_cond_init(&condPacket, NULL);

}

PlayQueue::~PlayQueue() {
    clearQueue();
    pthread_mutex_destroy(&mutexPacket);
    pthread_cond_destroy(&condPacket);
}

int PlayQueue::putAvpacket(AVPacket *packet) {

    pthread_mutex_lock(&mutexPacket);

    queuePacket.push(packet);

    //LOGD("放入一个AVpacket到队里里面， 个数为：%d", queuePacket.size());

    pthread_cond_signal(&condPacket);
    pthread_mutex_unlock(&mutexPacket);

    return 0;
}

int PlayQueue::getAvpacket(AVPacket *packet) {

    pthread_mutex_lock(&mutexPacket);

    while(playstatus != NULL && !playstatus->exit)
    {
        if(queuePacket.size() > 0)
        {
            AVPacket *avPacket =  queuePacket.front();
            if(av_packet_ref(packet, avPacket) == 0)
            {
                queuePacket.pop();
            }
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;

            //LOGD("从队列里面取出一个AVpacket，还剩下 %d 个", queuePacket.size());

            break;
        } else{
            pthread_cond_wait(&condPacket, &mutexPacket);
        }
    }
    pthread_mutex_unlock(&mutexPacket);
    return 0;
}

int PlayQueue::getQueueSize() {

    int size = 0;
    pthread_mutex_lock(&mutexPacket);
    size = queuePacket.size();
    pthread_mutex_unlock(&mutexPacket);

    return size;
}

void PlayQueue::clearQueue() {
    LOGD("clearQueue");
    pthread_cond_signal(&condPacket);
    pthread_mutex_lock(&mutexPacket);
    while (!queuePacket.empty()){
        AVPacket *avPacket =  queuePacket.front();
        queuePacket.pop();
        av_packet_free(&avPacket);
        av_free(avPacket);
        avPacket = NULL;
    }
    pthread_mutex_unlock(&mutexPacket);
}
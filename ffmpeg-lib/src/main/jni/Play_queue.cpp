//
// Created by haisheng.lu on 2019/12/22.
//

#include "Play_queue.h"

Play_queue::Play_queue(Play_status* playStatus){
    this->playStatus = playStatus;
    pthread_mutex_init(&mute,NULL);
    pthread_cond_init(&cond,NULL);
}

Play_queue::~Play_queue(){
    pthread_mutex_destroy(&mute);
    pthread_cond_destroy(&cond);
}

//入队
int Play_queue::putAvpacket(AVPacket *packet){
    pthread_mutex_lock(&mute);
    this->queuePacket.push(packet);
    LOGD("放入一个AVpacket到队里里面， 个数为：%d", queuePacket.size());
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mute);
    return 0;
}

//出队
int Play_queue::getAvpacket(AVPacket *packet){
    pthread_mutex_lock(&mute);
    while (playStatus!=NULL && !playStatus->is_exit){

        if (queuePacket.size()>0){
            AVPacket* avPacket = queuePacket.front();
            if (av_packet_ref(packet,avPacket) == 0 ){//copy
                queuePacket.pop();
                LOGD("从队列里取出一个AVpacket， 个数为：%d", queuePacket.size());
            }
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
        }else{
            LOGE("队列里没有AVpacket,wait");
            pthread_cond_wait(&cond,&mute);
        }

    }
    pthread_mutex_unlock(&mute);
    return 0;
}

//获取队列大小
int Play_queue::getQueueSize(){
    int size = 0;
    pthread_mutex_lock(&mute);
    size = this->queuePacket.size();
    pthread_mutex_unlock(&mute);
    return size;
}

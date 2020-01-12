//
// Created by yangw on 2018-2-28.
//

#include "FFmpegCore.h"

FFmpegCore::FFmpegCore(Playstatus *playstatus, CallJava *callJava, const char *url) {
    this->playstatus = playstatus;
    this->callJava = callJava;
    this->url = static_cast<char *>(malloc(sizeof(char) * strlen(url)));
    strcpy(this->url,url);

    pthread_mutex_init(&ffmpeg_mutex,NULL);
}

FFmpegCore::~FFmpegCore(){
    free(url);
    url = NULL;
    pthread_mutex_destroy(&ffmpeg_mutex);
}

void *decodeFFmpeg(void *data)
{
    FFmpegCore *wlFFmpeg = (FFmpegCore *) data;
    wlFFmpeg->decodeFFmpegThread();
    pthread_exit(&wlFFmpeg->decodeThread);
}

void FFmpegCore::prepare() {

    pthread_create(&decodeThread, NULL, decodeFFmpeg, this);

}

int avformat_callback(void *ctx)
{
    LOGD("avformat_callback");
    FFmpegCore *fFmpeg = (FFmpegCore *) ctx;
    if(fFmpeg->playstatus->exit)
    {
        return AVERROR_EOF;
    }
    return 0;
}

void FFmpegCore::decodeFFmpegThread() {

    pthread_mutex_lock(&ffmpeg_mutex);
    av_register_all();
    avformat_network_init();
    pFormatCtx = avformat_alloc_context();

    pFormatCtx->interrupt_callback.callback = avformat_callback;
    pFormatCtx->interrupt_callback.opaque = this;
    if(avformat_open_input(&pFormatCtx, url, NULL, NULL) != 0)
    {
        LOGE("can not open url :%s", url);


        if(pFormatCtx != NULL)
        {
            avformat_close_input(&pFormatCtx);
            avformat_free_context(pFormatCtx);
            pFormatCtx = NULL;
        }

        if(callJava){
            char temp[100];
            sprintf(temp,"can not open %s",url);
            callJava->onErrorback(CHILD_THREAD,001,temp);
        }

        pthread_mutex_unlock(&ffmpeg_mutex);
        exit = true;
        return;
    }
    if(avformat_find_stream_info(pFormatCtx, NULL) < 0)
    {
        LOGE("can not find streams from %s", url);

        if(pFormatCtx != NULL)
        {
            avformat_close_input(&pFormatCtx);
            avformat_free_context(pFormatCtx);
            pFormatCtx = NULL;
        }

        if(callJava){
            char temp[100];
            sprintf(temp,"can not find streams from %s",url);
            callJava->onErrorback(CHILD_THREAD,002,temp);
        }

        pthread_mutex_unlock(&ffmpeg_mutex);
        exit = true;

        return;
    }
    for(int i = 0; i < pFormatCtx->nb_streams; i++)
    {
        if(pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)//得到音频流
        {
            if(audio == NULL)
            {
                audio = new Audio(playstatus,pFormatCtx->streams[i]->codecpar->sample_rate,callJava);
                audio->duration = pFormatCtx->duration/AV_TIME_BASE;
                audio->time_base = pFormatCtx->streams[i]->time_base;
                audio->streamIndex = i;
                audio->codecpar = pFormatCtx->streams[i]->codecpar;
            }
        }
    }

    AVCodec *dec = avcodec_find_decoder(audio->codecpar->codec_id);
    if(!dec)
    {
        LOGE("can not find decoder");
        if(pFormatCtx != NULL)
        {
            avformat_close_input(&pFormatCtx);
            avformat_free_context(pFormatCtx);
            pFormatCtx = NULL;
        }

        if(callJava){
            char temp[100];
            sprintf(temp,"can not find decoder %s",url);
            callJava->onErrorback(CHILD_THREAD,003,temp);
        }
        pthread_mutex_unlock(&ffmpeg_mutex);
        exit = true;

        return;
    }

    audio->avCodecContext = avcodec_alloc_context3(dec);
    if(!audio->avCodecContext)
    {
        LOGE("can not alloc new decodecctx");
        if(pFormatCtx != NULL)
        {
            avformat_close_input(&pFormatCtx);
            avformat_free_context(pFormatCtx);
            pFormatCtx = NULL;
        }

        if(audio->avCodecContext != NULL)
        {
            avcodec_close(audio->avCodecContext);
            avcodec_free_context(&audio->avCodecContext);
            audio->avCodecContext = NULL;
        }

        if(callJava){
            char temp[100];
            sprintf(temp,"can not alloc new decodecctx %s",url);
            callJava->onErrorback(CHILD_THREAD,004,temp);
        }
        pthread_mutex_unlock(&ffmpeg_mutex);
        exit = true;
        return;
    }

    if(avcodec_parameters_to_context(audio->avCodecContext, audio->codecpar) < 0)
    {
        LOGE("can not fill decodecctx");
        if(pFormatCtx != NULL)
        {
            avformat_close_input(&pFormatCtx);
            avformat_free_context(pFormatCtx);
            pFormatCtx = NULL;
        }

        if(audio->avCodecContext != NULL)
        {
            avcodec_close(audio->avCodecContext);
            avcodec_free_context(&audio->avCodecContext);
            audio->avCodecContext = NULL;
        }

        if(callJava){
            char temp[100];
            sprintf(temp,"can not fill decodecctx %s",url);
            callJava->onErrorback(CHILD_THREAD,005,temp);
        }
        pthread_mutex_unlock(&ffmpeg_mutex);
        exit = true;

        return;
    }

    if(avcodec_open2(audio->avCodecContext, dec, 0) != 0)
    {
        LOGE("cant not open audio strames");

        if(pFormatCtx != NULL)
        {
            avformat_close_input(&pFormatCtx);
            avformat_free_context(pFormatCtx);
            pFormatCtx = NULL;
        }

        if(audio->avCodecContext != NULL)
        {
            avcodec_close(audio->avCodecContext);
            avcodec_free_context(&audio->avCodecContext);
            audio->avCodecContext = NULL;
        }

        if(callJava){
            char temp[100];
            sprintf(temp,"cant not open audio strames %s",url);
            callJava->onErrorback(CHILD_THREAD,006,temp);
        }
        pthread_mutex_unlock(&ffmpeg_mutex);
        exit = true;
        return;
    }
    if(callJava != NULL)
    {
        if(playstatus != NULL && !playstatus->exit)
        {
            callJava->onCallPrepare(CHILD_THREAD);
        } else{
            exit = true;
        }
    }

    pthread_mutex_unlock(&ffmpeg_mutex);

}

void FFmpegCore::start() {

    if(audio == NULL)
    {
            LOGE("audio is null");
            return;
    }
    audio->play();

    int count = 0;

    while(playstatus != NULL && !playstatus->exit)
    {
        AVPacket *avPacket = av_packet_alloc();
        if(av_read_frame(pFormatCtx, avPacket) == 0)
        {
            if(avPacket->stream_index == audio->streamIndex)
            {
                //解码操作
                count++;
                //LOGI("解码第 %d 帧", count);
                audio->queue->putAvpacket(avPacket);
            } else{
                av_packet_free(&avPacket);
                av_free(avPacket);
            }
        } else{
            av_packet_free(&avPacket);
            av_free(avPacket);
            while(playstatus != NULL && !playstatus->exit)
            {
                if(audio->queue->getQueueSize() > 0)
                {
                    continue;
                } else{
                    playstatus->exit = true;
                    break;
                }
            }
        }
    }

    callJava->onCallComplete(CHILD_THREAD);
    exit = true;
    LOGD("解码完成");

}

void FFmpegCore::pause(){
    if (audio){
        audio->pause();
    }
}


void FFmpegCore::resume(){
    if (audio){
        audio->resume();
    }
}

void FFmpegCore::release(){

    LOGD("release 开始释放Ffmpe");
//    if(playstatus->exit){
//        return;
//    }
    playstatus->exit = true;
    pthread_mutex_lock(&ffmpeg_mutex);
    int sleepCount = 0;
    while (!exit)
    {
        if(sleepCount > 1000)
        {
            exit = true;
        }

        LOGE("wait ffmpeg  exit %d", sleepCount);

        sleepCount++;
        av_usleep(1000 * 10);//暂停10毫秒
    }

    if(pFormatCtx != NULL)
    {
        avformat_close_input(&pFormatCtx);
        avformat_free_context(pFormatCtx);
        pFormatCtx = NULL;
    }


    if (audio){
        audio->stop();
        audio->release();
        delete(audio);
        audio = NULL;
    }

    if(playstatus != NULL)
    {
        playstatus = NULL;
    }
    pthread_mutex_unlock(&ffmpeg_mutex);

    LOGD("release 释放Ffmpe完毕");
}

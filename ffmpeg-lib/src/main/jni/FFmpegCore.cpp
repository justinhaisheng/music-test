//
// Created by yangw on 2018-2-28.
//

#include "FFmpegCore.h"

FFmpegCore::FFmpegCore(Playstatus *playstatus, CallJava *callJava, const char *url) {
    this->playstatus = playstatus;
    this->callJava = callJava;
    this->url = url;
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

void FFmpegCore::decodeFFmpegThread() {

    av_register_all();
    avformat_network_init();
    pFormatCtx = avformat_alloc_context();
    if(avformat_open_input(&pFormatCtx, url, NULL, NULL) != 0)
    {

            LOGE("can not open url :%s", url);

        return;
    }
    if(avformat_find_stream_info(pFormatCtx, NULL) < 0)
    {

            LOGE("can not find streams from %s", url);

        return;
    }
    for(int i = 0; i < pFormatCtx->nb_streams; i++)
    {
        if(pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)//得到音频流
        {
            if(audio == NULL)
            {
                audio = new Audio(playstatus);
                audio->streamIndex = i;
                audio->codecpar = pFormatCtx->streams[i]->codecpar;
            }
        }
    }

    AVCodec *dec = avcodec_find_decoder(audio->codecpar->codec_id);
    if(!dec)
    {

            LOGE("can not find decoder");

        return;
    }

    audio->avCodecContext = avcodec_alloc_context3(dec);
    if(!audio->avCodecContext)
    {

            LOGE("can not alloc new decodecctx");

        return;
    }

    if(avcodec_parameters_to_context(audio->avCodecContext, audio->codecpar) < 0)
    {

            LOGE("can not fill decodecctx");

        return;
    }

    if(avcodec_open2(audio->avCodecContext, dec, 0) != 0)
    {

            LOGE("cant not open audio strames");

        return;
    }
    callJava->onCallParpared(CHILD_THREAD);
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

                    LOGE("解码第 %d 帧", count);

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


        LOGD("解码完成");

}

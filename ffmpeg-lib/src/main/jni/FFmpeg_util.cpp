//
// Created by haisheng.lu on 2019/12/21.
//

#include "FFmpeg_util.h"

FFmpeg_util::FFmpeg_util(const char* url,Audio_callback* callback){
    LOGD("FFmpeg_util");
    this->url = static_cast<char *>(malloc(sizeof(char) * 100));
    strcpy(this->url,url);
    this->callback = callback;
    this->audio = audio;
}

FFmpeg_util::FFmpeg_util(const FFmpeg_util& fFmpegUtil){
    LOGD("拷贝 FFmpeg_util");
    this->url = static_cast<char *>(malloc(sizeof(char) * 100));
    strcpy(this->url,fFmpegUtil.url);
    this->callback = fFmpegUtil.callback;
    this->avFormatContext = fFmpegUtil.avFormatContext;
    this->audio = fFmpegUtil.audio;
    this->codec_t = fFmpegUtil.codec_t;
}

FFmpeg_util::~FFmpeg_util(){
    LOGD("~FFmpeg_util");
    if (this->url)
    {
        free(this->url);
    }
    this->url = NULL;
}



void *decode_ffmpeg(void *data)
{
    LOGD("decode_ffmpeg()");
    FFmpeg_util* fmpegUtil = static_cast<FFmpeg_util *>(data);
    //注册解码器并初始化网络
    av_register_all();
    avformat_network_init();
    LOGD("decode_ffmpeg() %s",fmpegUtil->url);
    //打开文件或网络流
    AVFormatContext* avFormatContext = avformat_alloc_context();
    LOGD("decode_ffmpeg() 0 %s",fmpegUtil->url);

    if (avformat_open_input(&avFormatContext, fmpegUtil->url, NULL, NULL)!=0){
        LOGE("avformat_open_input error %s !",fmpegUtil->url);
        return reinterpret_cast<void *>(-1);
    }
    LOGD("decode_ffmpeg() 1 %s",fmpegUtil->url);

    //获取流信息
    if (avformat_find_stream_info(avFormatContext,NULL)<0){
        LOGE("avformat_find_stream_info error %s !",fmpegUtil->url);
        return reinterpret_cast<void *>(-1);
    }
    LOGD("decode_ffmpeg() 2 %s",fmpegUtil->url);
    // 获取音频流
    for(int i =0;i<avFormatContext->nb_streams;i++){
        LOGD("流 %d %d",i,avFormatContext->nb_streams);
        if(avFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO){//音频流
            LOGD("codecpar->codec_id == AVMEDIA_TYPE_AUDIO %d",i);
            if (fmpegUtil->audio == NULL){
                LOGD("mpegUtil->audio == NULL %d",i);
                fmpegUtil->audio = new Audio();
                fmpegUtil->audio->setStreamIndex(i);
                fmpegUtil->audio->setParameters(avFormatContext->streams[i]->codecpar);
            }
        }
    }
    fmpegUtil->avFormatContext = avFormatContext;
    LOGD("decode_ffmpeg() 3 %s",fmpegUtil->url);
    if (fmpegUtil->audio->getParameters()== NULL){
        LOGE("getParameters()== NULL");
        return reinterpret_cast<void *>(-1);
    }
    //获取解码器
    AVCodec* avCodec =avcodec_find_decoder(fmpegUtil->audio->getParameters()->codec_id);
    LOGD("decode_ffmpeg() 33 %s",fmpegUtil->url);
    if (!avCodec){
        LOGE("avcodec_find_decoder error %s !",fmpegUtil->url);
        return reinterpret_cast<void *>(-1);
    }
    LOGD("decode_ffmpeg() 4 %s",fmpegUtil->url);
    //利用解码器创建解码器上下文
    AVCodecContext* avCodecContext = avcodec_alloc_context3(avCodec);
    if (!avCodecContext){
        LOGE("avcodec_alloc_context3 error %s !",fmpegUtil->url);
        return reinterpret_cast<void *>(-1);
    }
    LOGD("decode_ffmpeg() 5 %s",fmpegUtil->url);
    fmpegUtil->audio->setAvCodecContext(avCodecContext);
    if (avcodec_parameters_to_context(fmpegUtil->audio->getAvCodecContext(),fmpegUtil->audio->getParameters()) <0){
        LOGE("avcodec_parameters_to_context error %s !",fmpegUtil->url);
        return reinterpret_cast<void *>(-1);
    }
    LOGD("decode_ffmpeg() 6 %s",fmpegUtil->url);
    // 打开解码器
    if(avcodec_open2(fmpegUtil->audio->getAvCodecContext(),avCodec,0)!=0){
        LOGE("avcodec_open2 error %s !",fmpegUtil->url);
        return reinterpret_cast<void *>(-1);
    }
    LOGD("decode_ffmpeg() 7 %s",fmpegUtil->url);
    fmpegUtil->callback->onPrepare(CHILD_THREAD);

    //开始解码
    int count = 0;
    while (1){
        AVPacket* avPacket = av_packet_alloc();//申请每个avPacket的容量
        if (av_read_frame(avFormatContext,avPacket) == 0){
            if (avPacket->stream_index == fmpegUtil->audio->getStreamIndex()){//音频帧
                count++;
                LOGI("read_frame %d",count);
                if (count == 1){
                    fmpegUtil->callback->onStart(CHILD_THREAD);
                }
                //todo
                av_packet_free(&avPacket);
                av_free(avPacket);
            }else{
                av_packet_free(&avPacket);
                av_free(avPacket);
            }
        }else{
            av_packet_free(&avPacket);
            av_free(avPacket);
        }
    }





    //退出线程
    pthread_exit(&fmpegUtil->codec_t);

}



void FFmpeg_util::prepare(){
    LOGD("prepareddd %s",this->url);
    pthread_create(&this->codec_t,NULL,decode_ffmpeg,this);
    LOGD("prepare() %p",&this->codec_t);
    pthread_join(this->codec_t,NULL);


}

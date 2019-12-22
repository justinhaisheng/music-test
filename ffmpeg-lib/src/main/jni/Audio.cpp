//
// Created by haisheng.lu on 2019/12/21.
//

#include "Audio.h"

int Audio::getStreamIndex() const {
    return streamIndex;
}

void Audio::setStreamIndex(int streamIndex) {
    this->streamIndex = streamIndex;
}

AVCodecContext *Audio::getAvCodecContext() const {
    return avCodecContext;
}

void Audio::setAvCodecContext(AVCodecContext *avCodecContext) {
    this->avCodecContext = avCodecContext;
}

AVCodecParameters *Audio::getParameters() {
    return this->parameters;
}

void Audio::setParameters(AVCodecParameters *parameters) {
    this->parameters = parameters;
}

Audio::Audio(Play_status* play_status) {
    LOGD("Audio");
    this->play_status = play_status;
    queue =new Play_queue(play_status);
}

Audio::~Audio() {
    LOGD("~Audio");
}

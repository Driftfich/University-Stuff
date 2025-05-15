#include <QString>
#include <iostream>
#include <vector>
#include "video.h"

using namespace std;

int Video::setType(const QString& type) {
    this->type = type;
    return 0;
}

int Video::setCodec(const QString& codec) {
    this->codec = codec;
    return 0;
}

int Video::setAspectRatio(const QString& aspect_ratio) {
    this->aspect_ratio = aspect_ratio;
    return 0;
}

int Video::setAudioChannels(const QString& audio_channels) {
    this->audio_channels = audio_channels;
    return 0;
}
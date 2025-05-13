#include <iostream>
#include <string>
#include <vector>
#include "video.h"
// #include "config.h"

using namespace std;

int Video::setType(const string& type) {
    this->type = type;
    return 0;
}

int Video::setCodec(const string& codec) {
    this->codec = codec;
    return 0;
}
int Video::setAspectRatio(const string& aspect_ratio) {
    this->aspect_ratio = aspect_ratio;
    return 0;
}
int Video::setAudioChannels(const string& audio_channels) {
    this->audio_channels = audio_channels;
    return 0;
}
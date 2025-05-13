#include <string>
#include <iostream>
#include "audio.h"

using namespace std;

int setType(const string& type) {
    this->type = type;
    return 0;
}

int Audio::setChannels(const string& channels) {
    this->channels = channels;
    return 0;
}

int Audio::setCodec(const string& codec) {
    this->codec = codec;
    return 0;
}

int Audio::setAlbum(const string& album) {
    this->album = album;
    return 0;
}

int Audio::setTracks(const vector<string>& tracks) {
    this->tracks = tracks;
    return 0;
}



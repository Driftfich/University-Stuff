#include <string>
#include <QString>
#include <QVector>
#include <iostream>
#include "audio.h"

using namespace std;

int Audio::setType(const QString& type) {
    this->type = type;
    return 0;
}

int Audio::setChannels(const QString& channels) {
    this->channels = channels;
    return 0;
}

int Audio::setCodec(const QString& codec) {
    this->codec = codec;
    return 0;
}

int Audio::setAlbum(const QString& album) {
    this->album = album;
    return 0;
}

int Audio::setTracks(const QVector<QString>& tracks) {
    this->tracks = tracks;
    return 0;
}



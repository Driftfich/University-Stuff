#include <string>
#include <QString>
#include <QVector>
#include <QDate>
#include <QMap>
#include <QJsonObject>
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

// methods for loading & saving audio data
QJsonObject Audio::getSubclassParams() const {
    QJsonObject json;
    json["duration"] = static_cast<qint64>(this->duration);
    json["type"] = this->type;
    json["bitrate"] = static_cast<qint64>(this->bitrate);
    json["sample_rate"] = static_cast<qint64>(this->sample_rate);
    json["channels"] = this->channels;
    json["codec"] = this->codec;
    json["album"] = this->album;
    json["tracks"] = QJsonArray::fromStringList(this->tracks);
    return json;
}

int Audio::loadSubclassParams(const QJsonObject& json) {
    if (json.contains("duration")) {
        setDuration(json["duration"].toInt());
    }
    if (json.contains("type")) {
        setType(json["type"].toString());
    }
    if (json.contains("bitrate")) {
        setBitrate(json["bitrate"].toInt());
    }
    if (json.contains("sample_rate")) {
        setSampleRate(json["sample_rate"].toInt());
    }
    if (json.contains("channels")) {
        setChannels(json["channels"].toString());
    }
    if (json.contains("codec")) {
        setCodec(json["codec"].toString());
    }
    if (json.contains("album")) {
        setAlbum(json["album"].toString());
    }
    if (json.contains("tracks")) {
        QJsonArray tracksArray = json["tracks"].toArray();
        QVector<QString> tracks;
        for (const QJsonValue& value : tracksArray) {
            tracks.push_back(value.toString());
        }
        setTracks(tracks);
    }
    return 0;
}

QJsonObject Audio::getSubclassSchema() const {
    QJsonObject schema;
    schema["duration"] = QJsonObject{{"type", "integer"}};
    schema["type"] = QJsonObject{{"type", "string"}};
    schema["bitrate"] = QJsonObject{{"type", "integer"}};
    schema["sample_rate"] = QJsonObject{{"type", "integer"}};
    schema["channels"] = QJsonObject{{"type", "string"}};
    schema["codec"] = QJsonObject{{"type", "string"}};
    schema["album"] = QJsonObject{{"type", "string"}};
    schema["tracks"] = QJsonObject{{"type", "array"}, {"items", QJsonObject{{"type", "string"}}}};
    return schema;
}



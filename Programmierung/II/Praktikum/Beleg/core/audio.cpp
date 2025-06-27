/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Implementation file for the Audio class, which holds information and logic related to audio media.
*/

#include <QString>
#include <QVector>
#include <QDate>
#include <QMap>
#include <QJsonObject>
#include <QJsonArray>
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
// collect all audio parameters into a JSON object
QJsonObject Audio::getSubclassParams() const {
    QJsonObject json;
    json["duration"] = static_cast<qint64>(this->duration);
    json["type"] = this->type;
    json["bitrate"] = static_cast<qint64>(this->bitrate);
    json["sample_rate"] = static_cast<qint64>(this->sample_rate);
    json["channels"] = this->channels;
    json["codec"] = this->codec;
    json["album"] = this->album;
    // json["tracks"] = QJsonArray::fromStringList(this->tracks);
    QStringList tracksList = this->tracks.toList();
    json["tracks"] = QJsonArray::fromStringList(tracksList);
    return json;
}

// load the audio parameters from a JSON object
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

// get schema for this class: audio
QJsonObject Audio::getSubclassSchema() {
    QJsonObject schema;
    schema["duration"] = QJsonObject{{"type", "integer"},  {"rename", "Duration"}, {"description", "The duration of the audio in seconds"}, {"minimum", 0}};
    schema["type"] = QJsonObject{{"type", "string"}, {"rename", "Type"}, {"description", "The type of the audio (e.g. podcast, audiobook, music)"}};
    schema["bitrate"] = QJsonObject{{"type", "integer"}, {"rename", "Bitrate"}, {"description", "The bitrate of the audio in kbps"}, {"minimum", 0}};
    schema["sample_rate"] = QJsonObject{{"type", "integer"}, {"rename", "Sample Rate"}, {"description", "The sample rate of the audio in Hz"}, {"minimum", 0}};
    schema["channels"] = QJsonObject{{"type", "string"}, {"rename", "Channels"}, {"description", "The number of channels in the audio (e.g. mono, stereo)"}};
    schema["codec"] = QJsonObject{{"type", "string"}, {"rename", "Codec"}, {"description", "The codec used for the audio"}};
    schema["album"] = QJsonObject{{"type", "string"}, {"rename", "Album"}, {"description", "The album the audio belongs to"}};
    schema["tracks"] = QJsonObject{{"type", "array"}, {"items", QJsonObject{{"type", "string"}}}, {"rename", "Tracks"}, {"description", "The tracks in the audio"}};
    return schema;
}

QJsonObject Audio::getSchema() {
    QJsonObject schema = Media::getSchema();
    QJsonObject properties = schema["properties"].toObject();
    QJsonObject subclassParams = properties["subclass_params"].toObject();
    subclassParams["properties"] = getSubclassSchema();
    properties["subclass_params"] = subclassParams;
    schema["properties"] = properties;
    return schema;
}


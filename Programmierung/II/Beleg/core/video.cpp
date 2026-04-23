/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Implementation file for the Video class, which holds information and logic related to video media.
*/

#include <iostream>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

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

// methods for loading & saving video data
QJsonObject Video::getSubclassParams() const {
    QJsonObject json;
    json["type"] = this->type;
    json["codec"] = this->codec;
    json["aspect_ratio"] = this->aspect_ratio;
    json["audio_channels"] = this->audio_channels;
    return json;
}

Result Video::loadSubclassParams(const QJsonObject& json) {
    Result result = Result::Success();
    if (!json.contains("type") || setType(json["type"].toString()) != 0) {
        result = Result::Error("Failed to set type");
    }
    if (!json.contains("codec") || setCodec(json["codec"].toString()) != 0) {
        result = Result::Error("Failed to set codec");
    }
    if (!json.contains("aspect_ratio") || setAspectRatio(json["aspect_ratio"].toString()) != 0) {
        result = Result::Error("Failed to set aspect ratio");
    }
    if (!json.contains("audio_channels") || setAudioChannels(json["audio_channels"].toString()) != 0) {
        result = Result::Error("Failed to set audio channels");
    }
    return result;
}

// schema methods
QJsonObject Video::getSubclassSchema() {
    QJsonObject schema;
    schema["duration"] = QJsonObject{{"type", "integer"}, {"rename", "Duration"}, {"description", "The duration of the video in seconds"}, {"minimum", 0}};
    schema["type"] = QJsonObject{{"type", "string"}, {"rename", "Type"}, {"description", "The type of the video (e.g. movie, series, documentary)"}};
    schema["resolution"] = QJsonObject{{"type", "integer"}, {"rename", "Resolution"}, {"description", "The resolution of the video (e.g. 1080p, 4K)"}, {"minimum", 1}};
    schema["codec"] = QJsonObject{{"type", "string"}, {"rename", "Codec"}, {"description", "The codec used for the video"}};
    schema["aspect_ratio"] = QJsonObject{{"type", "string"}, {"rename", "Aspect Ratio"}, {"description", "The aspect ratio of the video (e.g. 16:9, 4:3)"}};
    schema["audio_channels"] = QJsonObject{{"type", "string"}, {"rename", "Audio Channels"}, {"description", "The number of audio channels in the video"}};
    return schema;
}

QJsonObject Video::getSchema() {
    QJsonObject schema = Media::getSchema();
    QJsonObject properties = schema["properties"].toObject();
    QJsonObject subclassParams = properties["subclass_params"].toObject();
    subclassParams["properties"] = getSubclassSchema();
    properties["subclass_params"] = subclassParams;
    schema["properties"] = properties;
    return schema;
}
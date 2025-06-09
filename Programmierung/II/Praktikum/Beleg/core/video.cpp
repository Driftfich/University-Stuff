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

// methods for loading & saving video data

QJsonObject Video::getSubclassParams() const {
    QJsonObject json;
    json["type"] = this->type;
    json["codec"] = this->codec;
    json["aspect_ratio"] = this->aspect_ratio;
    json["audio_channels"] = this->audio_channels;
    return json;
}

int Video::loadSubclassParams(const QJsonObject& json) {
    if (json.contains("type")) {
        setType(json["type"].toString());
    }
    if (json.contains("codec")) {
        setCodec(json["codec"].toString());
    }
    if (json.contains("aspect_ratio")) {
        setAspectRatio(json["aspect_ratio"].toString());
    }
    if (json.contains("audio_channels")) {
        setAudioChannels(json["audio_channels"].toString());
    }
    return 0;
}

QJsonObject Video::getSubclassSchema() {
    QJsonObject schema;
    schema["duration"] = QJsonObject{{"type", "integer"}};
    schema["type"] = QJsonObject{{"type", "string"}};
    schema["resolution"] = QJsonObject{{"type", "integer"}};
    schema["codec"] = QJsonObject{{"type", "string"}};
    schema["aspect_ratio"] = QJsonObject{{"type", "string"}};
    schema["audio_channels"] = QJsonObject{{"type", "string"}};
    return schema;
}

QJsonObject Video::getSchema() {
    QJsonObject schema = Media::getSchema();
    QJsonObject subclassParams = schema["subclass_params"].toObject();
    subclassParams["properties"] = getSubclassSchema();
    schema["subclass_params"] = subclassParams;
    return schema;
}
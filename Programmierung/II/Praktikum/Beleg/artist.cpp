#include <iostream>
#include <string>
#include <vector>
#include <QString>
#include <QVector>
#include "media.h"
#include "artist.h"

int Artist::setArtistType(const QString& artist_type) {
    this->artist_type = artist_type;
    return 0;
}

int Artist::setMediaIds(const QVector<unsigned long>& media_ids) {
    this->media_ids = media_ids;
    return 0;
}

QJsonObject Artist::getSubclassParams() const {
    QJsonObject json;
    json["artist_type"] = this->artist_type;
    QJsonArray arr;
    for (auto id : this->media_ids) {
        arr.append(static_cast<qint64>(id));
    }
    json["media_ids"] = arr;
    return json;
}

int Artist::loadSubclassParams(const QJsonObject& json) {
    if (json.contains("artist_type")) {
        setArtistType(json["artist_type"].toString());
    }
    if (json.contains("media_ids")) {
        QJsonArray mediaIdsArray = json["media_ids"].toArray();
        QVector<unsigned long> mediaIds;
        for (const QJsonValue& value : mediaIdsArray) {
            mediaIds.push_back(value.toVariant().toULongLong());
        }
        setMediaIds(mediaIds);
    }
    return 0;
}
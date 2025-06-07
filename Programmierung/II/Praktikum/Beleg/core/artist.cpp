#include <iostream>
#include <string>
#include <vector>
#include <QString>
#include <QVector>
#include <QJsonObject>
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


QJsonObject Artist::getSubclassSchema() {
    QJsonObject properties;
    // artist_type: string
    properties.insert("artist_type", QJsonObject{{"type", "string"}});
    // media_ids: array, dessen items jeweils integer sind
    QJsonObject itemsSchema{{"type", "integer"}};
    QJsonObject mediaIdsSchema{
        {"type", "array"},
        {"items", itemsSchema}
    };
    properties.insert("media_ids", mediaIdsSchema);
    QJsonObject schema;
    schema.insert("type", "object");
    schema.insert("properties", properties);
    schema.insert("optional", "true");

    return schema;
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
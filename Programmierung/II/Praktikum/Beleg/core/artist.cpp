/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Implementation file for the Artist class, which holds information and logic related to artists.
*/

#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include "artist.h"

// setters
int Artist::setArtistType(const QString& artist_type) {
    this->artist_type = artist_type;
    return 0;
}

int Artist::setMediaIds(const QVector<unsigned long>& media_ids) {
    this->media_ids = media_ids;
    return 0;
}

// get all parameters as a JSON object
QJsonObject Artist::getSubclassParams() const {
    QJsonObject json;
    json["artist_type"] = this->artist_type;
    QJsonArray arr;
    for (unsigned long id : this->media_ids) {
        arr.append(static_cast<qint64>(id));
    }
    json["media_ids"] = arr;
    return json;
}

// get the schema for this class. Used to build comprehensive form in infopanel
QJsonObject Artist::getSubclassSchema(bool checked) {
    QJsonObject properties;
    properties.insert("artist_type", QJsonObject{{"type", "string"}});
    QJsonObject itemsSchema{{"type", "integer"}};
    QJsonObject mediaIdsSchema{
        {"type", "array"},
        {"items", itemsSchema}
    };
    properties.insert("media_ids", mediaIdsSchema);
    QJsonObject schema;
    schema.insert("type", "object");
    schema.insert("properties", properties);
    schema.insert("optional", checked); // sets the default visibility, based on how the checkbox is set in the infopanel
    schema.insert("readonly", true); // This means that the artist object is not editable => no further fields can be added or deleted

    return schema;
}

// load parameters from a JSON object
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
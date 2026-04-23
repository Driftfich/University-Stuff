/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Implementation file for the Media class, which holds information and logic related to media items.
*/

#include <QDate>
#include <QString>
#include <QVector>
#include <QMap>

#include "media.h"
#include "config.h"
#include "text.h"
#include "audio.h"
#include "video.h"
#include "returns.h"
#include <iostream>

using namespace std;

// setters for Media class
// set the title with length checks
int Media::setTitle(const QString& title) {
    this->title = title;
    return 0;
}

int Media::setPublicationDate(const QDate publication_date) {
    this->publication_date = publication_date;
    return 0;
}

// set the artist ids with max count checks
int Media::setArtistIds(const QVector<unsigned long>& artist_ids) {
    this->artist_ids = artist_ids;
    return 0;
}

// set the publisher with length checks
int Media::setPublisher(const QString& publisher) {
    this->publisher = publisher;
    return 0;
}

// set the description with length checks
int Media::setDescription(const QString& description) {
    this->description = description;
    return 0;
}

// set the genre with length checks
int Media::setGenre(const QString& genre) {
    this->genre = genre;
    return 0;
}

// set the languages with length and count checks
int Media::setLanguages(const QVector<QString>& languages) {
    this->languages = languages;
    return 0;
}

// set the metadata with length and count checks
int Media::setMetadata(const QMap<QString, QVariant> & newMetadata) {
    this->metadata = newMetadata;
    return 0;
}

// collects all local parameters into a QJsonObject
QJsonObject Media::getLocalParams() const {
    QJsonObject json;
    json["id"] = static_cast<qint64>(this->id);
    json["title"] = this->title;
    json["publication_date"] = this->publication_date.toString(Qt::ISODate);
    QJsonArray artistArray;
    for (unsigned long id : this->artist_ids) {
        artistArray.append(static_cast<qint64>(id));
    }
    json["artist_ids"] = artistArray;
    json["publisher"] = this->publisher;
    json["description"] = this->description;
    json["genre"] = this->genre;
    QVariantList languageVariants;
    for (const QString& lang : this->languages) {
        languageVariants << lang;
    }
    json["languages"] = QJsonArray::fromVariantList(languageVariants);
    json["metadata"] = QJsonObject::fromVariantMap(this->metadata);
    return json;
}

// collects all parameters into a QJsonObject, including subclass type and parameters
QJsonObject Media::getJson() const {
    QJsonObject resjson;
    QJsonObject json = getLocalParams();
    resjson["media"] = json;
    resjson["subclass_type"] = getSubclassType();
    resjson["subclass_params"] = getSubclassParams();
    return resjson;
}

// writes the JSON object to a file
void Media::toFile(QFile& file) const {
    // check if the file is open, if not, try to open it in append mode
    if (!file.isOpen() && !file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        std::cerr << "Error: File could not be opened in append mode" << std::endl;
        return;
    }
    QJsonDocument doc(getJson());
    QByteArray line = doc.toJson(QJsonDocument::Compact) + "\n";
    if (file.write(line) == -1) {
        throw std::runtime_error("Error: Could not write to file");
    }
}

// load local parameters from a JSON object
Result Media::loadLocalParams(const QJsonObject& json) {
    Result result = Result::Success();
    if (!json.contains("id")) {
        result = Result::Error("Failed to set id");
    }
    else {
        setId(json["id"].toVariant().toULongLong());
    }

    if (!json.contains("title") || setTitle(json["title"].toString()) != 0) {
        result = Result::Error("Failed to set title");
    }
    if (!json.contains("publication_date") || setPublicationDate(QDate::fromString(json["publication_date"].toString(), Qt::ISODate)) != 0) {
        result = Result::Error("Failed to set publication date");
    }
    if (json.contains("artist_ids")) {
        QJsonArray artistIdsArray = json["artist_ids"].toArray();
        QVector<unsigned long> artistIds;
        for (const QJsonValue& value : artistIdsArray) {
            artistIds.push_back(value.toVariant().toULongLong());
        }
        if (setArtistIds(artistIds) != 0) {
            result = Result::Error("Failed to set artist ids");
        }
    } else {
        result = Result::Error("Failed to set artist ids");
    }
    if (!json.contains("publisher") || setPublisher(json["publisher"].toString()) != 0) {
        result = Result::Error("Failed to set publisher");
    }
    if (!json.contains("description") || setDescription(json["description"].toString()) != 0) {
        result = Result::Error("Failed to set description");
    }
    if (!json.contains("genre") || setGenre(json["genre"].toString()) != 0) {
        result = Result::Error("Failed to set genre");
    }
    if (json.contains("languages")) {
        QJsonArray languagesArray = json["languages"].toArray();
        QVector<QString> languages;
        for (const QJsonValue& value : languagesArray) {
            languages.push_back(value.toString());
        }
        if (setLanguages(languages) != 0) {
            result = Result::Error("Failed to set languages");
        }
    } else {
        result = Result::Error("Failed to set languages");
    }
    if (json.contains("metadata")) {
        QJsonObject metadataObj = json["metadata"].toObject();
        QMap<QString, QVariant> metadata;
        for (auto it = metadataObj.constBegin(); it != metadataObj.constEnd(); ++it) {
            metadata.insert(it.key(), it.value().toVariant());
        }
        if (setMetadata(metadata) != 0) {
            result = Result::Error("Failed to set metadata");
        }
    } else {
        result = Result::Error("Failed to set metadata");
    }

    return result;
}

// read new media from file
std::shared_ptr<Media> Media::fromFile(QFile& file) {
    if (!file.isOpen() && !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Error: File could not be opened");
    }
    // as long as the file is not at the end, read the next json object from new line delimited file
    while (!file.atEnd()) {
        QByteArray line = file.readLine().trimmed();
        if (line.isEmpty()) continue; // continue if the line is empty

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(line, &err);
        if (err.error != QJsonParseError::NoError) {
            throw std::runtime_error("Error: JSON parse error: " + err.errorString().toStdString());
        }
        if (!doc.isObject()) continue; // skip invalid lines

        QJsonObject obj = doc.object();
        return MediaFactory(obj); // see below
    }
    return nullptr; // EOF reached
}

std::shared_ptr<Media> Media::MediaFactory(const QJsonObject& json) {
    if (!json.contains("subclass_type") || !json["subclass_type"].isString()) {
        throw std::runtime_error("JSON is missing 'subclass_type' or it is not a string");
    }
    QString type = json["subclass_type"].toString();

    if (type == "Text") {
        return std::make_shared<Text>(json); // Constructor takes care of parsing
    }
    else if (type == "Audio") {
        return std::make_shared<Audio>(json);
    }
    else if (type == "Video") {
        return std::make_shared<Video>(json);
    }
    else {
        return std::make_shared<Media>(json); // Fallback to Media if no specific type is found
    }
}

// print method for the base class
void Media::printbase(std::ostream& os) const {
    os << "ID: " << this->id << "\n"
                << "Title: " << this->title.toStdString() << "\n"
                << "Publication Date: " << this->publication_date.toString(Qt::ISODate).toStdString() << "\n"
                << "Publisher: " << this->publisher.toStdString() << "\n"
                << "Description: " << this->description.toStdString() << "\n"
                << "Genre: " << this->genre.toStdString() << "\n"
                << "Languages: ";
    for (const auto& lang : this->languages) {
        os << lang.toStdString() << ", ";
    }
    os << "\n" << "Metadata: ";
    for (auto it = this->metadata.constBegin(); it != this->metadata.constEnd(); ++it) {
        os << it.key().toStdString() << ": " << it.value().toString().toStdString() << ", ";
    }
    os << "\n";
}

void Media::printsubclass(std::ostream& os) const {
    os << "Subclass type: " << getSubclassType().toStdString() << "\n";
    // iterate over the subclass parameters
    QJsonObject subclassParams = getSubclassParams();
    for (auto it = subclassParams.constBegin(); it != subclassParams.constEnd(); ++it) {
        os << it.key().toStdString() << ": " << it.value().toString().toStdString() << "\n";
    }
}

std::ostream& operator<<(std::ostream& os, const Media& media) {
    // use the printbase method to print the base class
    media.printbase(os);
    // use the printsubclass method to print the subclass
    media.printsubclass(os);
    os << "----------------------------------------" << std::endl;
    return os;
}

// schema methods
// get the local schema for the media class
QJsonObject Media::getLocalSchema() {
    QJsonObject schema;
    schema["id"] = QJsonObject{{"type", "integer"}, {"readonly", true}, {"required", true}, {"rename", "Media ID"}, {"description", "Unique identifier for the media"}};
    schema["title"] = QJsonObject{{"type", "string"}, {"rename", "Title"}, {"description", "Title of the media"}};
    schema["publication_date"] = QJsonObject{{"type", "string"}, {"format", "date"}, {"rename", "Publication Date"}, {"description", "Date when the media was published"}};
    schema["artist_ids"] = QJsonObject{{"type", "array"}, {"items", QJsonObject{{"type", "string"}}}, {"rename", "Artist IDs"}, {"description", "List of artist IDs associated with the media"}};
    schema["publisher"] = QJsonObject{{"type", "string"}, {"rename", "Publisher"}, {"description", "Publisher of the media"}};
    schema["description"] = QJsonObject{{"type", "string"}, {"rename", "Description"}, {"description", "Description of the media"}};
    schema["genre"] = QJsonObject{{"type", "string"}, {"rename", "Genre"}, {"description", "Genre of the media"}};
    schema["languages"] = QJsonObject{{"type", "array"}, {"items", QJsonObject{{"type", "string"}}}, {"rename", "Languages"}, {"description", "List of languages the media is available in"}};
    schema["metadata"] = QJsonObject{{"type", "object"}, {"items", QJsonObject{{"type", "string"}}}, {"rename", "Metadata"}, {"description", "Additional metadata for the media"}};
    return schema;
}

// get the complete schema for the media class
QJsonObject Media::getSchema() {
    QJsonObject schema;
    schema.insert("type", "object");
    QJsonObject properties;
    QJsonObject local_schema = getLocalSchema();
    // local_schema.insert("subclass_type", QJsonObject{{"type", "string"}, {"enum", QJsonArray{"Unknown", "Text", "Audio", "Video"}}, {"rename", "Media Type"}, {"description", "Type of the Media (e.g. Text, Audio, Video, Media)"}});
    properties.insert("media", QJsonObject{{"type", "object"}, {"properties", local_schema}, {"rename", "Base Information"}, {"description", "Base information about the media"}});
    properties.insert("subclass_params", QJsonObject{{"type", "object"}, {"properties", QJsonObject{{"readonly", true}}}, {"rename", "Extended Information"}, {"readonly", true}}); // 
    properties.insert("subclass_type", QJsonObject{{"type", "string"}, {"enum", QJsonArray{"Unknown", "Text", "Audio", "Video"}}, {"rename", "Media Type"}, {"description", "Type of the Media (e.g. Text, Audio, Video, Media)"}, {"readonly", true}}); // normally readonly
    schema.insert("properties", properties);
    schema.insert("rename", "Media");
    return schema;
}

// automatically gets the schema for the correct subclass type
QJsonObject Media::getSchemaByType() const {
    QString mediaType = getSubclassType();
    if (mediaType == "Text") {
        return Text::getSchema();
    }
    else if (mediaType == "Audio") {
        return Audio::getSchema();
    }
    else if (mediaType == "Video") {
        return Video::getSchema();
    }
    else {
        return getSchema();
    }
}

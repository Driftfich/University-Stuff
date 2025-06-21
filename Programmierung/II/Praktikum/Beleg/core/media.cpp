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
#include <iostream>

using namespace std;

// setters for Media class
// set the title with length checks
int Media::setTitle(const QString& title) {
    if (MIN_TITLE_LENGTH != -1 && MAX_TITLE_LENGTH != -1 && MIN_TITLE_LENGTH > MAX_TITLE_LENGTH) {
        cerr << "Error: MIN_TITLE_LENGTH > MAX_TITLE_LENGTH" << endl;
        return -1;
    }

    if (MIN_TITLE_LENGTH != -1 && title.length() < MIN_TITLE_LENGTH) {
        cerr << "Error: Title is too short" << endl;
        return -1;
    }

    if (MAX_TITLE_LENGTH != -1 && title.length() > MAX_TITLE_LENGTH) {
        cerr << "Warning: Title is too long, truncating to " << MAX_TITLE_LENGTH << " characters" << endl;
        this->title = title.left(MAX_TITLE_LENGTH);
    }
    else {
        this->title = title;
    }
    return 0;
}

int Media::setPublicationDate(const QDate publication_date) {
    this->publication_date = publication_date;
    return 0;
}

// set the artist ids with max count checks
int Media::setArtistIds(const QVector<unsigned long>& artist_ids) {
    if (artist_ids.size() > MAX_ARTIST_COUNT) {
        cerr << "Warning: Too many artists, truncating to " << MAX_ARTIST_COUNT << " artists" << endl;
        this->artist_ids = QVector<unsigned long>(artist_ids.begin(), artist_ids.begin() + MAX_ARTIST_COUNT);
    }
    this->artist_ids = artist_ids;
    return 0;
}

// set the publisher with length checks
int Media::setPublisher(const QString& publisher) {
    if (MAX_PUBLISHER_LENGTH != -1 && publisher.length() > MAX_PUBLISHER_LENGTH) {
        cerr << "Warning: Publisher is too long, truncating to " << MAX_PUBLISHER_LENGTH << " characters" << endl;
        this->publisher = publisher.left(MAX_PUBLISHER_LENGTH);
    }
    else {
        this->publisher = publisher;
    }
    return 0;
}

// set the description with length checks
int Media::setDescription(const QString& description) {
    if (MAX_DESCRIPTION_LENGTH != -1 && description.length() > MAX_DESCRIPTION_LENGTH) {
        cerr << "Warning: Description is too long, truncating to " << MAX_DESCRIPTION_LENGTH << " characters" << endl;
        this->description = description.left(MAX_DESCRIPTION_LENGTH);
    }

    else {
        this->description = description;
    }

    return 0;
}

// set the genre with length checks
int Media::setGenre(const QString& genre) {
    if (MAX_GENRE_LENGTH != -1 && genre.length() > MAX_GENRE_LENGTH) {
        cerr << "Warning: Genre is too long, truncating to " << MAX_GENRE_LENGTH << " characters" << endl;
        this->genre = genre.left(MAX_GENRE_LENGTH);
    }
    else {
        this->genre = genre;
    }
    return 0;
}

// set the languages with length and count checks
int Media::setLanguages(const QVector<QString>& languages) {
    this->languages.clear();
    size_t num_languages = languages.size();

    if (MAX_LANGUAGE_COUNT != -1 && ((size_t) languages.size()) > MAX_LANGUAGE_COUNT) {
        cerr << "Warning: Too many languages provided (" << languages.size()
             << "), processing only the first " << MAX_LANGUAGE_COUNT << " languages." << endl;
        num_languages = MAX_LANGUAGE_COUNT;
    }

    for (size_t i = 0; i < num_languages; ++i) {
        const QString& lang_str = languages[i];
        if (MAX_LANGUAGE_LENGTH != -1 && ((size_t) lang_str.length()) > static_cast<size_t>(MAX_LANGUAGE_LENGTH)) {
            cerr << "Warning: Language string '" << lang_str.toStdString() << "' is too long (length " << lang_str.length()
                 << "), truncating to " << MAX_LANGUAGE_LENGTH << " characters." << endl;
            this->languages.push_back(lang_str.left(static_cast<size_t>(MAX_LANGUAGE_LENGTH)));
        } else {
            this->languages.push_back(lang_str);
        }
    }
    return 0;
}

// set the metadata with length and count checks
int Media::setMetadata(const QMap<QString, QVariant> & newMetadata) {
    metadata.clear();
    int processed = 0;
    for (auto it = newMetadata.constBegin(); it != newMetadata.constEnd(); ++it) {
        if (MAX_METADATA_COUNT != -1 && processed >= MAX_METADATA_COUNT) {
            std::cerr << "Warning: Max metadata count exceeded, ignore rest\n";
            break;
        }
        const QString& key = it.key();
        QVariant value = it.value();

        // truncate strings if too long
        if (MAX_METADATA_LENGTH != -1 && value.typeId() == QMetaType::QString) {
            QString s = value.toString();
            if (s.length() > MAX_METADATA_LENGTH) {
                std::cerr << "Warning: Metadata '" << key.toStdString()
                          << "' too long, truncating\n";
                s.truncate(MAX_METADATA_LENGTH);
                value = s;
            }
        }
        metadata.insert(key, value);
        ++processed;
    }
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
int Media::loadLocalParams(const QJsonObject& json) {
    // qDebug() << json;
    if (json.contains("id")) {
        setId(json["id"].toVariant().toULongLong());
    } else {
        std::cerr << "Error: Missing 'id' in media JSON object\n";
        return -1;
    }

    if (json.contains("title")) {
        setTitle(json["title"].toString());
    } else {
        std::cerr << "Error: Missing 'title' in JSON object\n";
        return -1;
    }
    if (json.contains("publication_date")) {
        setPublicationDate(QDate::fromString(json["publication_date"].toString(), Qt::ISODate));
    } else {
        std::cerr << "Error: Missing 'publication_date' in JSON object\n";
        return -1;
    }
    if (json.contains("artist_ids")) {
        QJsonArray artistIdsArray = json["artist_ids"].toArray();
        QVector<unsigned long> artistIds;
        for (const QJsonValue& value : artistIdsArray) {
            artistIds.push_back(value.toVariant().toULongLong());
        }
        setArtistIds(artistIds);
    } else {
        std::cerr << "Error: Missing 'artist_ids' in JSON object\n";
        return -1;
    }
    if (json.contains("publisher")) {
        setPublisher(json["publisher"].toString());
    } else {
        std::cerr << "Error: Missing 'publisher' in JSON object\n";
        return -1;
    }
    if (json.contains("description")) {
        setDescription(json["description"].toString());
    } else {
        std::cerr << "Error: Missing 'description' in JSON object\n";
        return -1;
    }
    if (json.contains("genre")) {
        setGenre(json["genre"].toString());
    } else {
        std::cerr << "Error: Missing 'genre' in JSON object\n";
        return -1;
    }
    if (json.contains("languages")) {
        QJsonArray languagesArray = json["languages"].toArray();
        QVector<QString> languages;
        for (const QJsonValue& value : languagesArray) {
            languages.push_back(value.toString());
        }
        setLanguages(languages);
    } else {
        std::cerr << "Error: Missing 'languages' in JSON object\n";
        return -1;
    }
    if (json.contains("metadata")) {
        QJsonObject metadataObj = json["metadata"].toObject();
        QMap<QString, QVariant> metadata;
        for (auto it = metadataObj.constBegin(); it != metadataObj.constEnd(); ++it) {
            metadata.insert(it.key(), it.value().toVariant());
        }
        setMetadata(metadata);
    } else {
        std::cerr << "Error: Missing 'metadata' in JSON object\n";
        return -1;
    }

    return 0;
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
    schema["title"] = QJsonObject{{"type", "string"}, {"minLength", MIN_TITLE_LENGTH}, {"maxLength", MAX_TITLE_LENGTH}, {"rename", "Title"}, {"description", "Title of the media"}};
    schema["publication_date"] = QJsonObject{{"type", "string"}, {"format", "date"}, {"rename", "Publication Date"}, {"description", "Date when the media was published"}};
    schema["artist_ids"] = QJsonObject{{"type", "array"}, {"items", QJsonObject{{"type", "integer"}}}, {"rename", "Artist IDs"}, {"description", "List of artist IDs associated with the media"}};
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

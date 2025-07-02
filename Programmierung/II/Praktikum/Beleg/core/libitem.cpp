/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Implementation file for the Libitem class, which holds information and logic related to library items.
*/

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "libitem.h"
#include "returns.h"

QJsonObject Libitem::getJson() const {
    QJsonObject json;
    json["id"] = static_cast<qint64>(id);
    json["media_id"] = static_cast<qint64>(media_id);
    json["available_copies"] = static_cast<qint64>(available_copies);
    // json["borrowed_copies"] = static_cast<qint64>(borrowed_copies);
    json["location"] = location;
    json["condition"] = condition;
    return json;
}

void Libitem::toFile(QFile& file) const {
    // check if the file is open, if not, try to open it in append mode
    if (!file.isOpen() && !file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        std::cerr << "Error: File could not be opened in append mode" << std::endl;
        return;
    }
    // write the JSON object to the file
    QJsonDocument doc(getJson());
    QByteArray line = doc.toJson(QJsonDocument::Compact) + "\n";
    if (file.write(line) == -1) {
        throw std::runtime_error("Error: Could not write to file");
    }
}

// load local parameters from a JSON object
Result Libitem::loadLocalParams(const QJsonObject& json) {
    Result result = Result::Success();
    if (!json.contains("id") || setId(json["id"].toVariant().toULongLong()) != 0) {
        result = Result::Error("Failed to set id");
    }

    if (!json.contains("media_id") || setMediaId(json["media_id"].toVariant().toULongLong()) != 0) {
        result = Result::Error("Failed to set media id");
    }

    if (!json.contains("available_copies") || setAvailableCopies(json["available_copies"].toVariant().toULongLong()) != 0) {
        result = Result::Error("Failed to set available copies");
    }

    if (!json.contains("location") || setLocation(json["location"].toString()) != 0) {
        result = Result::Error("Failed to set location");
    }

    if (!json.contains("condition") || setCondition(json["condition"].toString()) != 0) {
        result = Result::Error("Failed to set condition");
    }
    return result;
}
// read new libitem from file
std::shared_ptr<Libitem> Libitem::fromFile(QFile& file, std::function<void(unsigned long libitemId, unsigned long oldMediaId, unsigned long newMediaId)> onMediaChangeCallback) {
    // check if the file is open, if not, try to open it in read mode
    if (!file.isOpen() && !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Error: File could not be opened");
    }
    // as long as the file is not at the end, read the next json object
    while (!file.atEnd()) {
        // read a line from the file, trim it and check if it is empty
        QByteArray line = file.readLine().trimmed();
        if (line.isEmpty()) continue;
        QJsonDocument doc(QJsonDocument::fromJson(line));
        if (doc.isNull()) {
            std::cerr << "Error: Invalid JSON format" << std::endl;
            return nullptr;
        }
        QJsonObject obj = doc.object();
        // create a new Libitem object from the JSON object
        return LibitemFactory(obj, onMediaChangeCallback);
    }
    return nullptr;
}

// factory method
std::shared_ptr<Libitem> Libitem::LibitemFactory(const QJsonObject& json, std::function<void(unsigned long libitemId, unsigned long oldMediaId, unsigned long newMediaId)> onMediaChangeCallback) {
    // call the constructor with the json object
    std::shared_ptr<Libitem> item = std::make_shared<Libitem>(json, onMediaChangeCallback);
    return item;
}

std::ostream& operator<<(std::ostream& os, const Libitem& libitem) {
    os << "Libitem ID: " << libitem.id << "\n"
       << "Media ID: " << libitem.media_id << "\n"
       << "Available Copies: " << libitem.available_copies << "\n"
       // << "Borrowed Copies: " << libitem.borrowed_copies << "\n"
       << "Location: " << libitem.location.toStdString() << "\n"
       << "Condition: " << libitem.condition.toStdString() << "\n"
       << "------------------------\n";
    return os;
}

// schema methods
QJsonObject Libitem::getLocalSchema() {
    QJsonObject schema;
    schema["id"] = QJsonObject{{"type", "integer"}, {"readonly", true}, {"required", true}, {"rename", "Libitem ID"}, {"description", "Unique identifier for the library item"}};
    schema["media_id"] = QJsonObject{{"type", "string"}, {"required", true}, {"rename", "Media ID"}, {"description", "Unique identifier for the media"}, {"pattern", "^[0-9]{1,19}$"}, {"placeholder", "Enter ID or search for media"}};
    schema["available_copies"] = QJsonObject{{"type", "integer"}, {"rename", "Available Copies"}, {"description", "Number of available copies"}};
    schema["borrowed_copies"] = QJsonObject{{"type", "integer"}, {"readonly", true}, {"rename", "Borrowed Copies"}, {"description", "Number of borrowed copies"}};
    schema["location"] = QJsonObject{{"type", "string"}, {"rename", "Location"}, {"description", "Location of the library item"}};
    schema["condition"] = QJsonObject{{"type", "string"}, {"enum", QJsonArray{"new", "good", "acceptable", "poor"}}, {"rename", "Condition"}, {"description", "Condition of the library item"}};
    return schema;
}

QJsonObject Libitem::getSchema() {
    QJsonObject schema;
    schema["properties"] = getLocalSchema();
    schema["type"] = "object";
    schema["rename"] = "Library Item";
    schema["description"] = "A library item that can be borrowed by users. It contains a reference to a media item and additional information about its condition and availability.";
    return schema;
}
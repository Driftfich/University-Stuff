#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "libitem.h"


QJsonObject Libitem::getJson() const {
    QJsonObject json;
    json["id"] = static_cast<qint64>(id);
    json["media_id"] = static_cast<qint64>(media_id);
    json["available_copies"] = static_cast<qint64>(available_copies);
    json["borrowed_copies"] = static_cast<qint64>(borrowed_copies);
    json["location"] = location;
    json["condition"] = condition;
    return json;
}
void Libitem::toFile(QFile& file) const {
    if (!file.isOpen() && !file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        std::cerr << "Fehler: Datei konnte nicht im Append-Modus geöffnet werden" << std::endl;
        return;
    }
    QJsonDocument doc(getJson());
    QByteArray line = doc.toJson(QJsonDocument::Compact) + "\n";
    if (file.write(line) == -1) {
        throw std::runtime_error("Fehler beim Schreiben in die Datei");
    }
}

// load json object from file
int Libitem::loadLocalParams(const QJsonObject& json) {
    if (json.contains("id")) {
        setId(json["id"].toVariant().toULongLong());
    } else {
        std::cerr << "Error: Missing 'id' in JSON object\n";
        return -1;
    }

    if (json.contains("media_id")) {
        setMediaId(json["media_id"].toVariant().toULongLong());
    } else {
        std::cerr << "Error: Missing 'media_id' in JSON object\n";
        return -1;
    }

    if (json.contains("available_copies")) {
        setAvailableCopies(json["available_copies"].toVariant().toULongLong());
    } else {
        std::cerr << "Error: Missing 'available_copies' in JSON object\n";
        return -1;
    }

    if (json.contains("borrowed_copies")) {
        setBorrowedCopies(json["borrowed_copies"].toVariant().toULongLong());
    } else {
        std::cerr << "Error: Missing 'borrowed_copies' in JSON object\n";
        return -1;
    }

    if (json.contains("location")) {
        setLocation(json["location"].toString());
    } else {
        std::cerr << "Error: Missing 'location' in JSON object\n";
        return -1;
    }

    if (json.contains("condition")) {
        setCondition(json["condition"].toString());
    } else {
        std::cerr << "Error: Missing 'condition' in JSON object\n";
        return -1;
    }

    // std::cout << "Loaded Libitem Params" << std::endl;
    
    return 0;
}
// read json object from file
std::shared_ptr<Libitem> Libitem::fromFile(QFile& file) {
    if (!file.isOpen() && !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Fehler: Datei konnte nicht geöffnet werden");
    }
    while (!file.atEnd()) {
        QByteArray line = file.readLine().trimmed();
        if (line.isEmpty()) continue;
        QJsonDocument doc(QJsonDocument::fromJson(line));
        if (doc.isNull()) {
            std::cerr << "Fehler: Ungültiges JSON-Format" << std::endl;
            return nullptr;
        }
        QJsonObject obj = doc.object();
        return LibitemFactory(obj);
    }
    return nullptr;
}

// factory method
std::shared_ptr<Libitem> Libitem::LibitemFactory(const QJsonObject& json) {
    // call the constructor with the json object
    // std::cout << "Reached LibitemFactory" << std::endl;
    std::shared_ptr<Libitem> item = std::make_shared<Libitem>(json);
    return item;
}

std::ostream& operator<<(std::ostream& os, const Libitem& libitem) {
    os << "Libitem ID: " << libitem.id << "\n"
       << "Media ID: " << libitem.media_id << "\n"
       << "Available Copies: " << libitem.available_copies << "\n"
       << "Borrowed Copies: " << libitem.borrowed_copies << "\n"
       << "Location: " << libitem.location.toStdString() << "\n"
       << "Condition: " << libitem.condition.toStdString() << "\n"
       << "------------------------\n";
    return os;
}
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <QDateTime>
#include "transaction.h"

QJsonObject Transaction::getJson() const {
    QJsonObject json;
    json["id"] = static_cast<qint64>(id);
    json["libitem_id"] = static_cast<qint64>(libitem_id);
    json["borrower_id"] = static_cast<qint64>(borrower_id);
    json["transaction_time"] = transaction_time.toString();
    return json;
}

void Transaction::toFile(QFile& file) const {
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

int Transaction::loadLocalParams(const QJsonObject& json) {
    if (json.contains("id") && json["id"].isDouble()) {
        id = static_cast<unsigned long>(json["id"].toDouble());
    }
    if (json.contains("libitem_id") && json["libitem_id"].isDouble()) {
        libitem_id = static_cast<unsigned long>(json["libitem_id"].toDouble());
    }
    if (json.contains("borrower_id") && json["borrower_id"].isDouble()) {
        borrower_id = static_cast<unsigned long>(json["borrower_id"].toDouble());
    }
    if (json.contains("transaction_time") && json["transaction_time"].isString()) {
        transaction_time = QDateTime::fromString(json["transaction_time"].toString());
    }
    return 0;
}

std::shared_ptr<Transaction> Transaction::fromFile(QFile& file) {
    if (!file.isOpen() && !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Fehler: Datei konnte nicht geöffnet werden");
    }
    while (!file.atEnd()) {
        QByteArray line = file.readLine().trimmed();
        QJsonDocument doc = QJsonDocument::fromJson(line);
        if (doc.isNull()) {
            std::cerr << "Fehler: Ungültiges JSON-Dokument" << std::endl;
            return nullptr;
        }
        QJsonObject json = doc.object();
        if (json.isEmpty()) {
            std::cerr << "Fehler: Leeres JSON-Objekt" << std::endl;
            return nullptr;
        }
        return TransactionFactory(json);
    }

    return nullptr;
}

std::shared_ptr<Transaction> Transaction::TransactionFactory(const QJsonObject& json) {
    std::shared_ptr<Transaction> transaction = std::make_shared<Transaction>(json);
    return transaction;
}

// schema methods
QJsonObject Transaction::getLocalSchema() const {
    QJsonObject schema;
    schema["id"] = QJsonObject{{"type", "integer"}, {"readonly", true}};
    schema["libitem_id"] = QJsonObject{{"type", "integer"}};
    schema["borrower_id"] = QJsonObject{{"type", "integer"}};
    schema["transaction_time"] = QJsonObject{{"type", "string"}, {"format", "date-time"}};
    return schema;
}

QJsonObject Transaction::getSchema() const {
    QJsonObject schema;
    schema["transaction"] = getLocalSchema();
    return schema;
}
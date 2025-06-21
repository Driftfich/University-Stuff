/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Implementation file for the Transaction class, which holds information and logic related to transactions.
*/

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>

#include "transaction.h"

QJsonObject Transaction::getJson() const {
    QJsonObject json;
    json["id"] = static_cast<qint64>(id);
    json["libitem_id"] = static_cast<qint64>(libitem_id);
    json["borrower_id"] = static_cast<qint64>(borrower_id);
    json["transaction_time"] = transaction_time.toString("yyyy-MM-dd hh:mm:ss");
    return json;
}

void Transaction::toFile(QFile& file) const {
    // Open the file for writing in append mode when not already open
    if (!file.isOpen() && !file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        std::cerr << "Fehler: Datei konnte nicht im Append-Modus geöffnet werden" << std::endl;
        return;
    }

    // get the JSON representation of the transaction
    QJsonDocument doc(getJson());
    QByteArray line = doc.toJson(QJsonDocument::Compact) + "\n";
    // Write the JSON line to the file
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
        // qDebug() << "Parsing Transaction Time " << json["transaction_time"].toString();
        transaction_time = QDateTime::fromString(json["transaction_time"].toString(), "yyyy-MM-dd hh:mm:ss");
        // qDebug() << "Parsed Transaction Time: " << transaction_time.toString();
    }
    return 0;
}

std::shared_ptr<Transaction> Transaction::fromFile(QFile& file) {
    if (!file.isOpen() && !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Fehler: Datei konnte nicht geöffnet werden");
    }
    // as long as the file is not at the end, read next new line delimited JSON object
    while (!file.atEnd()) {
        // parse the json object
        QByteArray line = file.readLine().trimmed();
        QJsonDocument doc = QJsonDocument::fromJson(line);
        // when the document is not valid, return nullptr
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
QJsonObject Transaction::getLocalSchema() {
    /*
    Libitem id, borrower id and transaction id are readonly by default so no more changes can be done in the infopanel.
    For the addpanel the libitem id and borrower id are made editable in the getDefaultSchema method in the transactiontablemodel.cpp file.
    */
    QJsonObject schema;
    schema["id"] = QJsonObject{{"type", "integer"}, {"readonly", true}, {"required", true}, {"rename", "Transaction ID"}, {"description", "Unique identifier for the transaction"}};
    schema["libitem_id"] = QJsonObject{{"type", "string"}, {"readonly", true}, {"required", true}, {"rename", "Library Item ID"}, {"description", "Unique identifier for the library item"}, {"pattern", "^[0-9]+$"}, {"placeholder", "Enter ID or search for library item"}};
    schema["borrower_id"] = QJsonObject{{"type", "string"}, {"readonly", true}, {"required", true}, {"rename", "Borrower ID"}, {"description", "Unique identifier for the borrower"}, {"pattern", "^[0-9]+$"}, {"placeholder", "Enter ID or search for borrower"}};
    schema["transaction_time"] = QJsonObject{{"type", "string"}, {"format", "datetime"}, {"required", true}, {"rename", "Transaction Time"}, {"description", "Time when the transaction occurred"}};
    return schema;
}

QJsonObject Transaction::getSchema() {
    QJsonObject schema;
    schema.insert("type", "object");
    schema.insert("properties", getLocalSchema());
    return schema;
}
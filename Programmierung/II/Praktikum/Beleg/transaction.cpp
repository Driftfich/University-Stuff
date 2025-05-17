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
    if (!file.isOpen()) {
        file.open(QIODevice::WriteOnly);
    }
    QJsonDocument doc(getJson());
    file.write(doc.toJson());
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
    if (!file.isOpen()) {
        file.open(QIODevice::ReadOnly);
    }
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isObject()) {
        QJsonObject json = doc.object();
        return TransactionFactory(json);
    }
    return nullptr;
}

std::shared_ptr<Transaction> Transaction::TransactionFactory(const QJsonObject& json) {
    std::shared_ptr<Transaction> transaction = std::make_shared<Transaction>(json);
    return transaction;
}
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include "borrower.h"

QJsonObject Borrower::getSubclassParams() const {
    QJsonObject json;
    json["limit"] = static_cast<qint64>(limit);
    json["bow_id"] = static_cast<qint64>(bow_id);
    return json;
}

int Borrower::loadSubclassParams(const QJsonObject& json) {
    if (json.contains("limit")) {
        setLimit(static_cast<unsigned int>(json["limit"].toVariant().toLongLong()));
    }
    if (json.contains("bow_id")) {
        setBowId(static_cast<unsigned long>(json["bow_id"].toVariant().toLongLong()));
    }
    return 0;
}

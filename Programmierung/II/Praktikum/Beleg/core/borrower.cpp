#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include "borrower.h"

QJsonObject Borrower::getSubclassParams() const {
    QJsonObject json;
    json["limit"] = static_cast<qint64>(limit);
    // json["bow_id"] = static_cast<qint64>(bow_id);
    return json;
}

QJsonObject Borrower::getSubclassSchema() {
    QJsonObject properties;
    properties["limit"] = QJsonObject{{"type", "integer"}, {"minimum", 0}, {"maximum", MAX_ITEMS_PER_BORROWER_HARD}};
    // properties["bow_id"] = QJsonObject{{"type", "integer"}, {"minimum", 0}};
    QJsonObject schema;
    schema.insert("type", "object");
    schema.insert("properties", properties);
    return schema;
}

int Borrower::loadSubclassParams(const QJsonObject& json) {
    if (json.contains("limit")) {
        setLimit(static_cast<unsigned int>(json["limit"].toVariant().toLongLong()));
    }
    // if (json.contains("bow_id")) {
    //     setBowId(static_cast<unsigned long>(json["bow_id"].toVariant().toLongLong()));
    // }
    return 0;
}

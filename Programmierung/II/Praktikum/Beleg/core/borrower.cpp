/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Implementation file for the Borrower class, which holds information and logic related to borrowers.
*/

#include <QJsonObject>
#include <QJsonDocument>
#include "borrower.h"
#include "returns.h"

QJsonObject Borrower::getSubclassParams() const {
    QJsonObject json;
    json["limit"] = static_cast<qint64>(limit);
    return json;
}

QJsonObject Borrower::getSubclassSchema(bool checked) {
    QJsonObject properties;
    properties["limit"] = QJsonObject{{"type", "integer"}, {"minimum", 0}, {"maximum", MAX_ITEMS_PER_BORROWER_HARD}};
    QJsonObject schema;
    schema.insert("type", "object");
    schema.insert("properties", properties);
    schema.insert("optional", checked);
    schema.insert("readonly", true); // don't allow deleting or adding fields in the borrower
    return schema;
}

Result Borrower::loadSubclassParams(const QJsonObject& json) {
    if (!json.contains("limit") || setLimit(static_cast<unsigned int>(json["limit"].toVariant().toLongLong())) != 0) {
        return Result::Error("Failed to set limit");
    }
    return Result::Success();
}

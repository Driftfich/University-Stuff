#ifndef JSONSCHEMAUTILS_H
#define JSONSCHEMAUTILS_H

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QString>

QJsonObject createDefaultJsonFromSchema(const QJsonObject& schemaObject);
QJsonValue createDefaultValueFromFieldSchema(const QJsonObject& fieldSchema);

#endif // JSONSCHEMAUTILS_H

#ifndef JSONSCHEMAUTILS_H
#define JSONSCHEMAUTILS_H

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QString>

static QJsonObject createDefaultJsonFromSchema(const QJsonObject& schemaObject);
static QJsonValue createDefaultValueFromFieldSchema(const QJsonObject& fieldSchema);

#endif // JSONSCHEMAUTILS_H

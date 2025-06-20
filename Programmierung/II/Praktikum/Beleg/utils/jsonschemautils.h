/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: This file contains the functions to create a default json object from a schema object and a default value from a field schema.
*/

#ifndef JSONSCHEMAUTILS_H
#define JSONSCHEMAUTILS_H

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QString>

QJsonObject createDefaultJsonFromSchema(const QJsonObject& schemaObject);
QJsonValue createDefaultValueFromFieldSchema(const QJsonObject& fieldSchema);

#endif // JSONSCHEMAUTILS_H

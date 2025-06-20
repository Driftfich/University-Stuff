/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: This file contains the functions to create a default json object from a schema object and a default value from a field schema.
*/

#include "jsonschemautils.h"

// create a default json object from a schema object
QJsonObject createDefaultJsonFromSchema(const QJsonObject& schemaObject) {
    QJsonObject defaultJson;
    
    // When a property is available, use it, otherwise use the schema object
    QJsonObject properties;
    if (schemaObject.contains("properties") && schemaObject["properties"].isObject()) {
        properties = schemaObject["properties"].toObject();
    } else {
        // If no "properties" is available, use the schema object
        properties = schemaObject;
    }
    
    // Iterate over all properties in the schema
    for (QJsonObject::const_iterator it = properties.constBegin(); it != properties.constEnd(); ++it) {
        const QString& fieldName = it.key();
        const QJsonValue& fieldSchema = it.value();
        
        if (!fieldSchema.isObject()) {
            continue; // Skip non-object schemas
        }
        
        QJsonObject fieldSchemaObj = fieldSchema.toObject();
        
        // Determine the default value based on the type
        QJsonValue defaultValue = createDefaultValueFromFieldSchema(fieldSchemaObj);
        
        // set the default value when given
        if (!defaultValue.isNull()) {
            defaultJson[fieldName] = defaultValue;
        }
    }
    
    return defaultJson;
}

QJsonValue createDefaultValueFromFieldSchema(const QJsonObject& fieldSchema) {
    QString type = fieldSchema["type"].toString();
    
    // Handle enum values first (have priority over the base type)
    if (fieldSchema.contains("enum") && fieldSchema["enum"].isArray()) {
        QJsonArray enumValues = fieldSchema["enum"].toArray();
        if (!enumValues.isEmpty()) {
            return enumValues.first(); // Use the first enum value as default
        }
    }
    
    // Handle different base types
    if (type == "string") {
        return QString(""); // Empty string as default
    } else if (type == "integer" || type == "number") {
        // Check for minimum value in the schema
        if (fieldSchema.contains("minimum")) {
            return fieldSchema["minimum"];
        }
        return 0; // Default number is 0
    } else if (type == "boolean") {
        return false; // Default boolean is false
    } else if (type == "array") {
        return QJsonArray(); // Empty array as default
    } else if (type == "object") {
        // For nested objects, recursive creation
        if (fieldSchema.contains("properties")) {
            return createDefaultJsonFromSchema(fieldSchema);
        }
        return QJsonObject(); // Empty object as default
    }
    
    // If the type is unknown, return an empty string
    return QString("");
}

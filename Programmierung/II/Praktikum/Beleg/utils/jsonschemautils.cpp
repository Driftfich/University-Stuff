#include "jsonschemautils.h"

/**
 * @brief Erstellt ein Standard-JSON-Objekt basierend auf einem Schema
 * @param schemaObject Das Schema-Objekt, das die Struktur definiert
 * @return Ein JSON-Objekt mit allen Schema-Feldern und Standard-Werten
 * 
 * Diese statische Methode durchläuft rekursiv ein Schema-Objekt und erstellt ein 
 * JSON-Objekt mit allen definierten Feldern. Felder erhalten Standard-Werte
 * basierend auf ihrem Typ. Readonly-Felder werden übersprungen, da sie normalerweise
 * auto-generiert werden (z.B. IDs).
 */
QJsonObject createDefaultJsonFromSchema(const QJsonObject& schemaObject) {
    QJsonObject defaultJson;
    
    // Falls das Schema ein "properties" Objekt hat, verwende dieses
    QJsonObject properties;
    if (schemaObject.contains("properties") && schemaObject["properties"].isObject()) {
        properties = schemaObject["properties"].toObject();
    } else {
        // Falls kein "properties" vorhanden, verwende das Schema direkt
        properties = schemaObject;
    }
    
    // Durchlaufe alle Eigenschaften im Schema
    for (QJsonObject::const_iterator it = properties.constBegin(); it != properties.constEnd(); ++it) {
        const QString& fieldName = it.key();
        const QJsonValue& fieldSchema = it.value();
        
        if (!fieldSchema.isObject()) {
            continue; // Überspringe nicht-objekt Schemas
        }
        
        QJsonObject fieldSchemaObj = fieldSchema.toObject();
        
        // Überspringe readonly Felder (meist auto-generierte IDs)
        // if (fieldSchemaObj.contains("readonly") && fieldSchemaObj["readonly"].toBool()) {
        //     continue;
        // }
        
        // Bestimme den Standard-Wert basierend auf dem Typ
        QJsonValue defaultValue = createDefaultValueFromFieldSchema(fieldSchemaObj);
        
        if (!defaultValue.isNull()) {
            defaultJson[fieldName] = defaultValue;
        }
    }
    
    return defaultJson;
}

/**
 * @brief Erstellt einen Standard-Wert für ein einzelnes Schema-Feld
 * @param fieldSchema Das Schema-Objekt für ein einzelnes Feld
 * @return Der Standard-Wert für dieses Feld basierend auf seinem Typ
 * 
 * Diese Hilfsmethode bestimmt den passenden Standard-Wert für ein Feld
 * basierend auf seinem Typ und anderen Schema-Eigenschaften.
 */
QJsonValue createDefaultValueFromFieldSchema(const QJsonObject& fieldSchema) {
    QString type = fieldSchema["type"].toString();
    
    // Behandle enum-Werte zuerst (haben Vorrang vor dem Basis-Typ)
    if (fieldSchema.contains("enum") && fieldSchema["enum"].isArray()) {
        QJsonArray enumValues = fieldSchema["enum"].toArray();
        if (!enumValues.isEmpty()) {
            return enumValues.first(); // Verwende den ersten enum-Wert als Standard
        }
    }
    
    // Behandle verschiedene Basis-Typen
    if (type == "string") {
        return QString(""); // Leerer String als Standard
    } else if (type == "integer" || type == "number") {
        // Prüfe auf minimum-Wert im Schema
        if (fieldSchema.contains("minimum")) {
            return fieldSchema["minimum"];
        }
        return 0; // Standard-Zahl ist 0
    } else if (type == "boolean") {
        return false; // Standard-Boolean ist false
    } else if (type == "array") {
        return QJsonArray(); // Leeres Array als Standard
    } else if (type == "object") {
        // Für verschachtelte Objekte, rekursive Erstellung
        if (fieldSchema.contains("properties")) {
            return createDefaultJsonFromSchema(fieldSchema);
        }
        return QJsonObject(); // Leeres Objekt als Standard
    }
    
    // Falls der Typ unbekannt ist, gib leeren String zurück
    return QString("");
}

/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Header file for the Text class, which holds information and logic related to text media.
*/

#include <iostream>
#include <QString>

#include "text.h"

using namespace std;

int Text::setIsbn(const QString& isbn) {
    // Check that the isbn exists and is not too short
    if (isbn.length() != 13) {
        cerr << "Error: ISBN must be 13 characters long" << endl;
        return -1;
    }
    
    // Check that all characters are digits
    for (int i = 0; i < 13; i++) {
        if (!isbn[i].isDigit()) {
            cerr << "Error: ISBN must contain only digits" << endl;
            return -1;
        }
    }
    // use ISBN-13 check digit validation
    int sum = 0;
    for (int i = 0; i < 12; i++) {
        if (i % 2 == 0) {
            sum += isbn[i].digitValue();
        } else {
            sum += isbn[i].digitValue() * 3;
        }
    }
    
    // Calculate the expected check digit
    int calculatedCheckDigit = (10 - (sum % 10)) % 10;
    
    // Get the actual check digit (13th digit)
    int actualCheckDigit = isbn[12].digitValue();
    
    // Validate that calculated and actual check digits match
    if (calculatedCheckDigit != actualCheckDigit) {
        cerr << "Error: ISBN check digit is invalid (expected " 
             << calculatedCheckDigit << ", got " << actualCheckDigit << ")" << endl;
        return -1;
    }
    this->isbn = isbn;
    return 0;
}

int Text::setBindingType(const QString& bindingType) {
    this->bindingType = bindingType;
    return 0;
}

int Text::setTextFormat(const QString& textFormat) {
    this->textFormat = textFormat;
    return 0;
}

// collects all parameters into a QJsonObject
QJsonObject Text::getSubclassParams() const {
    QJsonObject json;
    json["pages"] = pages;
    json["isbn"] = isbn;
    json["bindingType"] = bindingType;
    json["textFormat"] = textFormat;
    return json;
}

// Load subclass parameters from a JSON object. Overrides the base class method.
Result Text::loadSubclassParams(const QJsonObject& json) {
    Result result = Result::Success();
    if (!json.contains("pages") || setPages(json["pages"].toInt()) != 0) {
        result = Result::Error("Failed to set pages");
    }
    if (!json.contains("isbn") || setIsbn(json["isbn"].toString()) != 0) {
        result = Result::Error("Failed to set isbn");
    }
    if (!json.contains("bindingType") || setBindingType(json["bindingType"].toString()) != 0) {
        result = Result::Error("Failed to set binding type");
    }
    if (!json.contains("textFormat") || setTextFormat(json["textFormat"].toString()) != 0) {
        result = Result::Error("Failed to set text format");
    }
    return result;
}

// Collects the schema for the subclass parameters
QJsonObject Text::getSubclassSchema() {
    QJsonObject schema;
    schema["pages"] = QJsonObject{{"type", "integer"}, {"rename", "Number of Pages"},  {"description", "The number of pages in the text"}, {"minimum", 1}};
    schema["isbn"] = QJsonObject{{"type", "integer"}, {"rename", "ISBN"}, {"description", "The ISBN of the text"}, {"pattern", "^[0-9]{13}$"}};
    schema["bindingType"] = QJsonObject{{"type", "string"}, {"rename", "Binding Type"}, {"description", "The binding type of the text"}};
    schema["textFormat"] = QJsonObject{{"type", "string"}, {"rename", "Text Format"}, {"description", "The text format of the text"}};
    return schema;
}

// Returns complete the schema for the Text class. First gets the base class schema and overrides the subclass schema.
QJsonObject Text::getSchema() {
    QJsonObject schema = Media::getSchema();
    QJsonObject properties = schema["properties"].toObject();
    QJsonObject subclassParams = properties["subclass_params"].toObject();
    subclassParams["properties"] = getSubclassSchema();
    properties["subclass_params"] = subclassParams;
    schema["properties"] = properties;
    return schema;
}
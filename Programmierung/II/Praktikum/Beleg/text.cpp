#include <QString>
#include <iostream>
#include "text.h"

using namespace std;

int Text::setIsbn(const QString& isbn) {
    if (isbn.length() != 13) {
        cerr << "Error: ISBN must be 13 characters long" << endl;
        return -1;
    }
    // use parity check to validate the isbn number
    int sum = 0;
    for (int i = 0; i < 12; i++) {
        if (i % 2 == 0) {
            sum += isbn[i].digitValue();
        } else {
            sum += isbn[i].digitValue() * 3;
        }
    }
    int checkDigit = sum % 10;
    if (checkDigit != 0) {
        cerr << "Error: ISBN is invalid" << endl;
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

QJsonObject Text::getSubclassParams() const override {
    QJsonObject json;
    json["pages"] = pages;
    json["isbn"] = isbn;
    json["bindingType"] = bindingType;
    json["textFormat"] = textFormat;
    return json;
}

int Text::loadSubclassParams(const QJsonObject& json) const override {
    if (json.contains("pages")) {
        setPages(json["pages"].toInt());
    } else {
        std::cerr << "Error: Missing 'pages' in JSON object\n";
        return -1;
    }
    if (json.contains("isbn")) {
        setIsbn(json["isbn"].toString());
    } else {
        std::cerr << "Error: Missing 'isbn' in JSON object\n";
        return -1;
    }
    if (json.contains("bindingType")) {
        setBindingType(json["bindingType"].toString());
    } else {
        std::cerr << "Error: Missing 'bindingType' in JSON object\n";
        return -1;
    }
    if (json.contains("textFormat")) {
        setTextFormat(json["textFormat"].toString());
    } else {
        std::cerr << "Error: Missing 'textFormat' in JSON object\n";
        return -1;
    }

    return 0;
}
#include <string>
#include <iostream>
#include "text.h"

using namespace std;

int Text::setIsbn(const string& isbn) {
    if (isbn.length() != 13) {
        cerr << "Error: ISBN must be 13 characters long" << endl;
        return -1;
    }
    // use parity check to validate the isbn number
    int sum = 0;
    for (int i = 0; i < 12; i++) {
        if (i % 2 == 0) {
            sum += isbn[i] - '0';
        } else {
            sum += (isbn[i] - '0') * 3;
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

int Text::setBindingType(const string& bindingType) {
    this->bindingType = bindingType;
    return 0;
}

int Text::setTextFormat(const string& textFormat) {
    this->textFormat = textFormat;
    return 0;
}
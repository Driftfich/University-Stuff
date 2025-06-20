/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: This file contains the Result struct, which is used to return the result of a function.
It is used to return a boolean value and a message.
The boolean value is true if the operation was successful, false otherwise.
The message is a string that contains the result of the operation.
The error code is an integer that contains the error code of the operation.
*/

#ifndef RETURNS_H
#define RETURNS_H
#include <QString>

struct Result {
    bool success;
    QString msg;
    int errorCode;

    static Result Success(const QString& msg = QString("Operation successful")) {
        return {true, msg, 0};
    }

    static Result Error(const QString& msg, int errorCode = -1) {
        return {false, msg, errorCode};
    }

    // when converting to bool, it returns true if success is true
    operator bool() const {
        return success;
    }
    // string conversion
    operator QString() const {
        return msg;
    }

    operator int() const {
        return errorCode;
    }
};

#endif // RETURNS_H